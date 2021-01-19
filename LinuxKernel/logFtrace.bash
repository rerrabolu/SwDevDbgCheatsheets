#!/bin/bash


## Exit the script if any step encounters error
set -e

## Check if user wants to enable or print Ftrace log
if [ "$#" -ne 1 ]; then
  echo "$0 requires input: 0 to print ftrace, 1 to enable collect ftrace"
  echo "Run script as follows:  $0 0 OR  $0 1"
  exit
fi


# Capture users Ftrace preference
logFlag=$1
traceDir='/sys/kernel/debug/tracing/'
if [[ "$logFlag" -eq 1 ]]; then

  # echo "Enabling stack trace data from Ftrace"
  echo "Enabling function graph mode for ftrace"
  echo "Enabling ftrace to collect function graph for amdgpu module"
  cd  $traceDir
  
  # Reset tracer first before enabling a particular mode
  #
  # @note: Using "function" is not very helpful. It dumps
  # a lot of information that may is not very helpful. It
  # could be use if tracing for a few functions
  #
  echo nop > current_tracer
  # echo function > current_tracer
  echo function_graph > current_tracer
  
  # Determine which functions should be traced
  # Choosing a module scope will dump a lot of
  # information, so try to limit to functions
  # with certain substrings
  #
  # @note: It is not possible to get function_graph
  # for a module. One must specify the function(s)
  # that should be tracked
  #
  # echo ':mod:amdgpu' >  set_ftrace_filter
  # echo 'kfd_*' >  set_ftrace_filter
  # echo 'amdgpu_*' >  set_ftrace_filter
  echo 'kfd_*' >  set_graph_function
  echo 'amdgpu_*' >  set_graph_function

  # echo 1 > /proc/sys/kernel/stack_tracer_enabled

  # cat  set_ftrace_filter | wc -l
  # cat  set_ftrace_filter > /tmp/amdgpuFuncs.txt
  cat  set_graph_function | wc -l
  cat  set_graph_function  > /tmp/amdgpuFuncs.txt
  echo 1 > tracing_on

else
  echo " "
  echo "Print ftrace buffer and reset it to OFF mode"
  cd  $traceDir
  echo " "
  echo "Function graph data from Ftrace"
  echo " "
  cat trace > /tmp/funcTraceDump.txt
  #cat trace | wc -l
  echo " "
  #echo "Stack Trace data from Ftrace"
  #echo " "
  #cat stack_trace
  #echo " "
  echo nop > current_tracer
  #echo >  set_ftrace_filter
  echo >  set_graph_function
  echo 0 > tracing_on
  #echo 0 > /proc/sys/kernel/stack_tracer_enabled
fi


