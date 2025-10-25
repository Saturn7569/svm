class CompileError(Exception):
    def  __init__(self, msg:str):
        self.msg = msg

    def __str__(self):
        return f"Error while compiling:\n{self.msg}"

IR_REPR = {
    "nop": 0x00,
    "pop": 0x01,
    "dup": 0x02,

    "iconst": 0x10,

    "mvar": 0x20,
    "store": 0x21,
    "load": 0x22,

    "add": 0x30,
    "sub": 0x31,
    "mul": 0x32,
    "div": 0x33,
    "mod": 0x34,

    "jmp": 0x40,
    "jz": 0x41,
    "jnz": 0x42,
    "call": 0x43,
    "ret": 0x44,

    "eq": 0x50,
    "neq": 0x51,
    "lt": 0x52,
    "gt": 0x53,
    "gte": 0x54,
    "lte": 0x55,

    "hlt": 0x60,

    "dprint": 0xF0,
    "dchar": 0xF1,
}