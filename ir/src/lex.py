import re

KEYWORDS = {
    "nop", "pop", "dup",
    "iconst",
    "mvar", "store", "load",
    "add", "sub", "mul", "div", "mod",
    "jmp", "jz", "jnz", "call", "ret",
    "eq", "neq", "lt", "gt", "gte", "lte",
    "hlt",

    "dprint", "dchar",
}

def tokenize(code: str):
    global KEYWORDS
    tokens = []
    for line in code.splitlines():
        line = line.split(';', 1)[0].strip()
        if not line:
            continue

        parts = line.split()
        for part in parts:
            if part.startswith("::"):
                tokens.append(("LABEL", part[2:]))
            elif part.startswith("0x"):
                tokens.append(("NUM", int(part, 16)))
            elif part.startswith("0b"):
                tokens.append(("NUM", int(part, 2)))
            elif re.match(r"^'.'$", part):
                tokens.append(("NUM", ord(part[1])))
            elif part.isdigit():
                tokens.append(("NUM", int(part)))
            elif part.lower() in KEYWORDS:
                tokens.append(("KEYWORD", part.lower()))
            else:
                tokens.append(("EXPR", part))
    return tokens