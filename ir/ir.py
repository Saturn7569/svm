import sys

from src.lex import tokenize

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <file>")
        return

    code = None
    try:
        with open(sys.argv[1], 'r') as f:
            code = f.read()
    except PermissionError:
        print(f"{sys.argv[1]}: Permission denied")
        return
    except FileNotFoundError:
        print(f"{sys.argv[1]}: No such file or directory")
        return
    except Exception as e:
        print(f"{sys.argv[1]}: {e}")
        return

    toks = tokenize(code)
    print(toks)

if __name__ == "__main__": main()