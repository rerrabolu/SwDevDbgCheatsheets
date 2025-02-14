import os
import subprocess

# Global list of user-specified modules
usr_mod_list = []

# A set to track functions of a module without duplicates
usr_mod_func_set = set()

# A dictionary to link module names with their functions
usr_mod_func_map = {}

#
# A helper functions to print the help screen as to how
# the script must be invoked
#
def printHelpScreen():
    """Print the help screen with available modes."""
    print("\nScript requires one input: '1' or '2'")
    print("\t Mode-1 Enables Function Trace")
    print("\t Mode-2 Disables Function Trace\n")


#
# A helper functions to print the various functions that
# belong to a module
#
def printModFuncs():
    """Print the list of modules and their functions."""
    for mod_name in usr_mod_list:
        print(f"\nName Of Module: {mod_name}")
        func_list = usr_mod_func_map.get(mod_name, [])
        for func in func_list:
            print(f"\tName Of Function: {func}")


#
# A helper function to log the various modules and their
# functions that are being setup for data collection
#
def logModFuncs(logFile):
    """Log the list of modules and their functions to a file."""
    with open(logFile, 'a') as f:
        for mod_name in usr_mod_list:
            f.write(f"Name Of Module: {mod_name}\n")
            func_list = usr_mod_func_map.get(mod_name, [])
            for func in func_list:
                f.write(f"\tName Of Function: {func}\n")


#
# Prompt user to input a valid kernel module. A module's
# name is considered valid only if the following conditions
# are satisfied:
#   Module is loaded as determine by lsmod
#   Module's name matches completely with one reported by lsmod
#       i.e. it cannot be a substring of the module name
#
def isModuleNameValid(usrInput):
    """Check if a given module name is valid (currently loaded and not seen earlier)."""
    #if usrInput == 'END':
    #    return True

    if usrInput == 'KERNEL_CORE':
        return True

    # Get the list of currently loaded modules
    module_list = subprocess.check_output(['lsmod']).decode().splitlines()[1:]
    module_list = [line.split()[0] for line in module_list]

    # Check if input matches a valid module name
    if usrInput not in module_list:
        return False

    # Check if input module was already selected
    if usrInput in usr_mod_list:
        return False

    return True

#
# Get the list of functions of kernel's core
# that match user input. These functions don't
# belong to a module
#
def getCoreFuncs(func_name):
    func_list = []

    try:
        # Open /proc/kallsyms to read line-by-line
        with open("/proc/kallsyms", "r") as f:
            for line in f:

                # Debug: Print the line for verification
                # print(f"Reading line: {line.strip()}")  # Print raw line

                # Check if the line contains the function name
                if func_name in line:

                    # Debug: Print the line for verification
                    # print(f"Found module {func_name} in line.")

                    # Check if the line contains or 't' 'T', indicating
                    # a function symbol. Normalize whitespace (space or
                    # tab) and check for 't' or 'T' in tokenized form
                    line_parts = line.split()  # Tokenize by whitespace

                    # Checking 'T' or 't' in the 2nd field
                    if len(line_parts) > 2 and line_parts[1] in ['T', 't']:

                        # Debug: Print the line for verification
                        # print(f"Found 'T' or 't' in line, processing...")

                        # The function name is the third field (index 2)
                        func = line_parts[2].strip()
                        func_list.append(func)

                        # Debug: Print the line for verification
                        # print(f"Added function: {func}")

    except Exception as e:
        print(f"Error occurred: {e}")

    return func_list


#
# Returns the list of functions of a kernel module as
# given by input parameter. It consults system file
# /proc/kallsyms in building this list. The following
# is a brief description of the logic:
#
#   Using grep collect the lines that have module name
#   For each line:
#       Tokenize the lines using ' ' or '\t' as delimiters
#       Using grep collect the lines that have 't' or 'T' in them
#       Add the token (filed 3) that is function name into a list
#
def getModuleFuncs(mod_name):
    func_list = []

    try:
        # Open /proc/kallsyms to read line-by-line
        with open("/proc/kallsyms", "r") as f:
            for line in f:

                # Debug: Print the line for verification
                # print(f"Reading line: {line.strip()}")  # Print raw line

                # Check if the line contains the module name
                if mod_name in line:

                    # Debug: Print the line for verification
                    # print(f"Found module {mod_name} in line.")

                    # Check if the line contains ' T ', indicating a function symbol
                    # Normalize whitespace (space or tab) and check for ' T ' in tokenized form
                    line_parts = line.split()  # Tokenize by whitespace

                    # Checking 'T' or 't' in the 2nd field
                    if len(line_parts) > 2 and line_parts[1] in ['T', 't']:

                        # Debug: Print the line for verification
                        # print(f"Found 'T' or 't' in line, processing...")

                        # The function name is the third field (index 2)
                        func = line_parts[2].strip()
                        func_list.append(func)

                        # Debug: Print the line for verification
                        # print(f"Added function: {func}")

    except Exception as e:
        print(f"Error occurred: {e}")

    return func_list


