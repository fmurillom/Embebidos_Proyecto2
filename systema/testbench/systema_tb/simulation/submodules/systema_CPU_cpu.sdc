# Legal Notice: (C)2020 Altera Corporation. All rights reserved.  Your
# use of Altera Corporation's design tools, logic functions and other
# software and tools, and its AMPP partner logic functions, and any
# output files any of the foregoing (including device programming or
# simulation files), and any associated documentation or information are
# expressly subject to the terms and conditions of the Altera Program
# License Subscription Agreement or other applicable license agreement,
# including, without limitation, that your use is for the sole purpose
# of programming logic devices manufactured by Altera and sold by Altera
# or its authorized distributors.  Please refer to the applicable
# agreement for further details.

#**************************************************************
# Timequest JTAG clock definition
#   Uncommenting the following lines will define the JTAG
#   clock in TimeQuest Timing Analyzer
#**************************************************************

#create_clock -period 10MHz {altera_reserved_tck}
#set_clock_groups -asynchronous -group {altera_reserved_tck}

#**************************************************************
# Set TCL Path Variables 
#**************************************************************

set 	systema_CPU_cpu 	systema_CPU_cpu:*
set 	systema_CPU_cpu_oci 	systema_CPU_cpu_nios2_oci:the_systema_CPU_cpu_nios2_oci
set 	systema_CPU_cpu_oci_break 	systema_CPU_cpu_nios2_oci_break:the_systema_CPU_cpu_nios2_oci_break
set 	systema_CPU_cpu_ocimem 	systema_CPU_cpu_nios2_ocimem:the_systema_CPU_cpu_nios2_ocimem
set 	systema_CPU_cpu_oci_debug 	systema_CPU_cpu_nios2_oci_debug:the_systema_CPU_cpu_nios2_oci_debug
set 	systema_CPU_cpu_wrapper 	systema_CPU_cpu_debug_slave_wrapper:the_systema_CPU_cpu_debug_slave_wrapper
set 	systema_CPU_cpu_jtag_tck 	systema_CPU_cpu_debug_slave_tck:the_systema_CPU_cpu_debug_slave_tck
set 	systema_CPU_cpu_jtag_sysclk 	systema_CPU_cpu_debug_slave_sysclk:the_systema_CPU_cpu_debug_slave_sysclk
set 	systema_CPU_cpu_oci_path 	 [format "%s|%s" $systema_CPU_cpu $systema_CPU_cpu_oci]
set 	systema_CPU_cpu_oci_break_path 	 [format "%s|%s" $systema_CPU_cpu_oci_path $systema_CPU_cpu_oci_break]
set 	systema_CPU_cpu_ocimem_path 	 [format "%s|%s" $systema_CPU_cpu_oci_path $systema_CPU_cpu_ocimem]
set 	systema_CPU_cpu_oci_debug_path 	 [format "%s|%s" $systema_CPU_cpu_oci_path $systema_CPU_cpu_oci_debug]
set 	systema_CPU_cpu_jtag_tck_path 	 [format "%s|%s|%s" $systema_CPU_cpu_oci_path $systema_CPU_cpu_wrapper $systema_CPU_cpu_jtag_tck]
set 	systema_CPU_cpu_jtag_sysclk_path 	 [format "%s|%s|%s" $systema_CPU_cpu_oci_path $systema_CPU_cpu_wrapper $systema_CPU_cpu_jtag_sysclk]
set 	systema_CPU_cpu_jtag_sr 	 [format "%s|*sr" $systema_CPU_cpu_jtag_tck_path]

#**************************************************************
# Set False Paths
#**************************************************************

set_false_path -from [get_keepers *$systema_CPU_cpu_oci_break_path|break_readreg*] -to [get_keepers *$systema_CPU_cpu_jtag_sr*]
set_false_path -from [get_keepers *$systema_CPU_cpu_oci_debug_path|*resetlatch]     -to [get_keepers *$systema_CPU_cpu_jtag_sr[33]]
set_false_path -from [get_keepers *$systema_CPU_cpu_oci_debug_path|monitor_ready]  -to [get_keepers *$systema_CPU_cpu_jtag_sr[0]]
set_false_path -from [get_keepers *$systema_CPU_cpu_oci_debug_path|monitor_error]  -to [get_keepers *$systema_CPU_cpu_jtag_sr[34]]
set_false_path -from [get_keepers *$systema_CPU_cpu_ocimem_path|*MonDReg*] -to [get_keepers *$systema_CPU_cpu_jtag_sr*]
set_false_path -from *$systema_CPU_cpu_jtag_sr*    -to *$systema_CPU_cpu_jtag_sysclk_path|*jdo*
set_false_path -from sld_hub:*|irf_reg* -to *$systema_CPU_cpu_jtag_sysclk_path|ir*
set_false_path -from sld_hub:*|sld_shadow_jsm:shadow_jsm|state[1] -to *$systema_CPU_cpu_oci_debug_path|monitor_go
