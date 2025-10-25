import re



def tokenize(code: str):
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
            else:
                tokens.append(("EXPR", part))
    return tokens