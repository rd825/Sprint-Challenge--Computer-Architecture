#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{

  // Check that filepath is inputted.
  if (argc < 2)
  {
    fprintf(stderr, "Error: no filepath provided.\n");
    exit(1);
  }

  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]); // accept filepath from command line
  cpu_run(&cpu);

  return 0;
}