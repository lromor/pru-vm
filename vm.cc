
#include <iostream>
#include "pru-virtual-machine.h"

#define INSTRUCTION_RAM_SIZE 8192 // Bytes

static int usage(const char *prog) {
  fprintf(stderr, "Please pass a binary file (-b option in pasm).\n");
  fprintf(stderr, "Usage: %s [options] [<binary-filename>]\n", prog);
  return 1;
}

int main(int argc, char *argv[]) {

  if (argc < 2) return usage(argv[0]);

  std::cout << "PRU-ICSS Virtual machine" << std::endl;

  FILE *f = fopen(argv[1], "rb");

  if (f == NULL) {
    perror("Problem while opening file: ");
    return 1;
  }

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  rewind(f);

  if (fsize > INSTRUCTION_RAM_SIZE) {
    fprintf(stderr, "Error, binary file size greater than %d bytes.\n",
            INSTRUCTION_RAM_SIZE);
    return 1;
  }

  // Load the file in memory and check max size which is 8KB
  void *instructions = malloc(fsize);

  long result = fread(instructions, 1, fsize, f);
  if (result != fsize) {
    perror("Reading error: ");
    return 1;
  }

  std::cout << "Instructions total size: " << fsize << " bytes" << std::endl;
  std::cout << "Starting the vm..." << std::endl;

  PruVirtualMachine pru;
  pru.Load(instructions, fsize);
  pru.Run();

  while (pru.IsRunning()) {
    pru.Next();
  }

  free(instructions);

  return 0;
}
