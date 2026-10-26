#!/usr/bin/env python3
#
# Prints properties of PCIe devices by joining the information
# Linux exports under /sys/bus/pci/slots (FIDs on s390) with the
# information under /sys/bus/pci/devices (BDFs).
#
# Combines pci_info_1.py (complete flow, summary) and pci_info_2.py
# (debug / verbose output, flexible device matching).
#

import argparse
import os
import sys
from datetime import datetime
from typing import Dict, List, Tuple


class PCIeDeviceAnalyzer:
    def __init__(self, debug: bool = False, verbose: bool = False):
        # Directory that has FIDs of devices
        self.slot_dir = "/sys/bus/pci/slots"

        # Directory that has BDFs of devices
        self.dev_dir = "/sys/bus/pci/devices"

        # Output controls
        self.debug = debug
        self.verbose = verbose

        # List of files in Slots directory whose contents
        # will be read. Values are associated with labels
        self.slot_files = {
            "address": "LADDR"
        }

        # List of files in Devices directory whose contents
        # will be read. Values are associated with labels
        self.dev_files = {
            "config": "LCFG",
            "enable": "LEN"
        }

        # Properties of each PCIe device, one list of
        # (label, value) pairs per device
        self.devices: List[List[Tuple[str, str]]] = []

        # Raw config space of each device, for the hex dump
        self.configs: List[bytes] = []

        # Directory lists
        self.slot_list: List[str] = []
        self.dev_list: List[str] = []

        # Statistics
        self.num_matched = 0

    def debug_print(self, message: str, indent_level: int = 0):
        """Print debug messages only if debug mode is enabled"""
        if self.debug:
            print(f"[DEBUG] {'  ' * indent_level}{message}")

    def verbose_print(self, message: str, indent_level: int = 0):
        """Print progress messages only if verbose or debug mode is enabled"""
        if self.verbose or self.debug:
            print(f"{'  ' * indent_level}{message}")

    @staticmethod
    def print_header(title: str, char: str = "=", width: int = 60):
        """Print a formatted header"""
        print(char * width)
        print(title.center(width))
        print(char * width)

    def get_directory_lists(self):
        """Get list of directories in slots and devices directories"""

        # Get slot directories (on s390 the names are FIDs)
        if os.path.isdir(self.slot_dir):
            self.slot_list = sorted(d for d in os.listdir(self.slot_dir)
                                    if os.path.isdir(os.path.join(self.slot_dir, d)))
            self.debug_print(f"Found slot directories: {self.slot_list}")
        else:
            print(f"Warning: Slot directory {self.slot_dir} does not exist")

        # Get device directories (names are BDFs and contain ":")
        if os.path.isdir(self.dev_dir):
            self.dev_list = sorted(d for d in os.listdir(self.dev_dir)
                                   if os.path.isdir(os.path.join(self.dev_dir, d)) and ":" in d)
            self.debug_print(f"Found device directories: {self.dev_list}")
        else:
            print(f"Warning: Device directory {self.dev_dir} does not exist")

    def safe_read_text(self, file_path: str) -> str:
        """Safely read a text file and return its contents as a string"""
        self.debug_print(f"Reading text file: {file_path}", 1)
        try:
            with open(file_path, 'r') as fd:
                return fd.read().strip()
        except (OSError, UnicodeDecodeError) as e:
            print(f"Warning: Could not read {file_path}: {e}")
            return ""

    def safe_read_binary(self, file_path: str) -> bytes:
        """Safely read a binary file (e.g. PCI config space)"""
        self.debug_print(f"Reading binary file: {file_path}", 1)
        try:
            with open(file_path, 'rb') as fd:
                return fd.read()
        except OSError as e:
            print(f"Warning: Could not read {file_path}: {e}")
            return b""

    @staticmethod
    def describe_config(data: bytes) -> str:
        """Summarize config space: vendor / device IDs and bytes read"""
        if len(data) < 4:
            return f"({len(data)} bytes read)"
        vendor = int.from_bytes(data[0:2], "little")
        device = int.from_bytes(data[2:4], "little")
        return f"vendor=0x{vendor:04x} device=0x{device:04x} ({len(data)} bytes read)"

    def find_matching_device(self, addr: str) -> str:
        """Find the device directory (BDF) that belongs to a slot address.

        A slot address is domain:bus:device (e.g. 0000:00:00) while the
        device directory adds the function number (e.g. 0000:00:00.0).
        """
        self.debug_print(f"Searching for device matching address: {addr}", 1)
        if not addr:
            return ""

        # Exact match
        if addr in self.dev_list:
            self.debug_print(f"Found exact match: {addr}", 1)
            return addr

        # Match with function number appended, lowest function first
        addr_clean = addr.strip().lower()
        for dev in self.dev_list:
            if dev.lower().startswith(addr_clean + "."):
                self.debug_print(f"Found match with function number: {dev}", 1)
                return dev

        self.debug_print(f"No match found for: {addr}", 1)
        return ""

    def init_prop_list(self):
        """Collect the properties of every slot and its matching device"""

        self.get_directory_lists()
        print(f"Found {len(self.slot_list)} slot directories")
        print(f"Found {len(self.dev_list)} device directories")
        print("")

        for idx, slot in enumerate(self.slot_list, 1):
            self.verbose_print(f"Processing slot {idx}/{len(self.slot_list)}: {slot}")
            props: List[Tuple[str, str]] = []
            config = b""

            # Capture Slot name as FID
            props.append(("FID", slot))

            # Read the files of the slot
            slot_path = os.path.join(self.slot_dir, slot)
            addr = ""
            for file_name, label in self.slot_files.items():
                value = self.safe_read_text(os.path.join(slot_path, file_name))
                self.verbose_print(f"{label:<6} = {value}", 1)
                props.append((label, value))

                # Capture value of address to join values from devices directory
                if label == "LADDR":
                    addr = value

            # Read the files of the matching device
            dev = self.find_matching_device(addr)
            if dev:
                self.num_matched += 1
                dev_path = os.path.join(self.dev_dir, dev)
                self.verbose_print(f"Matching device: {dev_path}", 1)
                props.append(("BDF", dev))
                for file_name, label in self.dev_files.items():
                    file_path = os.path.join(dev_path, file_name)
                    if file_name == "config":
                        config = self.safe_read_binary(file_path)
                        value = self.describe_config(config)
                    else:
                        value = self.safe_read_text(file_path)
                    self.verbose_print(f"{label:<6} = {value}", 1)
                    props.append((label, value))
            else:
                print(f"Warning: No matching device found for address: {addr}")
                # Still add placeholder entries for missing device files
                props.append(("BDF", ""))
                for label in self.dev_files.values():
                    props.append((label, ""))

            self.devices.append(props)
            self.configs.append(config)
            self.verbose_print("")

    def print_prop_list(self):
        """Print the properties organized by device"""
        print("")
        self.print_header("PCIe DEVICE PROPERTIES")
        for idx, props in enumerate(self.devices):
            print(f"\nProperties of PCIe device at index: {idx}")
            print("-" * 40)
            for label, value in props:
                print(f"Label: {label:<10} Value: {value}")
            if self.verbose and self.configs[idx]:
                print("Config space:")
                self.print_hex_dump(self.configs[idx])

    @staticmethod
    def print_hex_dump(data: bytes, width: int = 16):
        """Print bytes as offset + hex columns"""
        for off in range(0, len(data), width):
            chunk = data[off:off + width]
            print(f"  {off:03x}: {' '.join(f'{b:02x}' for b in chunk)}")

    def print_summary(self):
        """Print a summary of the analysis"""
        print("")
        self.print_header("SUMMARY")
        print(f"Slot directory: {self.slot_dir}")
        print(f"Device directory: {self.dev_dir}")
        print(f"Total slots processed: {len(self.slot_list)}")
        print(f"Total devices found: {len(self.dev_list)}")
        print(f"Slots matched to a device: {self.num_matched}")
        print(f"Slot files monitored: {list(self.slot_files.keys())}")
        print(f"Device files monitored: {list(self.dev_files.keys())}")


