
#!/bin/bash

# Function to display process information
get_process_info() {
    pid=$1
    # Define the path to the process's information files
    pid_dir="/proc/$pid"
    
    # Check if the process directory exists and is readable
    if [[ -d $pid_dir && -r $pid_dir ]]; then
        # Read the required information from the /proc/<pid>/status file
        comm=$(grep -E '^Name:' $pid_dir/status | awk '{print $2}')
        state=$(grep -E '^State:' $pid_dir/status | awk '{print $2}')
        tty=$(grep -E '^Tty:' $pid_dir/status | awk '{print $2}')
        ppid=$(grep -E '^PPid:' $pid_dir/status | awk '{print $2}')
        rss=$(grep -E '^VmRSS:' $pid_dir/status | awk '{print $2}')
        pgid=$(grep -E '^Pgid:' $pid_dir/status | awk '{print $2}')
        sid=$(grep -E '^Sid:' $pid_dir/status | awk '{print $2}')

        # Count open files
        open_files=$(lsof -p $pid 2>/dev/null | wc -l)

        # Display the information for this process 
        echo -e "$pid\t$ppid\t$comm\t$state\t$tty\t$rss\t$pgid\t$sid\t$open_files"
    fi
}

# Print header
echo -e "PID\tPPID\tCOMMAND\tSTATE\tTTY\tRSS\tPGID\tSID\tOPEN_FILES"

# Loop over all directories in /proc and process only those that are numeric (i.e., PIDs)
for pid in $(ls -1 /proc | grep '^[0-9]\+$'); do
    # Call the function to get process info for each PID
    get_process_info $pid
done


