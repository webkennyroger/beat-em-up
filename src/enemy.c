#include "enemy.h"
#include "types.h"
#include "entity.h"
#include "control.h"
#include "combat.h"
#include "hud.h"

SpriteFrame gHunterFrames[ENEMY_TOTAL_FRAMES] = {
    { &hunter_idle_tiles,   &hunter_palette, STATE_IDLE,  STATE_WALK,  ENEMY_WALK_1,  60, 0,63,0,63, false },
    { &hunter_idle_tiles,   &hunter_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_2,  10, 0,63,0,63, false },
    { &hunter_idle_tiles,   &hunter_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_1,  10, 0,63,0,63, false },
    { &hunter_idle_tiles,   &hunter_palette, STATE_PUNCH, STATE_IDLE,  ENEMY_IDLE_1,  12,35,63,20,55, true  },
    { &hunter_idle_tiles,   &hunter_palette, STATE_HURT,  STATE_HURT,  ENEMY_HURT_1, 999, 0,63,0,63, false },
    { &hunter_idle_tiles,   &hunter_palette, STATE_DEAD,  STATE_DEAD,  ENEMY_DEAD_1, 999, 0,63,0,63, false },
};

SpriteFrame gMayFrames[ENEMY_TOTAL_FRAMES] = {
    { &may_idle_tiles,   &may_palette, STATE_IDLE,  STATE_WALK,  ENEMY_WALK_1,  50, 0,63,0,63, false },
    { &may_idle_tiles,   &may_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_2,   8, 0,63,0,63, false },
    { &may_idle_tiles,   &may_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_1,   8, 0,63,0,63, false },
    { &may_idle_tiles,   &may_palette, STATE_PUNCH, STATE_IDLE,  ENEMY_IDLE_1,  10,30,63,15,50, true  },
    { &may_idle_tiles,   &may_palette, STATE_HURT,  STATE_HURT,  ENEMY_HURT_1, 999, 0,63,0,63, false },
    { &may_idle_tiles,   &may_palette, STATE_DEAD,  STATE_DEAD,  ENEMY_DEAD_1, 999, 0,63,0,63, false },
};

SpriteFrame gAndoreFrames[ENEMY_TOTAL_FRAMES] = {
    { &andore_idle_tiles,   &andore_palette, STATE_IDLE,  STATE_WALK,  ENEMY_WALK_1,  40, 0,63,0,63, false },
    { &andore_idle_tiles,   &andore_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_2,  12, 0,63,0,63, false },
    { &andore_idle_tiles,   &andore_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_1,  12, 0,63,0,63, false },
    { &andore_idle_tiles,   &andore_palette, STATE_PUNCH, STATE_IDLE,  ENEMY_IDLE_1,  15,28,63,10,58, true  },
    { &andore_idle_tiles,   &andore_palette, STATE_HURT,  STATE_HURT,  ENEMY_HURT_1, 999, 0,63,0,63, false },
    { &andore_idle_tiles,   &andore_palette, STATE_DEAD,  STATE_DEAD,  ENEMY_DEAD_1, 999, 0,63,0,63, false },
};

static char sHunterName[] = "HUNTER";
static char sMayName[]    = "MAY";
static char sAndoreName[] = "ANDORE";

static void _setup_ai(Character *c, u8 hp, u8 speed, u8 hurtDur,
                      u8 aggression, u8 walkSpeed) {
    c->hp = hp;
    c->maxHp = hp;
    c->isEnemy = true;
    c->speed = speed;
    c->hurtDuration = hurtDur;
    c->aggression = aggression;
    c->walkSpeed = walkSpeed;
    c->runSpeed = walkSpeed;
    c->attackPower = 1;
    c->hostileMask = TYPE_PLAYER;
    c->candamageMask = TYPE_PLAYER;
    c->entityType = TYPE_ENEMY;
    c->aiMoveType = AI_MOVE_CHASE;
    c->reactionTimer = 0;
}

void enemy_init_hunter(Character *c) {
    c->frames       = gHunterFrames;
    c->totalFrames  = ENEMY_TOTAL_FRAMES;
    c->name         = sHunterName;
    _setup_ai(c, 6, 1, 28, 40, 1);
}

void enemy_init_may(Character *c) {
    c->frames       = gMayFrames;
    c->totalFrames  = ENEMY_TOTAL_FRAMES;
    c->name         = sMayName;
    _setup_ai(c, 5, 0, 22, 30, 1);
}

