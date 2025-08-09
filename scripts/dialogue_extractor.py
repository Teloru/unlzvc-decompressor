import re
import sys

# Mapping of post-demo commands (binary format) based on iff.h
TEXTBOX_COMMANDS = {
    0x01: "UNKNOWN",
    0x02: "COLOR", 
    0x03: "REDBUTTON",
    0x04: "GREENBUTTON", 
    0x05: "YELLOWBUTTON",
    0x06: "BLUEBUTTON",
    0x07: "NEWLINE",
    0x08: "WINDOWWIDTH",
    0x09: "WINDOWHEIGHT", 
    0x0A: "FONTHEIGHT",
    0x0B: "BORDERSIZE",
    0x0C: "MODELSCALE",
    0x0D: "RESOURCEOFFSETY",
    0x0E: "RESOURCEOFFSETX",
    0x0F: "NEWPARAGRAPH",
    0x10: "USEBUTTONS",
    0x11: "USEBUTTONSNOHELP",
    0x12: "WINDOWPOSITIONX",
    0x13: "WINDOWPOSITIONY",
    0x14: "PAUSEGAME",
    0x15: "CLEARWINDOW",
    0x16: "SCROLLING",
    0x17: "RESOURCE",
    0x18: "MODEL",
    0x19: "JUSTIFY",
    0x1A: "BORDER",
    0x1B: "CORNER",
    0x1C: "TITLE",
    0x1D: "TITLECURVATURE",
    0x1E: "TITLESCALE",
    0x1F: "TITLEOFFSETY",
    0x20: "ANIM",
    0x21: "CSRY",
    0x22: "WINDOWLIFETIME",
    0x23: "WINDOWLIFEMATCHAUDIO",
    0x24: "OPENRATE",
    0x25: "FADEIN",
    0x26: "SCALEIN",
    0x27: "APPEARANCECOUNT",
    0x28: "GLASS",
    0x29: "AUTOFADE",
    0x2A: "MORE",
    0x2B: "PAUSE",
    0x2C: "MOIGLEICON",
    0x2D: "PRIORITY",
    0x2E: "SPEECH",
    0x2F: "DIALOGRANDOM",
    0x30: "DIALOGLIST",
    0x31: "DIALOGSECTION",
    0x32: "DIALOGSEQUENCE",
    0x33: "WAIT",
    0x34: "IGNOREWAIT",
}

def parse_command_block(block):
    """Parse a command block @...@ and return (command, parameter)"""
    if not block.startswith('@') or not block.endswith('@'):
        return None, None
    
    content = block[1:-1]  # remove the @
    
    # demo format: @COLOR:FF40FF40@
    if ':' in content:
        parts = content.split(':', 1)
        command = parts[0]
        param = parts[1] if len(parts) > 1 else ""
        return command, param
    
    # post-demo format: @\x02FF40FF40@
    elif len(content) > 0 and ord(content[0]) < 32:  # control character
        cmd_byte = ord(content[0])
        param = content[1:] if len(content) > 1 else ""
        command = TEXTBOX_COMMANDS.get(cmd_byte, f"UNK_{cmd_byte:02X}")
        return command, param
    
    # unknown format - maybe simple text?
    return "UNKNOWN", content

def split_dialogue_by_speakers(line):
    """Split a line into multiple dialogues when speakers change after CLEARWINDOW"""
    
    if '@CLEARWINDOW@' not in line or '@RESOURCE:\\ACTORS\\' not in line:
        # no speaker changes -> process normally
        return [extract_dialogue_from_string(line)]
    
    # split by CLEARWINDOW to get dialogue segments
    segments = line.split('@CLEARWINDOW@')
    dialogues = []
    
    for segment in segments:
        segment = segment.strip()
        if segment:
            extracted = extract_dialogue_from_string(segment)
            if extracted:
                dialogues.append(extracted)
    
    return [d for d in dialogues if d]  # filter out None results

