# one0

<p align="center"><i>🔢 A minimalist esoteric programming language where every operation is represented by binary strings.</i>
    <br>
    <img alt="GitHub License" src="https://img.shields.io/github/license/ashkanfeyzollahi/one0">
</p>

**one0** is a Turing-complete, delimiter-based esoteric programming language that uses binary strings for all opcodes and arguments.  
With a tape-based memory model similar to Brainfuck and direct binary encoding, one0 provides a unique challenge for programmers interested in low-level computation and minimalist design.

Built using **pure C**, **argp** for argument parsing, and a custom garbage collection system, this interpreter is ideal for exploring language design, understanding memory management, or experimenting with esoteric programming paradigms.

---

## Features

* Binary-based instruction set (all opcodes are 8-bit binary strings)
* Tape-based memory model with pointer operations
* Built-in control flow (conditional and unconditional jumps)
* Character and string I/O operations
* Custom error handling via `panic` system
* Automatic garbage collection for interpreter memory
* Delimiter-based tokenization (spaces and newlines)
* Configurable memory limits
* Zero dependencies beyond standard C libraries

---

## Quick Reference

**Default source file:** `main.o0`  
**Command-line usage:** `./one0 -i <filename>`

After building the interpreter, create a `.o0` file and run:

```bash
./one0 -i hello.o0
```

---

## Language Basics

### Character Encoding
- **`1`** represents binary 1
- **`0` or any other character** represents binary 0
- **Whitespace and newlines** serve as delimiters

### Instruction Format
```
<opcode> <arg1> <arg2> ... <argN>
```

Each line contains an 8-bit binary opcode followed by zero or more 8-bit binary arguments.

### Function Table

| Opcode | Binary | Function | Description |
|--------|--------|----------|-------------|
| 0 | `00000000` | `print` | Print the arguments as a string |
| 1 | `00000001` | `printmem` | Print entire memory buffer |
| 2 | `00000010` | `printmembyte` | Print byte at current memory pointer |
| 3 | `00000011` | `moveptrr` | Move memory pointer right |
| 4 | `00000100` | `moveptrl` | Move memory pointer left |
| 5 | `00000101` | `incrementptrbyte` | Increment byte at pointer |
| 6 | `00000110` | `decrementptrbyte` | Decrement byte at pointer |
| 7 | `00000111` | `inputch` | Read one character to memory |
| 8 | `00001000` | `inputs` | Read string to memory |
| 9 | `00001001` | `gotoline` | Unconditional jump to line |
| 10 | `00001010` | `gotolineif` | Jump if current byte = args[0] |
| 11 | `00001011` | `gotolineifn` | Jump if current byte ≠ args[0] |

---

## Running the Project

### Requirements

* **GCC** or compatible C compiler
* **argp library** (usually included in glibc)
* Standard C libraries
* **Make**

### Steps to Run

1. Clone the repository:
```bash
git clone https://github.com/ashkanfeyzollahi/one0
```

2. Navigate to the project folder:
```bash
cd one0
```

3. Compile the interpreter:
```bash
make
```

4. Create a simple program (e.g., `hello.o0`):
```
00000000 01001000 01100101 01101100 01101100 01101111
```

5. Run the program:
```bash
./one0 -i hello.o0
```

6. View output:
```
Hello
```

---

## How It Works (Technical Breakdown)

* Parses command-line arguments using **argp** to get the source file path.
* Reads the entire source file into memory using `fgeteof()`.
* Allocates program memory buffer of size `PROGMEMLIMIT`.
* Tokenizes source code by lines, then by spaces/whitespace.
* Converts binary strings to bytes using `bin_to_ch()`:
  * Each 8-bit binary string becomes a single byte
  * First token is the opcode (function index)
  * Remaining tokens are arguments
* Looks up the function in `func_ptrs[]` array and executes it.
* Functions can manipulate memory, print output, or change control flow.
* Garbage collector tracks all `xmalloc` allocations via `gc_list`.
* On exit or panic, `collect_garbages()` frees all tracked memory.
* Control flow managed via `goto_` variable for jump instructions:
```c
if (progmem[progmem_curpos] != 0)
    goto_ = line;
```

---

## Examples

### Example 1: Print "Hi"
```
00000000 01001000 01101001
```
Prints the ASCII characters 'H' (72) and 'i' (105).

### Example 2: Memory Operations
```
00000101
00000101
00000101
00000010
```
Increments memory cell three times, then prints the byte as a character (ASCII value 3, a control character).

---

## Configuration

Edit `include/config.h` to customize interpreter settings:

```c
#define PROGMEMLIMIT 30000  // Memory buffer size in bytes
```

This defines the tape memory available to one0 programs.

---

## Error Handling

The interpreter includes a custom error system:

* **`panic()`** function handles all fatal errors
* Automatically runs garbage collection before exit
* Returns appropriate error codes
* Provides descriptive error messages:
  * "Binary numbers must not be longer than 8 characters"
  * "Function index exceeds function table size"
  * "Line doesn't exist"

Additionally, functions can set `cerrno` for custom error codes, which are checked after each function execution.

---

## Architecture Overview

### Core Components

* **Parser** (`eval_prog`, `eval_line`) – Interprets binary instructions
* **Tokenizer** (`strtoks`) – Splits input by delimiters
* **Binary Converter** (`bin_to_ch`) – Converts binary strings to bytes
* **Memory Manager** (`xmalloc`, `xrealloc`, `xfree`) – Wraps stdlib with GC
* **Garbage Collector** (`gc_list`) – Tracks and frees all allocations
* **Function Table** (`func_ptrs[]`) – O(1) function dispatch
* **Program Memory** (`progmem`) – Tape-based memory buffer

### Memory Safety

All allocations use custom wrappers:
* `xmalloc()` – Allocates and registers with GC
* `xrealloc()` – Reallocates and updates GC tracking
* `xfree()` – Frees and unregisters from GC
* `xngcrealloc()` – Reallocates without GC (for GC internal use)

On any error, `panic()` calls `collect_garbages()` to prevent memory leaks.

---

## Acknowledgements

Thanks to:

* **Brainfuck** and **Whitespace** for inspiration in esoteric language design
* **argp** for clean command-line argument parsing
* **GCC** for providing a robust C compilation environment

---

## Bug Reports

Found an issue or have a suggestion?  
[Open an issue](https://github.com/ashkanfeyzollahi/one0/issues) on GitHub.
