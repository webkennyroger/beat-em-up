#include "vblank.h"
#include <snes/snestypes.h>

extern u16 pad_keys[2];
extern u16 pad_keysold[2];

typedef struct {
    u8  *data;
    u16  vramDest;
    u16  size;
} SprEntry;

#define SPR_QUEUE_SIZE  32
#define MAX_PER_VBLANK  6

static SprEntry sSprQueue[SPR_QUEUE_SIZE];
static u8       sSprHead = 0;
static u8       sSprTail = 0;
static u8       sSprCount = 0;

void vblank_init(void) {
    sSprHead  = 0;
    sSprTail  = 0;
    sSprCount = 0;
}

void vblank_queue_sprite(u8 *data, u16 vramDest, u16 size) {
    u8 i;
    for (i = sSprHead; i != sSprTail; i = (i + 1) % SPR_QUEUE_SIZE) {
        if (sSprQueue[i].vramDest == vramDest) {
            sSprQueue[i].data = data;
            sSprQueue[i].size = size;
            return;
        }
    }
    if (sSprCount < SPR_QUEUE_SIZE) {
        sSprQueue[sSprTail].data     = data;
        sSprQueue[sSprTail].vramDest = vramDest;
        sSprQueue[sSprTail].size     = size;
        sSprTail = (sSprTail + 1) % SPR_QUEUE_SIZE;
        sSprCount++;
    }
}

void vblank_flush_sprite_queue(void) {
    u8 processed = 0;
    while (sSprCount > 0 && processed < MAX_PER_VBLANK) {
        dmaCopyVram(
            sSprQueue[sSprHead].data,
            sSprQueue[sSprHead].vramDest,
            sSprQueue[sSprHead].size
        );
        sSprHead = (sSprHead + 1) % SPR_QUEUE_SIZE;
        sSprCount--;
        processed++;
    }
}

void scanPads(void) {
    vuint16 *joyreg = (vuint16 *)0x4218;
    u8 i;
    for (i = 0; i < 2; i++) {
        u16 cur = joyreg[i];
        pad_keysold[i] = pad_keys[i];
        pad_keys[i]    = cur;
    }
}
