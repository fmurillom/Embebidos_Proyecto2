# ------------------------------------------------------------------------------
# Top Level Simulation Script to source msim_setup.tcl
# ------------------------------------------------------------------------------
set QSYS_SIMDIR obj/default/runtime/sim
source msim_setup.tcl
# Copy generated memory initialization hex and dat file(s) to current directory
file copy -force D:/fmuri/Documents/TEC/XI/Embebidos/software/smart_clock/mem_init/hdl_sim/systema_RAM.dat ./ 
file copy -force D:/fmuri/Documents/TEC/XI/Embebidos/software/smart_clock/mem_init/systema_RAM.hex ./ 
