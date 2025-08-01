import re
import sys

def sanitize_spaces(text):
    text = re.sub(r'FFFFFFFF@', 'FFFFFFFF@  ', text)
    return text

def extract_dialogue(text):
    text = sanitize_spaces(text)
    cleaned = re.sub(r'@[^@]*@', '',  text)
    cleaned = re.sub(r'  ', '\n', cleaned)
    lines = []
    for line in cleaned.split('\n'):
        line = line.strip()
        if line:
            lines.append(line)
    
    return lines

def main():
    if len(sys.argv) != 2:
        print(sys.argv)
        print("Usage: python dialogue_extractor.py input.strings.txt")
        return
    
    with open(sys.argv[1], 'r', encoding='utf-8') as f:
        content = f.read()
    
    dialogues = extract_dialogue(content)
    
    for dialogue in dialogues:
        print(dialogue)

if __name__ == "__main__":
    main()