def extract_dialogue_from_string(line):
    """Extract dialogue from a string by handling commands"""
    
    if not line.strip():
        return None
    
    # clean multiple alignment spaces
    line = re.sub(r'\s+', ' ', line.strip())
    
    result = {
        'dialogue': '',
        'speaker': '',
        'title': '',
        'buttons': [],
        'formatting': []
    }
    
    # if no commands (@), it's a simple string
    if '@' not in line:
        result['dialogue'] = line
        return result

    # parse all command blocks
    current_pos = 0
    dialogue_parts = []
    
    while current_pos < len(line):
        # look for the next @
        at_pos = line.find('@', current_pos)
        
        if at_pos == -1:
            # no more commands, add the rest as dialogue
            remaining = line[current_pos:].strip()
            if remaining:
                dialogue_parts.append(remaining)
            break
        
        # add text before the command
        if at_pos > current_pos:
            text_before = line[current_pos:at_pos].strip()
            if text_before:
                dialogue_parts.append(text_before)
        
        # look for the end of the command
        end_pos = line.find('@', at_pos + 1)
        if end_pos == -1:
            # malformed command, add @ and continue
            dialogue_parts.append('@')
            current_pos = at_pos + 1
            continue
        
        # extract and parse the command
        command_block = line[at_pos:end_pos + 1]
        command, param = parse_command_block(command_block)
        
        # process according to command type
        if command == "TITLE":
            result['title'] = param
        elif command == "RESOURCE" and param and "ACTORS" in param.upper():
            # Extract speaker name from RESOURCE:\ACTORS\HOIGLE
            speaker_match = re.search(r'ACTORS\\([A-Z]+)', param.upper())
            if speaker_match:
                result['speaker'] = speaker_match.group(1)
        elif command in ["REDBUTTON", "GREENBUTTON", "YELLOWBUTTON", "BLUEBUTTON"]:
            # Extract button name and its parameter, integrate directly in dialogue
            button_color = command.replace('BUTTON', '')
            dialogue_parts.append(f"[{button_color}: {param}]")
        elif command == "NEWLINE":
            dialogue_parts.append("\n")
        elif command == "NEWPARAGRAPH":
            dialogue_parts.append("\n\n")
        elif command == "CLEARWINDOW":
            # Skip CLEARWINDOW since we use it as separator now
            pass
        # Ignore UI/formatting commands completely
        elif command in ["COLOR", "PAUSE", "USEBUTTONS", "USEBUTTONSNOHELP", "PAUSEGAME", 
                         "SCROLLING", "BORDER", "OPEN", "CLOSE", "SCALEIN", "SCALEOUT",
                         "FADEIN", "FADEOUT", "GLASS", "AUTOFADE", "PRIORITY", "WAIT", 
                         "IGNOREWAIT", "WINDOWLIFEMATCHAUDIO", "OPENRATE", "APPEARANCECOUNT",
                         "MORE", "MOIGLEICON", "SPEECH", "DIALOGRANDOM", "DIALOGLIST",
                         "DIALOGSECTION", "DIALOGSEQUENCE"]:
            pass
        elif command == "UNKNOWN" and param:
            # For unknown commands, keep the parameter if it's text
            if param and any(c.isalpha() for c in param):
                dialogue_parts.append(param)
        
        current_pos = end_pos + 1
    
    # assemble the final dialogue
    result['dialogue'] = ' '.join(dialogue_parts).strip()
    
    # clean color codes that might remain (FFFFFFFF format)
    result['dialogue'] = re.sub(r'\b[A-F0-9]{8}\b', '', result['dialogue'])
    result['dialogue'] = re.sub(r'\b[A-F0-9]{6}\b', '', result['dialogue'])
    
    # clean sound file references (.ss)
    result['dialogue'] = re.sub(r'\.\w+\.ss', '', result['dialogue'])
    
    # clean technical codes at the beginning (numbers followed by characters)
    result['dialogue'] = re.sub(r'^\d+\s+\w+\s*', '', result['dialogue'])
    
    # clean technical references like \ACTORS\INTERFACE
    result['dialogue'] = re.sub(r'\\ACTORS\\[A-Z]+', '', result['dialogue'])
    
    # clean multiple newlines
    result['dialogue'] = re.sub(r'\\n\\n+', '\\n\\n', result['dialogue'])
    result['dialogue'] = re.sub(r'\\n+$', '', result['dialogue'])  # Remove trailing \n
    
    # clean multiple spaces and isolated @
    result['dialogue'] = re.sub(r'\s+', ' ', result['dialogue']).strip()
    result['dialogue'] = re.sub(r'@+', '', result['dialogue']).strip()
    result['dialogue'] = re.sub(r'#+', '', result['dialogue']).strip()
    
    return result if result['dialogue'] or result['title'] or result['buttons'] else None

