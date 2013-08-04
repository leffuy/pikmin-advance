typedef struct {
    char* data;
    int length;
    int start;
    int end;
} SOUNDCLIP;

// Defines

// Registers for controlling sound hardware
#define REG_SND_DSCNT  (* ((unsigned short *) 0x04000082))
#define REG_SND_STAT   (* ((unsigned short *) 0x04000084))

// Master sound enable, SET THIS BEFORE ANYTHING ELSE!!!
#define SND_STAT_ON (1 << 0x7)
#define SND_STAT_OFF (0 << 0x7)

// Direct sound channel volume ratios that go into REG_SND_DSCNT
#define SND_DSCNT_A50 (0 << 0x2)
#define SND_DSCNT_A100 (1 << 0x2)
#define SND_DSCNT_B50 (0 << 0x3)
#define SND_DSCNT_B100 (1 << 0x3)

// Enables A channel on right and/or left speaker
#define SND_DSCNT_AR (1 << 0x8)
#define SND_DSCNT_AL (1 << 0x9)

// Sets channel A to use timer 0 or timer 1
#define SND_DSCNT_ATMR0 (0 << 0xA)
#define SND_DSCNT_ATMR1 (1 << 0xA)

// FIFO buffer will be reset by DMA when it is used
#define SND_DSCNT_ARESET (1 << 0xB)

// Enables B channel on right and/or left speaker
#define SND_DSCNT_BR (1 << 0xC)
#define SND_DSCNT_BL (1 << 0xD)

// Sets channel B to use timer 0 or timer 1
#define SND_DSCNT_BTMR0 (0 << 0xE)
#define SND_DSCNT_BTMR1 (1 << 0xE)

// FIFO buffer will be reset by DMA when it is used
#define SND_DSCNT_BRESET (1 << 0xF)

// Addresses for FIFO registers
#define REG_FIFO_A ((unsigned short *) 0x040000A0)
#define REG_FIFO_B ((unsigned short *) 0x040000A4)

