#include <snes.h>
#include "src/types.h"
#include "src/entity.h"
#include "src/anim.h"
#include "src/physics.h"
#include "src/combat.h"
#include "src/control.h"
#include "src/player.h"
#include "src/enemy.h"
#include "src/hud.h"
#include "src/vblank.h"

void scanPads(void);

static enum { GS_PLAY, GS_GAMEOVER } gGameState;

extern char BG1_tiles, BG1_tiles_end;
extern char BG1_pal,   BG1_pal_end;
extern char BG1_map,   BG1_map_end;

extern char BG2_tiles, BG2_tiles_end;
extern char BG2_pal,   BG2_pal_end;
extern char BG2_map,   BG2_map_end;

extern char HUD_tiles, HUD_tiles_end;
extern char HUD_pal,   HUD_pal_end;
extern char HUD_map,   HUD_map_end;

extern char guy_idle_tiles,  guy_idle_tiles_end;
extern char guy_palette,     guy_palette_end;

typedef struct {
    u8   id;
    u16  vram;
    u16  scrX, scrY;
    u8  *bgMap;
    u8   mapPage;
    bool vRamFirstPage;
    u16  maxScrollX;
} BGScroll;

typedef struct {
    u8  *gfxoffset;
    u16  adrgfxvram;
    u16  size;
} BGPage;

typedef struct {
    BGPage bg1, bg2;
    u8 refreshBG1, refreshBG2;
} BGInfo;

static BGScroll bgMain, bgSub;
static BGInfo   bgInfo;

void myconsoleVblank(void) {
    scanPads();
    dmaCopyOAram((unsigned char*)&oamMemory, 0, 0x220);

    if (bgInfo.refreshBG1) {
        dmaCopyVram(bgInfo.bg1.gfxoffset, bgInfo.bg1.adrgfxvram, bgInfo.bg1.size);
        bgInfo.refreshBG1 = 0;
    }
    if (bgInfo.refreshBG2) {
        dmaCopyVram(bgInfo.bg2.gfxoffset, bgInfo.bg2.adrgfxvram, bgInfo.bg2.size);
        bgInfo.refreshBG2 = 0;
    }

    vblank_flush_sprite_queue();
    hud_draw();
}

static void updateBG1(u8 *data, u16 vram, int size) {
    bgInfo.bg1.gfxoffset  = data;
    bgInfo.bg1.adrgfxvram = vram;
    bgInfo.bg1.size       = size;
    bgInfo.refreshBG1     = 1;
}

static void updateBG2(u8 *data, u16 vram, int size) {
    bgInfo.bg2.gfxoffset  = data;
    bgInfo.bg2.adrgfxvram = vram;
    bgInfo.bg2.size       = size;
    bgInfo.refreshBG2     = 1;
}

static void handleScroll(BGScroll *s, int playerX, s8 playerVelX) {
    if (playerVelX <= 0) return;
    if (playerX <= 94) return;
    if (s->scrX < s->maxScrollX) s->scrX++;

    if (s->scrX >= (u16)((s->mapPage + 1) * 256)) {
        s->mapPage++;
        s->vRamFirstPage = !s->vRamFirstPage;
        u16 vram = s->vram + (s->vRamFirstPage ? 0 : 1024);
        if (s->id == 0) updateBG1(&BG1_map + 2048 * s->mapPage, vram, 2048);
        else            updateBG2(&BG2_map + 2048 * s->mapPage, vram, 2048);
    }
    bgSetScroll(s->id, s->scrX - 255, s->scrY - 223);
}

