#!/usr/bin/env python3

import os
import sys
from pathlib import Path


def get_comment_style(file_ext):
    """Determine the appropriate comment syntax based on file extension"""
    # C-style languages
    if file_ext in ['.c', '.cpp', '.cc', '.h', '.hpp', '.java', '.js', '.jsx', '.ts', '.tsx', 
                   '.php', '.cs', '.go', '.swift', '.kt', '.scala', '.groovy', '.dart']:
        return '//'
    # Hash-style languages
    elif file_ext in ['.py', '.rb', '.pl', '.pm', '.sh', '.bash', '.r', '.yml', '.yaml', 
                     '.tcl', '.perl', '.ps1', '.cmake', '.makefile', '.Makefile']:
        return '#'
    # HTML/XML
    elif file_ext in ['.html', '.htm', '.xml', '.svg', '.xhtml', '.vue', '.jsx', '.tsx']:
        return '<!--'
    # CSS
    elif file_ext in ['.css', '.scss', '.sass', '.less']:
        return '/*'
    # LaTeX
    elif file_ext in ['.tex', '.latex']:
        return '%'
    # SQL
    elif file_ext in ['.sql']:
        return '--'
    # Lisp-style
    elif file_ext in ['.lisp', '.scm', '.rkt', '.clj', '.el']:
        return ';;'
    # Batch files
    elif file_ext in ['.bat', '.cmd']:
        return 'REM'
    # Lua
    elif file_ext in ['.lua']:
        return '--'
    # Default for unknown file types
    else:
        return '#'


def write_comment_header(outfile, comment_style, file_path):
    """Write the file header comment in the appropriate style"""
    if comment_style == '<!--':
        outfile.write(f"{comment_style} {file_path} -->\n")
    elif comment_style == '/*':
        outfile.write(f"{comment_style} {file_path} */\n")
    else:
        outfile.write(f"{comment_style} {file_path}\n")


def is_binary_file(file_path):
    """Check if a file is likely binary (non-text)"""
    # Common binary file extensions
    binary_extensions = {'.exe', '.dll', '.so', '.pyc', '.class', '.o', '.obj', '.bin', 
                        '.jpg', '.jpeg', '.png', '.gif', '.bmp', '.ico', '.pdf', '.zip', 
                        '.tar', '.gz', '.7z', '.rar', '.mp3', '.mp4', '.avi', '.mov'}
    
    ext = os.path.splitext(file_path)[1].lower()
    if ext in binary_extensions:
        return True
    
    # Try to read the first few bytes to check for binary content
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            f.read(1024)
        return False
    except UnicodeDecodeError:
        return True


def combine_files(input_dir, output_file):
    """Combine all files from input_dir and its subdirectories into output_file"""
    abs_output_path = os.path.abspath(output_file)
    
    with open(output_file, 'w', encoding='utf-8') as outfile:
        for root, _, files in os.walk(input_dir):
            for file in sorted(files):
                file_path = os.path.join(root, file)
                abs_file_path = os.path.abspath(file_path)
                
                # Skip the output file itself if it's in the directory
                if abs_file_path == abs_output_path:
                    continue
                
                # Get relative path from input_dir
                rel_path = os.path.relpath(file_path, input_dir)
                
                # Check if it's a binary file
                if is_binary_file(file_path):
                    continue
                
                try:
                    file_ext = os.path.splitext(file)[1].lower()
                    comment_style = get_comment_style(file_ext)
                    
                    with open(file_path, 'r', encoding='utf-8', errors='replace') as infile:
                        # Write file header
                        write_comment_header(outfile, comment_style, rel_path)
                        
                        # Write file content
                        content = infile.read()
                        outfile.write(content)
                        
                        # Ensure there's a newline at the end
                        if content and not content.endswith('\n'):
                            outfile.write('\n')
                        
                        # Add an extra empty line between files
                        outfile.write('\n')
                        
                except Exception as e:
                    print(f"Error processing {rel_path}: {e}")


def main():
    if len(sys.argv) < 2:
        print("Usage: python combine_files.py <input_directory> [output_file]")
        sys.exit(1)
    
    input_dir = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else "combined_files.txt"
    
    if not os.path.isdir(input_dir):
        print(f"Error: '{input_dir}' is not a valid directory")
        sys.exit(1)
    
    print(f"Processing directory: {input_dir}")
    print(f"Output will be written to: {output_file}")
    
    combine_files(input_dir, output_file)
    print(f"Files successfully combined into '{output_file}'")


if __name__ == "__main__":
    main()