def clean_dialogue_text(text):
    """Clean dialogue text from formatting artifacts"""
    if not text:
        return text
    
    # Remove common formatting artifacts
    text = re.sub(r'\bNEWPARAGRAPH\b', '\n\n', text)
    text = re.sub(r'\bNEWLINE\b', '\n', text)
    text = re.sub(r'\bCLEARWINDOW\b', ' | ', text)
    text = re.sub(r'\bUSEBUTTONS\b', '', text)
    text = re.sub(r'\bUSEBUTTONSNOHELP\b', '', text)
    text = re.sub(r'\bPAUSEGAME\b', '', text)
    text = re.sub(r'\bSCROLLING\b', '', text)
    text = re.sub(r'\bBORDER\b', '', text)
    text = re.sub(r'\bOPEN:\w+\b', '', text)
    text = re.sub(r'\bCLOSE:\w+\b', '', text)
    text = re.sub(r'\bSCALEIN\b', '', text)
    text = re.sub(r'\bSCALEOUT\b', '', text)
    
    # remove audio file references (.ss files)
    text = re.sub(r'\.[a-zA-Z0-9_-]+\.ss\b', '', text)  # Catches all .xxx.ss patterns
    
    # Remove emotional modifiers at the start of sentences
    text = re.sub(r',MODE\s*', '', text)
    text = re.sub(r',HAPPY\s*', '', text)
    text = re.sub(r',SAD\s*', '', text)
    text = re.sub(r',ANGRY\s*', '', text)
    
    # Clean multiple spaces and line breaks
    text = re.sub(r'\s+', ' ', text)
    text = re.sub(r'\n\s*\n', '\n\n', text)
    
    return text.strip()

def format_output(dialogue_data):
    """Format extracted data for output"""
    lines = []
    
    # Add title if it exists
    if dialogue_data['title']:
        clean_title = clean_dialogue_text(dialogue_data['title'])
        lines.append(f"[TITLE: {clean_title}]")
    
    # Add speaker if it exists
    if dialogue_data['speaker']:
        lines.append(f"[{dialogue_data['speaker']}]")
    
    # Skip separate button handling since they're now integrated in dialogue
    
    # Add main dialogue
    if dialogue_data['dialogue']:
        clean_dialogue = clean_dialogue_text(dialogue_data['dialogue'])
        if clean_dialogue:  # Only add if there's actual content after cleaning
            lines.append(clean_dialogue)
    
    result = ' '.join(lines)
    # Final cleanup of the entire line
    result = clean_dialogue_text(result)
    return result

def extract_dialogue(text):
    """Extract all dialogue lines from the text"""
    lines = text.split('\n')
    dialogues = []
    
    for line in lines:
        line = line.strip()
        if not line:
            continue
            
        # Ignore metadata lines and other non-dialogue elements
        if (line.startswith('String ') or line.startswith('Hash:') or 
            line.startswith('===') or line.startswith('---') or
            line.startswith('Language:') or line.startswith('Strings count:') or
            line.startswith('File offset:') or line == 'Data is empty' or
            line.startswith('#') or line.startswith('Generated by') or
            line.startswith('Extracted from')):
            continue
        
        # Ignore sound files (.ss)
        if line.endswith('.ss') or re.match(r'^\.\w+\.ss$', line):
            continue
            
        # Ignore lines too short that are probably not dialogue
        if len(line) < 4:
            continue
            
        # Ignore lines that only contain technical codes/references
        if re.match(r'^[A-Z0-9_]+$', line) and len(line) < 15:
            continue
            
        # Process Data: lines
        if line.startswith('Data:'):
            data_content = line[5:].strip()  # Remove "Data:"
            if data_content and data_content != "Data is empty":
                # handle speaker changes
                extracted_list = split_dialogue_by_speakers(data_content)
                for extracted in extracted_list:
                    if extracted:
                        formatted = format_output(extracted)
                        if formatted:
                            dialogues.append(formatted)
        else:
            # process other lines normally
            extracted_list = split_dialogue_by_speakers(line)
            for extracted in extracted_list:
                if extracted:
                    formatted = format_output(extracted)
                    if formatted:
                        dialogues.append(formatted)
    
    return dialogues

def main():
    if len(sys.argv) != 2:
        print("Usage: python dialogue_extractor.py input.strings.txt.data.txt")
        return
    
    input_file = sys.argv[1]
    output_file = "floigan_bros_all_dialogues.txt"
    
    content = None
    for encoding in ['utf-8', 'latin-1', 'cp1252']:
        try:
            with open(input_file, 'r', encoding=encoding) as f:
                content = f.read()
            print(f"Successfully read {input_file} using {encoding} encoding")
            break
        except UnicodeDecodeError:
            continue
        except FileNotFoundError:
            print(f"Error: File {input_file} not found")
            return
    
    if content is None:
        print("Error: Could not read file with any supported encoding")
        return
    
    dialogues = extract_dialogue(content)
    
    try:
        with open(output_file, 'w', encoding='utf-8') as f:
            for dialogue in dialogues:
                f.write(dialogue + '\n')
        
        print(f"Extracted {len(dialogues)} dialogues to {output_file}")
        
    except Exception as e:
        print(f"Error writing to {output_file}: {e}")
        return

if __name__ == "__main__":
    main()