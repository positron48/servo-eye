#!/usr/bin/env python3
"""
Build script for HTML to C++ compilation
Automatically compiles HTML files to C++ headers during build process
"""

import os
import sys
import subprocess
from pathlib import Path

def compile_html_files():
    """Compile all HTML files to C++ headers"""
    
    # Get script directory
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    
    # Define paths
    web_dir = project_root / "web"
    src_dir = project_root / "src"
    html_compiler = script_dir / "html_to_cpp.py"
    
    # Check if web directory exists
    if not web_dir.exists():
        print(f"Warning: Web directory {web_dir} not found")
        return
    
    # Find all HTML files
    html_files = list(web_dir.glob("*.html"))
    
    if not html_files:
        print("No HTML files found in web directory")
        return
    
    print(f"Found {len(html_files)} HTML files to compile:")
    
    for html_file in html_files:
        # Generate output filename - use 'ui_html' for ui.html
        if html_file.stem == "ui":
            var_name = "ui_html"
        else:
            var_name = html_file.stem + "_html"
        output_file = src_dir / f"{var_name}.h"
        
        print(f"  Compiling {html_file.name} -> {output_file.name}")
        
        # Run HTML compiler
        try:
            result = subprocess.run([
                sys.executable, str(html_compiler),
                str(html_file),
                str(output_file),
                var_name
            ], capture_output=True, text=True)
            
            if result.returncode == 0:
                print(f"    ✓ {result.stdout.strip()}")
            else:
                print(f"    ✗ Error: {result.stderr.strip()}")
                
        except Exception as e:
            print(f"    ✗ Exception: {e}")

def main():
    """Main function"""
    print("HTML to C++ Compiler")
    print("===================")
    
    compile_html_files()
    
    print("HTML compilation complete!")

if __name__ == "__main__":
    main()
