#!/usr/bin/env python3
import re
import sys
import os

def extract_data_fields(content):
    """Extract all non-empty Data: fields from the strings.txt file"""
    data_fields = []
    
    # find all "Data: "
    lines = content.split('\n')
    
    for line in lines:
        line = line.strip()
        if line.startswith('Data: ') and not line.endswith('Data is empty'):
            data_content = line[6:]  # remove prefix
            if data_content:  # only add non-empty data
                data_fields.append(data_content)
    
    return data_fields

def main():
    if len(sys.argv) != 2:
        print("Usage: python extract_data.py input.strings.txt")
        print("This will create input.strings.txt.data.txt with only the Data fields")
        return 1
    
    input_file = sys.argv[1]
    
    if not os.path.exists(input_file):
        print(f"Error: File {input_file} not found")
        return 1
    
    # trying with different encodings (avoid bugs lol)
    content = None
    for encoding in ['utf-8', 'latin-1', 'cp1252']:
        try:
            with open(input_file, 'r', encoding=encoding) as f:
                content = f.read()
            print(f"Successfully read file using {encoding} encoding")
            break
        except UnicodeDecodeError:
            continue
        except Exception as e:
            print(f"Error reading {input_file}: {e}")
            return 1
    
    if content is None:
        print(f"Could not read {input_file} with any supported encoding")
        return 1
    
    data_fields = extract_data_fields(content)
    
    if not data_fields:
        print(f"No data fields found in {input_file}")
        return 0
    
    output_file = input_file + ".data.txt"
    try:
        with open(output_file, 'w', encoding='utf-8') as f:
            for data in data_fields:
                f.write(data + '\n')
        
        print(f"Extracted {len(data_fields)} data fields to {output_file}")
        return 0
        
    except Exception as e:
        print(f"Error writing {output_file}: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
