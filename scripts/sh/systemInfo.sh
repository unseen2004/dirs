#!/bin/bash

# Function to convert bytes to human-readable format
convert_bytes() {
    local bytes=$1
    if (( bytes < 1024 )); then
        echo "${bytes} B"
    elif (( bytes < 1048576 )); then
        echo "$(( bytes / 1024 )) KB"
    else
        echo "$(( bytes / 1048576 )) MB"
    fi
}

# Function to get network data
get_network_data() {
    local interface=$1
    local rx_bytes=$(grep "$interface" /proc/net/dev | awk '{print $2}')
    local tx_bytes=$(grep "$interface" /proc/net/dev | awk '{print $10}')
    echo "$rx_bytes $tx_bytes"
}

# Function to get CPU utilization
get_cpu_utilization() {
    grep -P '^cpu[0-9]+' /proc/stat | while read cpu_line; do
        local cpu_name=$(echo $cpu_line | awk '{print $1}')
        local user=$(echo $cpu_line | awk '{print $2}')
        local nice=$(echo $cpu_line | awk '{print $3}')
        local system=$(echo $cpu_line | awk '{print $4}')
        local idle=$(echo $cpu_line | awk '{print $5}')
        local iowait=$(echo $cpu_line | awk '{print $6}')
        local irq=$(echo $cpu_line | awk '{print $7}')
        local softirq=$(echo $cpu_line | awk '{print $8}')
        local steal=$(echo $cpu_line | awk '{print $9}')
        local guest=$(echo $cpu_line | awk '{print $10}')
        local guest_nice=$(echo $cpu_line | awk '{print $11}')

        # Calculate total and idle times
        local total=$((user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice))
        local total_idle=$((idle + iowait))

        # Calculate CPU utilization
        local utilization=0
        if [[ $total -ne 0 ]]; then
            utilization=$((100 * (total - total_idle) / total))
        fi

        # Get CPU frequency
        local freq=$(cat /sys/devices/system/cpu/${cpu_name}/cpufreq/scaling_cur_freq 2>/dev/null || echo "N/A")
        echo "$cpu_name: $utilization% @ ${freq}kHz"
    done
}

# Function to get uptime
get_uptime() {
    local uptime=$(awk '{print int($1)}' /proc/uptime)
    local days=$(( uptime / 86400 ))
    local hours=$(( (uptime % 86400) / 3600 ))
    local minutes=$(( (uptime % 3600) / 60 ))
    local seconds=$(( uptime % 60 ))
    echo "${days}d ${hours}h ${minutes}m ${seconds}s"
}

# Function to get battery status
get_battery_status() {
    local battery_status=$(grep "POWER_SUPPLY_CAPACITY=" /sys/class/power_supply/BAT0/uevent | cut -d= -f2)
    echo "${battery_status}%"
}

# Function to get system load
get_system_load() {
    awk '{print $1", "$2", "$3}' /proc/loadavg
}

# Function to get memory usage
get_memory_usage() {
    local total=$(awk '/MemTotal/ {print $2}' /proc/meminfo)
    local free=$(awk '/MemFree/ {print $2}' /proc/meminfo)
    local available=$(awk '/MemAvailable/ {print $2}' /proc/meminfo)
    echo "Total: $(convert_bytes $total) | Free: $(convert_bytes $free) | Available: $(convert_bytes $available)"
}

# Function to print bar chart
print_bar_chart() {
    local value=$1
    local max_value=$2
    local width=20
    local filled=$(( value * width / max_value ))
    local empty=$(( width - filled ))
    printf "["
    for ((i=0; i<filled; i++)); do printf "#"; done
    for ((i=0; i<empty; i++)); do printf " "; done
    printf "]"
}

# Main loop
declare -A rx_prev tx_prev
interfaces=$(ls /sys/class/net)
while true; do
    clear
    echo "System Information - $(date)"
    echo "=============================="
    
    echo "Network Data:"
    for interface in $interfaces; do
        if [[ "$interface" != "lo" ]]; then
            read rx_bytes tx_bytes <<< $(get_network_data $interface)
            if [[ -n "${rx_prev[$interface]}" ]]; then
                rx_rate=$(( (rx_bytes - rx_prev[$interface]) ))
                tx_rate=$(( (tx_bytes - tx_prev[$interface]) ))
                echo "Interface: $interface | RX: $(convert_bytes $rx_rate) | TX: $(convert_bytes $tx_rate)"
                echo -n "RX: "
                print_bar_chart $rx_rate 1048576
                echo ""
                echo -n "TX: "
                print_bar_chart $tx_rate 1048576
                echo ""
            fi
            rx_prev[$interface]=$rx_bytes
            tx_prev[$interface]=$tx_bytes
        fi
    done
    echo "------------------------------"
    
    echo "CPU Utilization & Frequency:"
    get_cpu_utilization
    echo "------------------------------"
    
    echo "Uptime: $(get_uptime)"
    echo "------------------------------"
    
    echo "Battery Status: $(get_battery_status)"
    echo "------------------------------"
    
    echo "System Load: $(get_system_load)"
    echo "------------------------------"
    
    echo "Memory Usage: $(get_memory_usage)"
    
    sleep 1
done