def parse_arguments():
    """Parse command line arguments"""
    parser = argparse.ArgumentParser(
        description="PCIe Device Analyzer - Analyze PCIe slots and devices",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 pci_dev_info.py              # Normal mode
  python3 pci_dev_info.py --verbose    # Progress and config space hex dump
  python3 pci_dev_info.py --debug      # Debug mode (implies progress output)
  sudo python3 pci_dev_info.py -v      # Full config space (non-root: first 64 bytes)
        """
    )
    parser.add_argument('-d', '--debug', action='store_true',
                        help='Enable debug output')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Show progress and a hex dump of config space')
    return parser.parse_args()


def main():
    """Main function"""
    args = parse_arguments()

    print("PCIe Device Analyzer")
    print("=" * 20)
    if args.debug:
        print("DEBUG MODE ENABLED")
    if args.verbose:
        print("VERBOSE MODE ENABLED")
    print(f"Started at: {datetime.now():%a %b %d %H:%M:%S %Y}")
    print("")

    # Check if running as root
    if os.geteuid() != 0:
        print("Warning: Not running as root. Some files may not be readable,")
        print("and only the first 64 bytes of config space are visible.")
        print("")

    analyzer = PCIeDeviceAnalyzer(debug=args.debug, verbose=args.verbose)
    try:
        analyzer.init_prop_list()
        analyzer.print_prop_list()
        analyzer.print_summary()
    except KeyboardInterrupt:
        print("\nInterrupted by user")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        if args.debug:
            import traceback
            traceback.print_exc()
        sys.exit(1)

    print(f"\nCompleted at: {datetime.now():%a %b %d %H:%M:%S %Y}")


if __name__ == "__main__":
    main()
