#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  // TODO
  // PC: Program Counter, address of the currently executing instruction
  unsigned char PC;

  // FL: the flags register holds the current flags status. These flags can change based on the operands given to the CMP opcode.
  // The register is made up of 8 bits. If a particular bit is set, that flag is "true".

  // FL bits: 00000LGE
  // L Less-than: during a CMP, set to 1 if registerA is less than registerB, zero otherwise.
  // G Greater-than: during a CMP, set to 1 if registerA is greater than registerB, zero otherwise.
  // E Equal: during a CMP, set to 1 if registerA is equal to registerB, zero otherwise.  //
  unsigned char FL;

  // 8 general-purpose 8-bit numeric registers R0-R7.
  // R5 is reserved as the interrupt mask (IM); R6 is reserved as the interrupt status (IS); R7 is reserved as the stack pointer (SP)
  // These registers only hold values between 0-255. After performing math on registers in the emulator, bitwise-AND the result with 0xFF (255) to keep the register values in that range.
  unsigned char registers[8];

  // RAM (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op
{
  // Add more here
  ALU_MUL,
  ALU_ADD,
  ALU_CMP,
  // STRETCH
  ALU_AND,
  ALU_OR,
  ALU_XOR,
  ALU_NOT,
  ALU_SHL,
  ALU_SHR,
  ALU_MOD,

};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010 // LOAD IMMEDIATE
#define HLT 0b00000001 // HALT
#define PRN 0b01000111 // PRINT
// TODO: more instructions here. These can be used in cpu_run().

// ALU
#define MUL 0b10100010 // MULTIPLY
#define ADD 0b10100000 // ADD
#define CMP 0b10100111 // COMPARE
#define AND 0b10101000 // AND
#define OR 0b10101010  // OR
#define XOR 0b10101011 // EXCLUSIVE OR
#define NOT 0b01101001 // NOT
#define SHL 0b10101100 // SHIFT LEFT
#define SHR 0b10101101 // SHIFT RIGHT
#define MOD 0b10100100 // MODULO

// STACK
#define PUSH 0b01000101 // STACK PUSH
#define POP 0b01000110  // STACK POP

// SUBROUTINE RELATED
#define CALL 0b01010000 // CALL SUBROUTINE
#define RET 0b00010001  // RETURN FROM SUBROUTINE

// JUMP
#define JMP 0b01010100 // Jump to the address stored in the given register; set the PC to the address stored in the given register.
#define JEQ 0b01010101 // If equal flag is set (true), jump to the address stored in the given register.
#define JNE 0b01010110 // If E flag is clear (false, 0), jump to the address stored in the given register.

// Function declarations
extern void cpu_load(struct cpu *cpu, char *filepath); // filepath from argv[1]
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
