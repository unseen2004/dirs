#!/bin/bash

# Hide the cursor
tput civis
# Clear the screen
clear

# Handle cleanup on exit
trap "tput cnorm; clear; exit" SIGINT SIGTERM

# Get terminal size
rows=$(tput lines)
cols=$(tput cols)

# Define maze dimensions
maze_height=$((rows - 2))   # Leave space for status/info
maze_width=$((cols / 2 - 2))  # Adjust for character width

# Initialize maze grid
declare -A maze
for ((y=0; y<maze_height; y++)); do
    for ((x=0; x<maze_width; x++)); do
        maze[$x,$y]=1  # Wall
    done
done

# Stack for DFS
stack=()

# Directions: up, down, left, right
dx=(0 0 -1 1)
dy=(-1 1 0 0)

# Function to check if cell is in bounds
in_bounds() {
    local x=$1
    local y=$2
    if (( x >= 0 && x < maze_width && y >= 0 && y < maze_height )); then
        return 0
    else
        return 1
    fi
}

# Generate maze using Randomized DFS
generate_maze() {
    local x=$1
    local y=$2

    maze[$x,$y]=0  # Passage
    stack+=("$x,$y")

    while [ ${#stack[@]} -gt 0 ]; do
        current="${stack[-1]}"
        IFS=',' read -r x y <<< "$current"

        # Randomly order directions
        dirs=(0 1 2 3)
        for ((i=3; i>0; i--)); do
            j=$((RANDOM % (i+1)))
            tmp=${dirs[i]}
            dirs[i]=${dirs[j]}
            dirs[j]=$tmp
        done

        moved=0
        for d in "${dirs[@]}"; do
            nx=$((x + dx[d]*2))
            ny=$((y + dy[d]*2))

            if in_bounds $nx $ny && [ "${maze[$nx,$ny]}" -eq 1 ]; then
                maze[$((x + dx[d])),$((y + dy[d]))]=0  # Remove wall
                maze[$nx,$ny]=0  # Passage
                stack+=("$nx,$ny")
                moved=1
                break
            fi
        done

        if [ $moved -eq 0 ]; then
            unset 'stack[-1]'
        fi
    done
}

# Starting point for maze generation
generate_maze 1 1

# Player position
player_x=1
player_y=1

# Exit position
exit_x=$((maze_width - 2))
exit_y=$((maze_height - 2))
maze[$exit_x,$exit_y]=0

# Function to draw the maze
draw_maze() {
    clear
    for ((y=0; y<maze_height; y++)); do
        for ((x=0; x<maze_width; x++)); do
            if [ $x -eq $player_x ] && [ $y -eq $player_y ]; then
                echo -ne "\e[32m@\e[0m"  # Player
            elif [ $x -eq $exit_x ] && [ $y -eq $exit_y ]; then
                echo -ne "\e[31mX\e[0m"  # Exit
            elif [ "${maze[$x,$y]}" -eq 1 ]; then
                echo -ne "\u2588"  # Wall
            else
                echo -ne " "  # Passage
            fi
        done
        echo
    done
    echo "Use arrow keys to move. Reach the exit 'X' to win. Press 'q' to quit."
}

# Disable input buffering and echo
stty -echo -icanon time 0 min 0

# Main game loop
while true; do
    draw_maze

    # Read user input
    read -rsn1 input
    read -rsn2 -t 0.001 remaining  # For arrow keys

    case "$input$remaining" in
        $'\e[A')  # Up
            new_x=$player_x
            new_y=$((player_y - 1))
            ;;
        $'\e[B')  # Down
            new_x=$player_x
            new_y=$((player_y + 1))
            ;;
        $'\e[D')  # Left
            new_x=$((player_x - 1))
            new_y=$player_y
            ;;
        $'\e[C')  # Right
            new_x=$((player_x + 1))
            new_y=$player_y
            ;;
        q)
            break
            ;;
        *)
            new_x=$player_x
            new_y=$player_y
            ;;
    esac

    # Move player if the next position is not a wall
    if [ "${maze[$new_x,$new_y]}" -eq 0 ]; then
        player_x=$new_x
        player_y=$new_y
    fi

    # Check for win condition
    if [ $player_x -eq $exit_x ] && [ $player_y -eq $exit_y ]; then
        draw_maze
        echo "Congratulations! You've escaped the maze!"
        break
    fi
done

# Restore terminal settings
stty echo icanon
tput cnorm