int main(void) {
    int i;

    bgSetMapPtr(0, 0x2000,        SC_64x32);
    bgSetMapPtr(1, 0x3000,        SC_64x32);
    bgSetMapPtr(2, 0x4000,        SC_32x32);

    bgInitTileSet(0, &BG1_tiles, &BG1_pal, 2,
                  (&BG1_tiles_end - &BG1_tiles), 32, BG_16COLORS, 0x5000);
    bgInitTileSet(1, &BG2_tiles, &BG2_pal, 4,
                  (&BG2_tiles_end - &BG2_tiles), 32, BG_16COLORS, 0x7000);
    bgInitTileSet(2, &HUD_tiles, &HUD_pal, 0,
                  (&HUD_tiles_end - &HUD_tiles), 8, BG_4COLORS, 0x6000);

    updateBG1(&BG1_map, 0x2000,        2048);
    dmaCopyVram((u8*)&BG1_map, 0x2000, 2048);
    updateBG2(&BG2_map, 0x3000,        2048);
    dmaCopyVram((u8*)&BG2_map, 0x3000, 2048);

    setMode(BG_MODE1, BG3_MODE1_PRIORITY_HIGH);
    dmaCopyVram((u8*)&HUD_map, 0x4000, (&HUD_map_end - &HUD_map));

    oamInitGfxSet(
        &guy_idle_tiles,
        (&guy_idle_tiles_end - &guy_idle_tiles),
        &guy_palette,
        32,
        0, 0x0000, OBJ_SIZE16_L32
    );

    for (i = 0; i < MAX_CHARS; i++) gYOrder[i] = NULL;

    gPlayer = &gCharacters[0];
    gPlayer->entityIndex = 0;
    gPlayer->entityType = TYPE_PLAYER;
    char_init(gPlayer, GUY_TILE_SIZE, 32);
    player_init(gPlayer);
    gPlayer->x = 48;
    gPlayer->z = 24;  // Center depth
    gPlayer->y = GROUND_Y;
    gPlayer->groundY = GROUND_Y;
    gPlayer->walkSpeed = PLAYER_SPEED_X;
    gPlayer->runSpeed = DASH_SPEED_X;
    gPlayer->attackPower = 1;
    gPlayer->specialCost = 3;
    gPlayer->hostileMask = TYPE_ENEMY;
    gPlayer->candamageMask = TYPE_ENEMY;
    char_load_gfx(gPlayer, 0, 0x0000, 0, 0);

    for (i = 1; i < MAX_CHARS; i++)
    {
        gCharacters[i].alive = false;
        gCharacters[i].entityIndex = i;
    }

    collision_init_projectiles();

    hud_init("GUY");
    hud_draw_hp(gPlayer->hp, gPlayer->maxHp);
    hud_draw_timer(gTimer);
    WaitForVBlank();
    hud_draw();

    enemy_init_waves();

    vblank_init();
    nmiSet(myconsoleVblank);

    setScreenOn();

    gGameState = GS_PLAY;

    bgMain.id           = 0;
    bgMain.scrX         = 255;
    bgMain.scrY         = 255;
    bgMain.vram         = 0x2000;
    bgMain.vRamFirstPage= true;
    bgMain.mapPage      = 0;
    bgMain.maxScrollX   = 768;

    bgSub.id            = 1;
    bgSub.scrX          = 255;
    bgSub.scrY          = 255;
    bgSub.vram          = 0x3000;
    bgSub.vRamFirstPage = true;
    bgSub.mapPage       = 0;
    bgSub.maxScrollX    = 1024 + 128;

    bgSetScroll(0, 0, 32);
    bgSetScroll(1, 0, 32);

    // ============================================================
    //  GAME LOOP 
    // ============================================================
    while (1) {
        u16 pad = padsCurrent(0);
        gFrames++;

        if (gGameState == GS_GAMEOVER) {
            WaitForVBlank();
            continue;
        }

        // 1) Engine: physics for all characters
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive) {
                physics_update(&gCharacters[i]);
            }
        }

        // 2) Player logic 
        if (gPlayer->alive)
            control_player(gPlayer);

        // 3) Check player death
        if (!gPlayer->alive) {
            gGameState = GS_GAMEOVER;
            continue;
        }

        // 4) Enemy logic
        for (i = 1; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                control_enemy(&gCharacters[i]);
        }

        // 5) Update hurt states for all characters
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive && gCharacters[i].isHurt)
                char_update_hurt(&gCharacters[i]);
        }

        // 6) Spawn waves
        enemy_update_waves(gPlayer);

        // 7) Animation updates
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                anim_update(&gCharacters[i]);
        }

        // 7) Legacy animation (for SpriteFrame DMA)
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                char_update_gfx(&gCharacters[i]);
        }

        // 8) Position updates (for rendering + legacy physics)
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                char_update_pos(&gCharacters[i]);
        }

        // 9) Projectile spawn check
        {
            static u8 sLastSub = 255;
            u8 curSub = gPlayer->subState;
            if (gPlayer->state == STATE_SPECIAL2 && curSub == GUY_SPECIAL2_2 && sLastSub != curSub) {
                int dir = gPlayer->hflip ? -1 : 1;
                collision_spawn_projectile(
                    gPlayer->x + (dir > 0 ? 50 : -16),
                    gPlayer->y - 30,
                    dir * 4, 2, true
                );
            }
            sLastSub = curSub;
            if (gPlayer->state != STATE_SPECIAL2) sLastSub = 255;
        }

        // 10) Combat hit checks 
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                combat_check_hits(&gCharacters[i]);
        }

        // 11) Projectile movement
        collision_update_projectiles();

        // 12) Overlap resolution
        collision_resolve_overlap();

        // 13) Z-depth sort
        char_sort_y_order();

        // 14) Draw all characters
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                char_draw(&gCharacters[i]);
        }

        // 15) HUD
        hud_draw_hp(gPlayer->hp, gPlayer->maxHp);

        // 16) Scrolling
        handleScroll(&bgMain, gPlayer->x, gPlayer->velX);
        handleScroll(&bgSub,  gPlayer->x, gPlayer->velX);

        // 17) VBlank
        WaitForVBlank();
    }

    return 0;
}
