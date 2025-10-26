from src.utils import CompileError, IR_REPR

class Compiler:
    def __init__(self):
        self.res = []
        self.toks = []
        self.labels = {}
        self.pos = 0

        self.s_label = {}

    def reset(self, toks:list[tuple]):
        self.res.clear()
        self.labels.clear()
        self.toks = list(toks)
        self.pos = 0

    def peek(self):
        return self.toks[self.pos] if self.pos < len(self.toks) else None

    def next(self):
        tok = self.peek()
        self.pos += 1
        return tok

    def expect(self, tok):
        t = self.peek()
        if t:
            if t[0] in tok: return
            else: raise CompileError(f"Expected {tok.lower()} but got {t[0].lower()}")
        else:
            raise CompileError("Unexpected EOF")

    def compile(self):
        self.s_label.clear()

        while self.pos < len(self.toks):
            t = self.next()
            if not t: raise CompileError("Unexpected EOF")
            t, v = t
            match t:
                case "LABEL":
                    if v in self.labels: raise CompileError(f"Cannot declare label '{v}' twice")
                    self.labels[v] = len(self.res) - 1
                case "KEYWORD":
                    self.handle_keyword()

    def handle_keyword(self):
        t = self.peek()
        t, v = t

        global IR_REPR

        if v in IR_REPR:
            self.res.append(IR_REPR[v])
            self.next()
            match v:
                case "iconst":
                    self.expect(("NUM"))
                    _, v = self.next()
                    self.res += list(v.to_bytes(4, "little"))
                    return
                case "jmp" | "jz" | "jnz":
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