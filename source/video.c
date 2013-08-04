#include "video.h"

	
void waitForVBlank() {
   while (*SCANLINE_COUNTER > 160);
   while (*SCANLINE_COUNTER < 160);
}
