from src.utils import CompileError, IR_REPR

class Compiler:
    def __init__(self):
        self.res = []
        self.toks = []
        self.labels = {}
        self.pos = 0

        self.s_label = {}
        self.macros = {} # dict[str, int]

    def reset(self, toks:list[tuple]):
        self.res.clear()
        self.labels.clear()
        self.toks = list(toks)
        self.pos = 0

    def peek(self):
        if self.pos < len(self.toks): return self.toks[self.pos]
        else: raise CompileError("Unexpected EOF")

    def next(self):
        tok = self.peek()
        self.pos += 1
        return tok

    def expect(self, tok):
        t = self.peek()
        if t[0] in tok: return
        else: raise CompileError(f"Expected {tok.lower()} but got {t[0].lower()}")

    def compile(self):
        self.s_label.clear()
        self.macros.clear()

        while self.pos < len(self.toks):
            self.parse_token()

        #print(self.res)

        for l, p in self.s_label.items():
            if l not in self.labels:
                raise CompileError(f"Unknown label: '{l}'")
            for pos in p:
                #print(self.res[pos:pos+5])
                self.res[pos:pos+4] = list(self.labels[l].to_bytes(4, "little"))

        print(f"Compiling finished (program size: {len(self.res)} bytes)")

    def parse_token(self):
        t, v = self.peek()

        print(f"[D:{self.pos}]: {t}({v}), labels:{self.labels}, insert:{self.labels}, macros:{self.macros}")

        match t:
            case "LABEL":
                if v in self.labels: raise CompileError(f"Cannot declare label '{v}' twice (second definition at {self.pos})")
                self.labels[v] = len(self.res)
                self.next()
            case "KEYWORD":
                self.handle_keyword()
            case "KEYWORD_MACRO":
                self.handle_macro_keyword()
            case _:
                raise CompileError(f"Unknown token {t} {v}")

    def handle_macro_keyword(self):
        _, v = self.next()
        match v:
            case "define":
                self.expect(("EXPR"))
                _, name = self.next()
                if name in self.macros: raise CompileError(f"Macro {name} defined twice")
                val = self.get_number()
                self.macros[name] = val
                return
            case _: raise CompileError(f"Unknown macro function {v}")

    def resolve_macro(self, m:str) -> int:
        if m not in self.macros: raise CompileError(f"Undefined macro: {m}")
        return self.macros[m]

    def get_number(self) -> int:
        self.expect({"NUM", "MACRO"})
        t, v = self.next()
        if t == "NUM": return v
        return self.resolve_macro(v)

    def handle_keyword(self):
        t, v = self.next()

        #print(t, v)

        global IR_REPR

        if v in IR_REPR:
            self.res.append(IR_REPR[v])
            match v:
                case "iconst" | "store" | "load":
                    v = self.get_number()
                    self.res += list(v.to_bytes(4, "little"))
                    return
                case "jmp" | "jz" | "jnz" | "call":
                    self.expect({"NUM", "LABEL"})
                    t, v = self.next()
                    if t == "NUM":
                        self.res += list(v.to_bytes(4, "little"))
                    else:
                        self.res += [0] * 4 # Some random value
                        pos = len(self.res) - 4
                        if v in self.s_label:
                            self.s_label[v].append(pos)
                        else:
                            self.s_label[v] = [pos]
                        return
                    return
                case _: return
        else:
            raise CompileError(f"Unknown keyword: {v}")

    def export(self) -> bytes:
        return bytes(self.res)