#
# Determine if user input of function name is valid. A function name
# is considered valid if it matches one or more functions names that
# belong to the kernel module whose name is given as input. Therefore
# a user can give a substring or the full name of the function. For
# example, user can give as input "ev_" to pick up functions dev_read
# and dev_write or give them fully, depending upon their choice
#
def isFuncNameValid(usrInput, mod_name):
    """Return a list of valid function names matching user input in a module."""
    cand_list = []

    # Get the list of functions that belong to
    # Kernel core
    if mod_name == 'KERNEL_CORE':
        mod_func_list = getCoreFuncs(usrInput)
        for func in mod_func_list:
            if usrInput in func:
                cand_list.append(func)
        return cand_list

    # Get the list of functions for the module
    mod_func_list = getModuleFuncs(mod_name)

    # Filter for functions that match user input
    for func in mod_func_list:
        if usrInput in func:
            cand_list.append(func)

    return cand_list


#
# Write the various functions that belong to various modules
# that the user has specified to be traced
#
def setupModFuncs():

    """Setup Ftrace filter with the selected functions for each module."""
    for mod_name in usr_mod_list:
        # print("Name of module whose functions are written to filter: ", mod_name)
        func_list = usr_mod_func_map.get(mod_name, [])
        for func in func_list:
            # print("Name of function written to filter: ", func)
            with open("/sys/kernel/debug/tracing/set_ftrace_filter", "a") as f:
                f.write(func)


#
# Resets FTrace's control plane files:
#
#   "trace"
#   "current_tracer"
#   "set_ftrace_filter"
#
def setFtraceBaseline():

    # Reset FTrace data file
    with open("/sys/kernel/debug/tracing/trace", "w") as f:
        f.write("")

    # Reset FTrace filter file
    with open("/sys/kernel/debug/tracing/set_ftrace_filter", "w") as f:
        f.write("")

    # Reset Enable function tracer in Ftrace
    with open("/sys/kernel/debug/tracing/current_tracer", "w") as f:
        f.write("nop")

    # Enable FTrace to begin collection of data
    with open("/sys/kernel/debug/tracing/tracing_on", "w") as f:
        f.write("0")


#
# Enable FTrace's control plane files:
#
#   "trace"
#   "current_tracer"
#   "set_ftrace_filter"
#
def enableFtraceBaseline():

    # Populate FTrace filter file
    setupModFuncs()

    # Enable function tracer in Ftrace
    with open("/sys/kernel/debug/tracing/current_tracer", "w") as f:
        f.write("function")

    # Enable FTrace to begin collection of data
    with open("/sys/kernel/debug/tracing/tracing_on", "w") as f:
        f.write("1")


#
# Executes the logic to enable FTrace data collection
#
def handle_mode_1():

    """Handle Mode 1: Enable Function Trace."""
    global usr_mod_func_set

    while True:
        # Get module name input from user
        print("\nEnter 'END' to terminate MODULE input")
        mod_name = input("Enter Module Name / 'KERNEL_CORE' : ")

        # Indicates user has no modules of interest
        if mod_name == 'END':
            break

        if not isModuleNameValid(mod_name):
            print("Invalid module name. Try again.")
            continue

        save_funcs = False
        usr_mod_func_set.clear()

        # Collect functions for the module
        while True:
            print("\n\tEnter 'END' to terminate FUNCTION input")
            func_name = input(f"\tEnter function name for {mod_name} : ")
            if func_name == 'END':
                break

            func_list = isFuncNameValid(func_name, mod_name)
            if not func_list:
                print("No matching functions found.")
                continue

            for func in func_list:
                if func not in usr_mod_func_set:
                    usr_mod_func_set.add(func)
                    save_funcs = True

        if save_funcs:
            usr_mod_list.append(mod_name)
            usr_mod_func_map[mod_name] = list(usr_mod_func_set)

    if input("\nList the modules and their functions? (y/n): ").lower() == 'y':
        printModFuncs()

    if input("\nLog the modules and their functions? (y/n): ").lower() == 'y':
        logFile = input("Enter file name: ")
        logModFuncs(logFile)

    # Program Ftrace control plane to baseline condition
    setFtraceBaseline()

    # Program Ftrace control plane per user directions
    enableFtraceBaseline()

    print(" ")

#
# Executes the logic to collecte FTrace data
#
def handle_mode_2():

    """Handle Mode 2: Disable Function Trace and log trace data."""
    # Disable function tracing
    with open("/sys/kernel/debug/tracing/tracing_on", "w") as f:
        f.write("0")

    # Log the trace data to a file
    logFile = input("\nFilename to log tracing data: ")
    with open("/sys/kernel/debug/tracing/trace", "r") as trace_file, open(logFile, "w") as out_file:
        out_file.write(trace_file.read())

    # Reset the trace filter file
    with open("/sys/kernel/debug/tracing/set_ftrace_filter", "w") as f:
        f.write("")

    # Reset the trace file
    with open("/sys/kernel/debug/tracing/trace", "w") as f:
        f.write("")

    # Reset the current_tracer file
    with open("/sys/kernel/debug/tracing/current_tracer", "w") as f:
        f.write("nop")

    print("");


def main():
    """Main entry point of the script."""
    if len(os.sys.argv) < 2:
        printHelpScreen()
        return

    mode = os.sys.argv[1]
    if mode == '1':
        handle_mode_1()
    elif mode == '2':
        handle_mode_2()
    else:
        printHelpScreen()

#
# Starting point of the execution
#
if __name__ == "__main__":
    main()


