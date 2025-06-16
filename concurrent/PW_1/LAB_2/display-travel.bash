#!/bin/bash

# **TRAVELERS SIMULATION VISUALIZER**
# Displays the movement of travelers and wild tenants on a 2D grid
# Usage: ./ex_2_enhanced | ./display-travel.bash

# **VISUALIZATION PARAMETERS**
DELAY=0.1  # Delay between frames (seconds)
CLEAR_SCREEN=1  # Set to 0 to disable screen clearing

# **COLOR DEFINITIONS** - Make visualization more readable
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# **BOARD INITIALIZATION**
declare -A board
declare -A last_board
traveler_count=0
board_width=0
board_height=0
frame_count=0

# **INPUT PROCESSING**: Read simulation parameters and events
while read line; do
    if [[ $line =~ ^-1[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+) ]]; then
        # **HEADER LINE**: Extract simulation parameters
        traveler_count=${BASH_REMATCH[1]}
        board_width=${BASH_REMATCH[2]}
        board_height=${BASH_REMATCH[3]}
        
        echo "=== TRAVELERS SIMULATION WITH WILD TENANTS ==="
        echo "Travelers: $traveler_count"
        echo "Board: ${board_width}x${board_height}"
        echo "Legend: A-Z=Travelers, a-z=Deadlocked, 0-9=Wild Tenants, .=Empty"
        echo "=========================================="
        
        # **BOARD INITIALIZATION**
        for ((y=0; y<board_height; y++)); do
            for ((x=0; x<board_width; x++)); do
                board[$x,$y]="."
                last_board[$x,$y]="."
            done
        done
        
    elif [[ $line =~ ^[[:space:]]*([0-9.-]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+(.+) ]]; then
        # **MOVEMENT EVENT**: Update board state
        timestamp=${BASH_REMATCH[1]}
        entity_id=${BASH_REMATCH[2]}
        x=${BASH_REMATCH[3]}
        y=${BASH_REMATCH[4]}
        symbol=$(echo "${BASH_REMATCH[5]}" | tr -d ' ')
        
        # **BOARD UPDATE**: Clear previous position and set new position
        for ((py=0; py<board_height; py++)); do
            for ((px=0; px<board_width; px++)); do
                if [[ "${board[$px,$py]}" == "$symbol" ]] && [[ "$px" != "$x" || "$py" != "$y" ]]; then
                    board[$px,$py]="."
                fi
            done
        done
        
        # **POSITION UPDATE**: Handle off-board positions (entity removal)
        if [[ $x -ge $board_width || $y -ge $board_height ]]; then
            # Entity left the board - represented as removal
            continue
        else
            board[$x,$y]="$symbol"
        fi
        
        # **DISPLAY UPDATE**: Show current board state
        if [[ $CLEAR_SCREEN -eq 1 ]]; then
            clear
        fi
        
        echo "=== Frame $((++frame_count)) - Time: $timestamp ==="
        
        # **BOARD RENDERING**: Display with colors for different entity types
        for ((y=0; y<board_height; y++)); do
            for ((x=0; x<board_width; x++)); do
                symbol="${board[$x,$y]}"
                
                # **COLOR CODING**: Different colors for different entity types
                if [[ "$symbol" =~ [A-Z] ]]; then
                    # Active travelers - green
                    echo -ne "${GREEN}$symbol${NC} "
                elif [[ "$symbol" =~ [a-z] ]]; then
                    # Deadlocked travelers - red
                    echo -ne "${RED}$symbol${NC} "
                elif [[ "$symbol" =~ [0-9] ]]; then
                    # Wild tenants - yellow
                    echo -ne "${YELLOW}$symbol${NC} "
                else
                    # Empty space - default
                    echo -ne "$symbol "
                fi
            done
            echo
        done
        
        echo "Legend: ${GREEN}A-Z${NC}=Active, ${RED}a-z${NC}=Deadlocked, ${YELLOW}0-9${NC}=Wild Tenants"
        echo "----------------------------------------"
        
        sleep $DELAY
    fi
done

echo "=== SIMULATION COMPLETED ==="
echo "Total frames: $frame_count"
