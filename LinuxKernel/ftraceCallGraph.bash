#!/bin/bash


## Exit the script if any step encounters error
set -e

# Print Help message
function printHelp() {
  echo -e ""
  echo -e "\t$0 requires input: "
  echo -e "\t\t0 to print ftrace"
  echo -e "\t\t1 <func-substring> to enable collection of ftrace"
  echo -e "\tRun script as follows: "
  echo -e "\t\t$0 0"
  echo -e "\t\t\tOR"
  echo -e "\t\t$0 1 <func-substring>"
  echo -e ""
  exit
}

# Check if user wants to enable or print Ftrace log
if [ "$#" -eq 0 ]; then
	printHelp
fi

# Determine request to print is valid
if [ "$1" -eq 0 ]; then
	if [ "$#" -gt 1 ]; then
		printHelp
	fi
fi

# Determine request to enable ftrace is valid
if [ "$1" -eq 1 ]; then
	if [ "$#" -ne 2 ]; then
		printHelp
	fi
fi

#
#  Background: A "tracer" determines what kinds of logs can be
#  collected. The log is dumped into a ring buffer referenced by
#  file handle called "trace"

#  One can see what tracers are available by reading the text
#  file: cat /sys/kernel/debug/tracing/available_tracers
#  The list includes: timerlat osnoise hwlat blk mmiotrace
#		function_graph wakeup_dl wakeup_rt wakeup function nop
#
#  One must choose a tracer and apply it to current_tracer file 
#   echo function_graph > current_tracer
#  One can program current_tracer with only one tracer
#
#


# Capture users Ftrace preference
logFlag=$1
funcName=$2
traceDir='/sys/kernel/debug/tracing/'

# Variables used to capture data
timestamp=`printf '%(%Y-%m-%d-%M)T\n'`
ftraceDataDump="/tmp/ftraceDataDump_${timestamp}.txt"
ftraceFuncList="/tmp/ftraceFuncList_${timestamp}.txt"

# Cd into right wokring directory
cd  $traceDir

if [[ "$logFlag" -eq 1 ]]; then
  
	echo "Enabling function_graph mode of ftrace for functions with substring ${funcName}"

	#
	# The basic sequence is as follows:
	#   Stop recording trace
	#   Reset / Disable current_tracer
	#   Select and apply a tracer
	#   Determine which functions should be traced
	#

	# Stop recording in trace buffer
	echo 0 > tracing_on

	# Disable current_tracer
	echo nop > current_tracer

	# Set the trace ring buffer to 256 MiB
	# 256 MiB == 256 * 1024
	echo 262144 > buffer_size_kb

	# Clear the trace buffer of old data
	trace-cmd clear -a

	# Set the max-depth of call graph
	# The function being traced is at Level - 1
	# A max-depth of 2 will include child methods
	# but not those that are considered as grand-children
	#
	# Tune this parameter per needs
	#
	echo 1 > max_graph_depth

	# Using "function" is not very helpful. It dumps a lot
	# of information that may not be very helpful. It could
	# be used if tracing for a few functions
	#
	echo function_graph > current_tracer

	# Determine which functions should be traced
	# One can give a substring to collect trace
	# data on a whole lot of functions
	#
	# It is not possible to specify a module to
	# collection function graph
	#
	# echo 'kfd_*' >  set_graph_function
	# echo 'amdgpu_*' >  set_graph_function
	# Use >> to add functions to filter
	#
	echo '*'${funcName}'*' >  set_graph_function

	# Filter out tracing following functions
	echo 'unlock_page*' > set_graph_notrace
	echo 'folio_unlock*' > set_graph_notrace

	cat  set_graph_function > ${ftraceFuncList}
	numFuncs=`cat  set_graph_function | wc -l`
	echo "Number of functions being traced is: ${numFuncs}"
	echo "File ${ftraceFuncList} has the list of functions being traced"

	# Start recording in trace buffer
	echo 1 > tracing_on

else

	echo " "
	echo "Print function trace buffer into ${ftraceDataDump}"

	#
	# The basic sequence is as follows:
	#   Stop recording trace
	#   Reset the function_graph filter
	#   Copy or cat the trace ring buffer
	#     This must be done before reset
	#   Reset the current_tracer
	#

	# Stop recording in trace buffer
	echo 0 > tracing_on

	# Reset the function_graph filter
	echo >  set_graph_function
	echo >  set_graph_notrace

	# Echo trace data into log file
	cat trace > ${ftraceDataDump}
	echo " "

	# Reset the trace ringer buffer
	echo nop > current_tracer

fi


