# RISC-V emulator
---

#### Built with
C++ / opengl / glfw / glad / imgui

---
## Supported instructions
```
add(i) sub(i) xor(i) or(i) and(i) sll(i) srl(i) mul(i) div(i) rem(i)
lw lh lb sb sh sw li
beq bne blt ble bge bgt 
jump j  // both do the same thing
call ret
ecall
````
---
### ecall
| x10/a0 | action performed by ecall | 
| :---   |         :----:            |
| 1      |  print integer in x11/a1  |
| 11     | print character in x11/a1 |
| 10     |       exit code           | 
---

### registers
supports 32 registers(4 byte) with both names x1/sp ...

---

### pc and ra
**pc** doesn't do anything. the code is first checked for validity, while also storing defines for execution. defining a functions stores the character index in text so when the code goes to call or jump instruction tokenizer, goes to the character stored in a map, so it is the same as moving the pc to the desired instruction. **ra** also stores the character of the caller so calling **ret** moves the tokenizer right after the caller instruction

---

### white space
tokenizer analyzes the whole text and it ignores white space characters which are **space(' '), comma(',') and newLine('\n')**, but **comma(',')** starts a new **Token**. this means you cant use more than one comma in some places.

**Correct ways**
```
addi, sp, sp, -4
///
addi,  sp,   sp,   -4 , ,, ,
///
addi sp,
sp,   -4
```
*only one or zero comma(',') is in between the instructions and arguments*

**Inorrect way**
```
addi, ,sp, ,sp, -4
```
*more than one comma(',') is in between the instruction and argument so the tokenizer sees it as an empty argument and shows an error*

---

## Usage

> - Both methods only work on windows.
> - Sometimes the window might be too small and you might have to resize it on the first open.
---
**First method** (Needs Visual Studio)
1. **Download** the repository.
2. Open the solution and run the project.

**Second method**
1. **Download** the repository.
2. Open the "**App**" folder and run "**RISCV.exe**". (dont move the files, but you can make a shortcut).
---
