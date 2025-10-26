import sys

from src.utils import CompileError
from src.lex import tokenize
from src.compiler import Compiler

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

    try:
        print("Tokenizing...")
        toks = tokenize(code)
        print(toks)
        print("Compiling...")
        c = Compiler()
        c.reset(toks)
        c.compile()
        print(c.res)
    except CompileError as e:
        print(e)

if __name__ == "__main__": main()