
#include <pruss_intc_mapping.h>
#include <prussdrv.h>

#include "pru-virtual-machine.h"

// TODO: For now we are implementing only the driver functions
// that we need in beagleg

// Initialize the virtual machine
int prussdrv_init(void) { return 0; }

int prussdrv_open(unsigned int host_interrupt) { return 0; }
int prussdrv_pruintc_init(const tpruss_intc_initdata *prussintc_init_data) {
  return 0;
}
int prussdrv_map_prumem(unsigned int pru_ram_id, void **address) { return 0; }
int prussdrv_pru_write_memory(unsigned int pru_ram_id,
                              unsigned int wordoffset,
                              const unsigned int *memarea,
                              unsigned int bytelength) { return 0; }
int prussdrv_pru_enable(unsigned int prunum) { return 0; }
unsigned int prussdrv_pru_wait_event(unsigned int host_interrupt) { return 0; }
int prussdrv_pru_clear_event(unsigned int host_interrupt,
                             unsigned int sysevent) { return 0; }
int prussdrv_pru_disable(unsigned int prunum) { return 0; }
int prussdrv_exit(void) { return 0; }