void enemy_init_andore(Character *c) {
    c->frames       = gAndoreFrames;
    c->totalFrames  = ENEMY_TOTAL_FRAMES;
    c->name         = sAndoreName;
    _setup_ai(c, 14, 2, 35, 60, 2);
}

#define ATTACK_RANGE_X  35
#define ATTACK_RANGE_Y   8

void enemy_update(Character *c, Character *player) {
    control_enemy(c);
}

typedef struct {
    bool  used;
    bool  spawned;
    u8    enemyType;
    u8    count;
    int   spawnX;
    int   spawnY;
    int   spawnZ;
    int   triggerX;
} WaveSlot;

#define MAX_WAVES 6
static WaveSlot sWaves[MAX_WAVES];

void enemy_init_waves(void) {
    int i;
    for (i = 0; i < MAX_WAVES; i++)
        sWaves[i].used = false;

    sWaves[0].used = true; sWaves[0].spawned = false;
    sWaves[0].enemyType = 0; sWaves[0].count = 2;
    sWaves[0].spawnX = 220; sWaves[0].spawnY = 160;
    sWaves[0].spawnZ = 24;
    sWaves[0].triggerX = 100;

    sWaves[1].used = true; sWaves[1].spawned = false;
    sWaves[1].enemyType = 1; sWaves[1].count = 1;
    sWaves[1].spawnX = 230; sWaves[1].spawnY = 155;
    sWaves[1].spawnZ = 24;
    sWaves[1].triggerX = 200;

    sWaves[2].used = true; sWaves[2].spawned = false;
    sWaves[2].enemyType = 0; sWaves[2].count = 2;
    sWaves[2].spawnX = 240; sWaves[2].spawnY = 160;
    sWaves[2].spawnZ = 24;
    sWaves[2].triggerX = 280;

    sWaves[3].used = true; sWaves[3].spawned = false;
    sWaves[3].enemyType = 1; sWaves[3].count = 2;
    sWaves[3].spawnX = 250; sWaves[3].spawnY = 155;
    sWaves[3].spawnZ = 24;
    sWaves[3].triggerX = 400;

    sWaves[4].used = true; sWaves[4].spawned = false;
    sWaves[4].enemyType = 0; sWaves[4].count = 2;
    sWaves[4].spawnX = 260; sWaves[4].spawnY = 160;
    sWaves[4].spawnZ = 24;
    sWaves[4].triggerX = 400;

    sWaves[5].used = true; sWaves[5].spawned = false;
    sWaves[5].enemyType = 2; sWaves[5].count = 1;
    sWaves[5].spawnX = 230; sWaves[5].spawnY = 150;
    sWaves[5].spawnZ = 24;
    sWaves[5].triggerX = 550;
}

static void _spawn_enemy(u8 type, int spawnX, int spawnY, int spawnZ) {
    int i;
    Character *slot = NULL;

    for (i = 1; i < MAX_CHARS; i++) {
        if (!gCharacters[i].alive) { slot = &gCharacters[i]; break; }
    }
    if (!slot) return;

    slot->entityIndex = i;
    char_init(slot, GUY_TILE_SIZE, 32);
    slot->x = spawnX;
    slot->y = spawnY;
    slot->z = spawnZ;
    slot->a = 0;

    u8 oamAddr = (u8)(i * 24);
    u16 vramAddr = i * (GUY_TILE_SIZE / 2);
    u16 tileOffs = (u16)(i * 96);

    switch (type) {
        case 0: enemy_init_hunter(slot); break;
        case 1: enemy_init_may(slot);    break;
        case 2: enemy_init_andore(slot); break;
    }

    char_load_gfx(slot, oamAddr, vramAddr, tileOffs, (u8)i);
}

void enemy_update_waves(Character *player) {
    static bool sScored[MAX_CHARS];
    int i;

    for (i = 0; i < MAX_WAVES; i++) {
        WaveSlot *w = &sWaves[i];
        if (!w->used || w->spawned) continue;
        if (player->x >= w->triggerX) {
            w->spawned = true;
            int j;
            for (j = 0; j < w->count; j++) {
                _spawn_enemy(w->enemyType,
                    w->spawnX + j * 40,
                    w->spawnY,
                    w->spawnZ + j * 8);
            }
        }
    }

    for (i = 1; i < MAX_CHARS; i++) {
        Character *e = &gCharacters[i];
        if (!e->alive) {
            if (!sScored[i]) {
                sScored[i] = true;
            }
        } else {
            sScored[i] = false;
        }
    }
}
