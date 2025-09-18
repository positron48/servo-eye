#!/usr/bin/env python3
"""
HTML to C++ Compiler with Gzip Compression
Compiles HTML files to C++ header files with gzip compression
"""

import os
import sys
import gzip
import base64
from pathlib import Path

def compress_html(html_content):
    """Compress HTML content using gzip"""
    return gzip.compress(html_content.encode('utf-8'))

def html_to_cpp(html_file, output_file, var_name):
    """Convert HTML file to C++ header with direct string embedding"""
    
    # Read HTML file
    with open(html_file, 'r', encoding='utf-8') as f:
        html_content = f.read()
    
    # Escape quotes and backslashes for C++ raw string
    escaped_content = html_content.replace('\\', '\\\\').replace('"', '\\"')
    
    # Get file size
    original_size = len(html_content.encode('utf-8'))
    
    # Generate C++ header
    cpp_content = f'''#ifndef {var_name.upper()}_H
#define {var_name.upper()}_H

#include <Arduino.h>

// Auto-generated from {os.path.basename(html_file)}
// Size: {original_size} bytes

const char {var_name}_data[] = R"rawliteral({html_content})rawliteral";

const size_t {var_name}_size = {original_size};

#endif // {var_name.upper()}_H
'''
    
    # Write C++ header
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(cpp_content)
    
    print(f"✓ Compiled {html_file} -> {output_file}")
    print(f"  Size: {original_size} bytes")

def main():
    """Main function"""
    if len(sys.argv) != 4:
        print("Usage: python3 html_to_cpp.py <input.html> <output.h> <var_name>")
        sys.exit(1)
    
    html_file = sys.argv[1]
    output_file = sys.argv[2]
    var_name = sys.argv[3]
    
    if not os.path.exists(html_file):
        print(f"Error: HTML file '{html_file}' not found")
        sys.exit(1)
    
    # Create output directory if it doesn't exist
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    
    html_to_cpp(html_file, output_file, var_name)

if __name__ == "__main__":
    main()
