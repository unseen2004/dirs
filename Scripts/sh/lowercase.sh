#!/bin/bash

# Check if a directory is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

# Change to the specified directory
cd "$1" || { echo "Failed to open directory $1"; exit 1; }

for file in *; do
    # Skip if it's not a file
    [ -f "$file" ] || continue

    # Convert filename to lowercase
    new_file=$(echo "$file" | tr '[:upper:]' '[:lower:]')

    # Rename file if the new name is different
    if [ "$file" != "$new_file" ]; then
        mv -- "$file" "$new_file"
    fi
done

#       Usage
# chmod +x lowercase.sh
# ./lowercase.sh /path/to/directory

