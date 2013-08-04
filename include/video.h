#define VRAM_BASE (0x6000000)
#define SPRITEPAL (0x5000200)


#define SCANLINE_COUNTER ((volatile unsigned short*) 0x4000006)

void waitForVBlank(void);

