#ifndef ENTITY_H
#define ENTITY_H

#include "types.h"

#define CANVAS_H   96
#define TILE_BUF_SIZE  3072

extern Character  gCharacters[MAX_CHARS];
extern Character *gPlayer;
extern Character *gYOrder[MAX_CHARS];
extern int gFrames;

void char_init(Character *c, u16 tileSize, u16 paletteSize);
void char_load_gfx(Character *c, u8 oamAddr, u16 vramAddr, u16 vramOffset, u8 paletteSlot);
void char_update_gfx(Character *c);
void char_update_pos(Character *c);
void char_update_hurt(Character *c);
void char_sort_y_order(void);
void char_swap_oam(Character *p1, Character *p2);
void char_draw(Character *c);

#endif
