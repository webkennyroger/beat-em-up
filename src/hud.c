#include "hud.h"
#include <snes.h>
#include <string.h>

u16 gHudBuffer[HUD_WORDS];
u8  gHudDirty = 0;

u8 gTimer      = 99;
u8 gTimerTicks = 0;

#define MAP_BASE    0x0000

#define NAME_POS    32

#define HP_START    40
#define HP_WIDTH    16
#define HP_FILLED   80
#define HP_EMPTY    64

#define TIMER_POS   58

#define DIGIT_TILES  72
#define LETTER_TILES 78
#define BLANK_TILE   2

static u8 sLastTimer = 255;

void hud_init(const char *name) {
    u16 i;

    for (i = 0; i < HUD_WORDS; i++)
        gHudBuffer[i] = MAP_BASE + BLANK_TILE;

    for (i = 0; i < 8; i++) {
        char c = name[i];
        if (c == 0) break;
        if (c >= 'A' && c <= 'Z')
            gHudBuffer[NAME_POS + i] = MAP_BASE + LETTER_TILES + (c - 'A');
    }

    gHudDirty = 1;
}

void hud_draw_hp(u8 hp, u8 maxHp) {
    // HP bar tiles not available in current tileset; shows static from map.
}

void hud_draw_timer(u8 seconds) {
    u8 tens, ones;

    if (seconds == sLastTimer) return;
    sLastTimer = seconds;

    tens = seconds / 10;
    ones = seconds % 10;

    gHudBuffer[TIMER_POS]     = MAP_BASE + DIGIT_TILES + tens;
    gHudBuffer[TIMER_POS + 1] = MAP_BASE + DIGIT_TILES + ones;

    gHudDirty = 1;
}

void hud_draw(void) {
    gTimerTicks++;

    if (gTimerTicks >= 60) {
        gTimerTicks = 0;
        if (gTimer > 0)
            gTimer--;
    }

    hud_draw_timer(gTimer);

    if (gHudDirty) {
        dmaCopyVram((u8*)gHudBuffer, 0x4000, HUD_WORDS * 2);
        gHudDirty = 0;
    }
}
