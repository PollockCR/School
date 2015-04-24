~ Operating System Simulator - Phase 3.0 ~

1. Unzip contents
2. From terminal, type "make" from inside PA04 directory
3. Ensure valid configuration and meta-data files, including filepaths
   - Note: Configuration and meta-data files must match those detailed in SimulatorAssignment_v12.pdf for sucessful process simulation. Missing or corrupted files will be reported by the program. 
   - Note: All previously implemented scheduling codes (FIFO, SJF, SRTF) still work, as well as newly implemented codes (FIFO-P, RR, SRTF-P). Program will execute and choose appropriate functions based on code given. For old scheduling codes, a time quantum is still required in config file, but time quantum will not be used. 
4. Run simulator - From inside the directory in the terminal, type "./OS_Phase_3 config" and replace "config" with appropriate filepath/filename
   - Note: g++ must be installed (v4.3 or later) and this must be ran on a Linux machine
5. Observe operations on monitor and/or in log file
