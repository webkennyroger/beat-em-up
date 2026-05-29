#include "entity.h"
#include "anim.h"
#include "vblank.h"
#include <string.h>

Character  gCharacters[MAX_CHARS];
Character *gPlayer      = NULL;
Character *gYOrder[MAX_CHARS];
int gFrames = 0;

void char_init(Character *c, u16 tileSize, u16 paletteSize)
{
    int i;

    c->tileSize     = tileSize;
    c->paletteSize  = paletteSize;
    c->state        = STATE_IDLE;
    c->subState     = 0;
    c->prevState    = 255;
    c->prevSubState = 255;
    c->frameCounter = 0;
    c->x = c->y = 0;
    c->z = 0;
    c->a = 0;
    c->y            = GROUND_Y;
    c->groundY      = GROUND_Y;
    c->velX = c->velY = c->velZ = 0;
    c->velA = 0;
    c->speed        = 0;
    c->speedTimer   = 0;
    c->hflip        = 0;
    c->alive        = true;
    c->visible      = true;
    c->priority     = 2;
    c->isInvincible = false;
    c->isHurt       = false;
    c->hurtTimer    = 0;
    c->hurtDuration = 20;
    c->hurtShake    = 0;

    c->didHit = 0;
    c->attackId = 10;
    c->lastHitId = 0;
    c->reactionTimer = 0;
    c->comboQueued = 0;
    c->comboStep = 0;
    c->knockdown = 0;
    c->isRunning = 0;
    c->lastTapButton = 0;
    c->tapTimer = 0;
    c->current_buttons = 0;
    c->previous_buttons = 0;
    c->inputHead = 0;
    c->entityIndex = 0;
    c->entityType = TYPE_NONE;
    c->stats = NULL;
    c->walkSpeed = 0;
    c->runSpeed = 0;
    c->attackPower = 1;
    c->specialCost = 0;
    c->aggression = 50;
    c->hostileMask = 0;
    c->candamageMask = 0;
    c->aiMoveType = AI_MOVE_CHASE;

    for (i = 0; i < MAX_CHARS; i++)
    {
        if (gYOrder[i] == NULL) { gYOrder[i] = c; break; }
    }
}

void char_load_gfx(Character *c, u8 oamAddr, u16 vramAddr,
                   u16 vramOffset, u8 paletteSlot)
{
    c->oamAddress  = oamAddr;
    c->vramAddress = vramAddr;
    c->vramOffset  = vramOffset;
    c->paletteSlot = paletteSlot;

    WaitForVBlank();

    dmaCopyVram(c->frames[0].tiles, vramAddr, c->tileSize);

    u8 palEntry = 128 + paletteSlot * 16;
    dmaCopyCGram(c->frames[0].palette, palEntry, c->paletteSize);

    oamSet(oamAddr, 0, 0, c->priority, 0, 0, vramOffset, paletteSlot);
    oamSetEx(oamAddr, OBJ_SMALL, OBJ_SHOW);
}

void char_update_gfx(Character *c)
{
    if (!c->alive || !c->visible) return;

    if (c->subState != c->prevSubState)
    {
        c->prevSubState = c->subState;
        c->prevState    = c->state;
        c->frameCounter = 0;
        vblank_queue_sprite(
            c->frames[c->subState].tiles,
            c->vramAddress,
            c->tileSize
        );
    }
}

static void _oam_set_xy_ex(u8 id, int x, u8 y)
{
    oamSetXY(id, x, y);

    u8  oam_id    = (u8)(id >> 2);
    int bit_shift = (oam_id & 3) << 1;
    int hi_pos    = (oam_id >> 4) + 128;
    int mask      = 1 << bit_shift;
    oamMemory[hi_pos] &= ~mask;
    oamMemory[hi_pos] |= ((x >> 8) & 1) << bit_shift;
}

void char_update_pos(Character *c)
{
    c->oldX = c->x;
    c->oldY = c->y;

    if (c->velX > 0) c->hflip = 0;
    if (c->velX < 0) c->hflip = 1;

    int displayY = c->y + c->z - c->a;
    if (displayY < SCREEN_MIN_Y) displayY = SCREEN_MIN_Y;
    if (displayY > SCREEN_MAX_Y + MAX_Z) displayY = SCREEN_MAX_Y + MAX_Z;

    oamFlip(c->oamAddress, c->hflip, 0);
    _oam_set_xy_ex(c->oamAddress, c->x, (u8)(displayY - (CANVAS_H - 1)));
}

void char_update_hurt(Character *c)
{
    if (!c->isHurt) return;

    if (c->hurtTimer > 0)
    {
        c->hurtTimer--;
        if ((c->hurtTimer & 4) == 0)
        {
            c->x += (c->hurtShake == 0) ? -2 : 2;
            c->hurtShake ^= 1;
        }
    }
    else
    {
        c->isHurt       = false;
        c->isInvincible = false;
        if (c->alive)
            anim_set_state(c, STATE_IDLE);
    }
}

void char_sort_y_order(void)
{
    int i;
    for (i = 0; i < MAX_CHARS - 1; i++)
    {
        if (gYOrder[i] && gYOrder[i+1])
        {
            int depthA = gYOrder[i]->y + gYOrder[i]->z;
            int depthB = gYOrder[i+1]->y + gYOrder[i+1]->z;
            if (depthB > depthA)
            {
                Character *tmp = gYOrder[i];
                gYOrder[i]     = gYOrder[i+1];
                gYOrder[i+1]   = tmp;
                char_swap_oam(gYOrder[i], gYOrder[i+1]);
            }
        }
        else if (!gYOrder[i] && gYOrder[i+1])
        {
            gYOrder[i] = gYOrder[i+1];
        }
    }
}

void char_swap_oam(Character *p1, Character *p2)
{
    u8 a1 = p1->oamAddress;
    u8 a2 = p2->oamAddress;
    u8 tmp[4];
    memcpy(tmp,            &oamMemory[a1], 4);
    memcpy(&oamMemory[a1], &oamMemory[a2], 4);
    memcpy(&oamMemory[a2], tmp,            4);
    p1->oamAddress = a2;
    p2->oamAddress = a1;
}

void char_draw(Character *c)
{
    u8 row, col, i = 0;
    s16 baseX, baseY;
    int displayY;

    if (!c->alive || !c->visible) return;

    baseX = c->x;
    displayY = c->y + c->z - c->a;
    baseY = displayY - (CANVAS_H - 1);

    for (row = 0; row < 6; row++)
    {
        for (col = 0; col < 4; col++)
        {
            u8 drawCol = c->hflip ? (3 - col) : col;
            u16 oamId = c->oamAddress + i * 4;

            oamSet(oamId,
                   baseX + drawCol * 16,
                   baseY + row * 16,
                   3,
                   c->hflip,
                   0,
                   c->vramOffset + (u8[]){
                      0,  2,  4,  6,
                      8, 10, 12, 14,
                     32, 34, 36, 38,
                     40, 42, 44, 46,
                     64, 66, 68, 70,
                     72, 74, 76, 78
                   }[i],
                   c->paletteSlot);
            oamSetEx(oamId, OBJ_SMALL, OBJ_SHOW);
            i++;
        }
    }
}
