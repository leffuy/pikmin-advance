#include "dma.h"

void dmaTransfer(void *destination, const void *source, unsigned int chunks, int channel, unsigned int mode) {
  DMA_MEMORY[channel].control = 0;
  DMA_MEMORY[channel].source = source;
  DMA_MEMORY[channel].destination = destination;
  DMA_MEMORY[channel].control = chunks | mode;
}
