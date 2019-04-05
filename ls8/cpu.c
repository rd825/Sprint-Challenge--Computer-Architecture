#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Set the stack pointer to 7 (i.e. Register 7)
#define SP 7

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

// STACK PUSH
void stack_push(struct cpu *cpu, unsigned char value)
{
  // decrement stack pointer, starts at R7
  cpu->registers[SP]--;
  cpu_ram_write(cpu, cpu->registers[SP], value);
}

// STACK POP
unsigned char stack_pop(struct cpu *cpu)
{
  unsigned char popped = cpu->ram[cpu->registers[SP]];
  cpu->ram[cpu->registers[SP]++] = 0x00;
  return popped;
}

// CALL SUBROUTINE
void call(struct cpu *cpu, int next_inst)
{
  stack_push(cpu, cpu->PC + next_inst);
}

// RETURN FROM SUBROUTINE
unsigned char ret(struct cpu *cpu)
{
  return cpu->PC = stack_pop(cpu);
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

// filepath from argv[1]
void cpu_load(struct cpu *cpu, char *filepath)
{
  FILE *fp;
  int address = 0;
  char line[1024];

  fp = fopen(filepath, "r"); // read from file

  if (fp == NULL)
  {
    fprintf(stderr, "Couldn't read file: %s\n", filepath);
    return;
  }

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *endptr;
    unsigned char val;

    val = strtoul(line, &endptr, 2);

    if (endptr == line)
    {
      // skip this line
      continue;
    }
    cpu_ram_write(cpu, address++, val);
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB]; // MULTIPLY regA * regB
    break;

    // TODO: implement more ALU ops
  case ALU_ADD:
    cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB]; // ADD regA + regB
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO

    // 1. Get the value of the current instruction (in address PC).
    // IR: Instruction Register, contains a copy of the currently executing instruction
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA, operandB;
    int next_inst = 1;

    if (IR & 0x80) // bitwise AND for 2 operands
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1) & 0xFF;
      operandB = cpu_ram_read(cpu, cpu->PC + 2) & 0xFF;
      next_inst = 3;
    }
    else if (IR & 0x40) // bitwise AND for 1 operand
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1) & 0xFF;
      next_inst = 2;
    }

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    // Set the value of a register to an integer.
    case LDI:
      cpu->registers[operandA] = operandB;
      break;

    // Print to the console the decimal integer value that is stored in the given register.
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;

    // use the ALU to multiply OpA * OpB
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    // use the ALU to add OpA + OpB
    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;

    case PUSH:
      stack_push(cpu, cpu->registers[operandA]);
      break;

    case POP:
      cpu->registers[operandA] = stack_pop(cpu);
      break;

    case CALL:
      call(cpu, next_inst);
      cpu->PC = cpu->registers[operandA];
      continue;

    case RET:
      ret(cpu);
      continue;

    // Halt the CPU (and exit the emulator).
    case HLT:
      running = 0;
      break;

    default:
      printf("Unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->PC);
      running = 0;
      break;
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += next_inst;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;

  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));

  cpu->registers[SP] = 0xF4; // stack is empty
}
