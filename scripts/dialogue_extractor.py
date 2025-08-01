import re
import sys

def extract_dialogue(text):
    cleaned = re.sub(r'@[^@]*@', '', text)
    lines = []
    for line in cleaned.split('\n'):
        line = line.strip()
        if line:
            lines.append(line)
    
    return lines

def main():
    if len(sys.argv) != 2:
        print("Usage: python dialogue_extractor.py input.strings.txt")
        return
    
    with open(sys.argv[1], 'r', encoding='utf-8') as f:
        content = f.read()
    
    dialogues = extract_dialogue(content)
    
    for dialogue in dialogues:
        print(dialogue)

if __name__ == "__main__":
    main()