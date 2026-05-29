#ifndef TYPES_H
#define TYPES_H

#include <snes.h>

#define MAX_CHARS       5
#define MAX_SPR_QUEUE   16
#define HUD_SIZE        160
#define MAX_PROJECTILES 3
#define MAX_ENTITIES    MAX_CHARS

typedef enum {
    TYPE_NONE = 0,
    TYPE_PLAYER,
    TYPE_ENEMY,
    TYPE_ITEM,
    TYPE_OBSTACLE
} EntityType;

typedef enum {
    AI_MOVE_CHASE = 0,
    AI_MOVE_CHASE_X,
    AI_MOVE_CHASE_Z,
    AI_MOVE_AVOID,
    AI_MOVE_WANDER,
    AI_MOVE_HOLS
} AIMoveType;

typedef struct {
    s8 x_offset, y_offset;
    u8 width, height, z_depth;
} Box;

typedef struct _CharStats {
    u16 max_health;
    const u8 *name;
    u8 walk_speed;
    u8 attack_power;
    u8 run_speed;
    u8 special_cost;
    Box body_box;
    AIMoveType ai_move;
    u8 aggression;
    u8 hostile_mask;
    u8 candamage_mask;
} CharStats;

#define INPUT_BUF_SIZE      30
#define DOUBLE_TAP_WINDOW   15
#define BASE_REACTION_TIME  120
#define ATTACK_RANGE_X      80
#define ATTACK_RANGE_Z      8
#define KNOCK_PUSH          3
#define KNOCK_LIFT          8
#define KNOCKBACK_FORCE     4
#define HIT_PUSH            2

#define STATE_IDLE          0
#define STATE_WALK          1
#define STATE_PUNCH         2
#define STATE_KICK          3
#define STATE_HURT          4
#define STATE_DEAD          5
#define STATE_JUMP          6
#define STATE_GRAB          7
#define STATE_JUMP_KICK     8
#define STATE_DASH          9
#define STATE_SPECIAL1     10
#define STATE_SPECIAL2     11
#define STATE_LOW_KICK     12
#define STATE_DASH_ATTACK  13
#define STATE_ELBOW_DROP   14
#define STATE_FLYING_KICK  15
#define STATE_WALL_JUMP    16
#define STATE_TURN_ATTACK  17
#define STATE_GRAB_PUNCH   18
#define STATE_SHOULDER_THROW 19
#define STATE_BACK_VAULT   20
#define STATE_GERMAN_SUPLEX 21
#define STATE_MEGA_CRASH   22
#define STATE_SENPUUKYAKU  23
#define STATE_IZUNA_OTOSHI 24
#define STATE_SUPER        25

#define GUY_IDLE_1       0

#define GUY_WALK_1       1
#define GUY_WALK_2       2
#define GUY_WALK_3       3
#define GUY_WALK_4       4
#define GUY_WALK_5       5
#define GUY_WALK_6       6
#define GUY_WALK_7       7
#define GUY_WALK_8       8

#define GUY_PUNCH_1      9
#define GUY_PUNCH_2     10
#define GUY_PUNCH_3     11
#define GUY_PUNCH_4     12

#define GUY_KICK_1      13
#define GUY_KICK_2      14
#define GUY_KICK_3      15
#define GUY_KICK_4      16

#define GUY_HURT_1      17
#define GUY_HURT_2      18
#define GUY_DEAD_1      19

#define GUY_GRAB_1      20
#define GUY_GRAB_2      21
#define GUY_GRAB_3      22

#define GUY_JUMP_1      23
#define GUY_JUMP_2      24
#define GUY_JUMP_3      25
#define GUY_JUMP_4      26

#define GUY_JUMP_KICK_1 27
#define GUY_JUMP_KICK_2 28
#define GUY_JUMP_KICK_3 29
#define GUY_JUMP_KICK_4 30

#define GUY_DASH_1 31
#define GUY_DASH_2 32
#define GUY_DASH_3 33
#define GUY_DASH_4 34
#define GUY_DASH_5 35
#define GUY_DASH_6 36

#define GUY_SPECIAL1_1 37
#define GUY_SPECIAL1_2 38
#define GUY_SPECIAL1_3 39
#define GUY_SPECIAL1_4 40
#define GUY_SPECIAL1_5 41
#define GUY_SPECIAL1_6 42
#define GUY_SPECIAL1_7 43
#define GUY_SPECIAL1_8 44

#define GUY_SPECIAL2_1 45
#define GUY_SPECIAL2_2 46
#define GUY_SPECIAL2_3 47

#define GUY_DASH_KICK_1 48
#define GUY_DASH_KICK_2 49
#define GUY_DASH_KICK_3 50
#define GUY_DASH_KICK_4 51
#define GUY_DASH_KICK_5 52
#define GUY_DASH_KICK_6 53
#define GUY_DASH_KICK_7 54
#define GUY_DASH_KICK_8 55

#define GUY_LOW_KICK_1 56
#define GUY_LOW_KICK_2 57
#define GUY_LOW_KICK_3 58

#define GUY_ELBOW_DROP_1 59
#define GUY_ELBOW_DROP_2 60
#define GUY_ELBOW_DROP_3 61
#define GUY_ELBOW_DROP_4 62

#define GUY_SUPER_1 63
#define GUY_SUPER_2 64
#define GUY_SUPER_3 65
#define GUY_SUPER_4 66
#define GUY_SUPER_5 67
#define GUY_SUPER_6 68
#define GUY_SUPER_7 69
#define GUY_SUPER_8 70

#define GUY_SUPERPRETO_1 71
#define GUY_SUPERPRETO_2 72
#define GUY_SUPERPRETO_3 73
#define GUY_SUPERPRETO_4 74

#define GUY_WALL_JUMP_1 75
#define GUY_WALL_JUMP_2 76
#define GUY_WALL_JUMP_3 77

#define GUY_TURN_ATTACK_1 78
#define GUY_TURN_ATTACK_2 79
#define GUY_TURN_ATTACK_3 80

#define GUY_GRAB_ATTACK_1 81
#define GUY_GRAB_ATTACK_2 82
#define GUY_GRAB_ATTACK_3 83
#define GUY_GRAB_ATTACK_4 84
#define GUY_GRAB_ATTACK_5 85
#define GUY_GRAB_ATTACK_6 86
#define GUY_GRAB_ATTACK_7 87
#define GUY_GRAB_ATTACK_8 88

#define GUY_MEGA_CRASH_1 89
#define GUY_MEGA_CRASH_2 90
#define GUY_MEGA_CRASH_3 91
#define GUY_MEGA_CRASH_4 92
#define GUY_MEGA_CRASH_5 93
#define GUY_MEGA_CRASH_6 94
#define GUY_MEGA_CRASH_7 95

#define GUY_IZUNA_OTOSHI_1 96
#define GUY_IZUNA_OTOSHI_2 97
#define GUY_IZUNA_OTOSHI_3 98
#define GUY_IZUNA_OTOSHI_4 99
#define GUY_IZUNA_OTOSHI_5 100
#define GUY_IZUNA_OTOSHI_6 101

#define GUY_GRAPPLE_FINISH_1 102
#define GUY_GRAPPLE_FINISH_2 103
#define GUY_GRAPPLE_FINISH_3 104
#define GUY_GRAPPLE_FINISH_4 105
#define GUY_GRAPPLE_FINISH_5 106
#define GUY_GRAPPLE_FINISH_6 107
#define GUY_GRAPPLE_FINISH_7 108
#define GUY_GRAPPLE_FINISH_8 109

#define GUY_TOTAL_FRAMES 110

#define ENEMY_IDLE_1    0
#define ENEMY_WALK_1    1
#define ENEMY_WALK_2    2
#define ENEMY_ATTACK_1  3
#define ENEMY_HURT_1    4
#define ENEMY_DEAD_1    5
#define ENEMY_TOTAL_FRAMES 6

#define PLAYER_SPEED_X  3
#define PLAYER_SPEED_Y  2
#define DASH_SPEED_X    6
#define ENEMY_SPEED_X   1
#define ENEMY_SPEED_Y   1

#define GRAVITY          1
#define JUMP_VEL         7
#define GROUND_Y        160

#define SCREEN_MIN_X   -16
#define SCREEN_MAX_X    218
#define SCREEN_MIN_Y    90
#define SCREEN_MAX_Y    190

#define MIN_Z            0
#define MAX_Z            64
#define PLAYER_SPEED_Z   1
#define ENEMY_SPEED_Z    1

#define GUY_TILE_SIZE   0x0C00

typedef struct {
    u8  *tiles;
    u8  *palette;
    u16  state;
    u16  nextState;
    u16  nextSubState;
    u16  duration;
    u8   hitboxX1;
    u8   hitboxX2;
    u8   hitboxY1;
    u8   hitboxY2;
    bool hasHitbox;
} SpriteFrame;

typedef struct _Character {
    u16         tileSize;
    u16         paletteSize;
    u8          oamAddress;
    SpriteFrame *frames;
    u16         totalFrames;
    u16         vramAddress;
    u16         vramOffset;

    u8  state;
    u8  subState;
    u8  prevState;
    u8  prevSubState;
    u16 frameCounter;

    int   x, y;
    int   oldX, oldY;
    s8    velX, velY;
    u8    speed;
    u8    speedTimer;
    u8    hflip;
    int   groundY;

    int   z, a;
    s8    velZ;
    s16   velA;
    int   groundZ;

    EntityType entityType;
    u8         entityIndex;

    u16 inputBuffer[INPUT_BUF_SIZE];
    u8  inputHead;
    u16 lastTapButton;
    u16 tapTimer;
    u8  isRunning;
    u16 current_buttons;
    u16 previous_buttons;

    u8  didHit;
    u8  attackId;
    u8  lastHitId;
    u16 reactionTimer;
    u8  comboQueued;
    u8  comboStep;
    u8  knockdown;

    u8  aggression;
    u8  hostileMask;
    u8  candamageMask;
    AIMoveType aiMoveType;
    EntityType aiTargetType;

    CharStats *stats;
    u8 walkSpeed;
    u8 runSpeed;
    u8 attackPower;
    u8 specialCost;

    u8   hp;
    u8   maxHp;
    bool alive;
    bool isEnemy;
    bool isInvincible;
    bool isHurt;
    u8   hurtTimer;
    u8   hurtDuration;
    u8   hurtShake;

    u8    paletteSlot;
    bool  visible;
    u8    priority;

    char *name;
} Character;

typedef struct {
    int  x, y;
    int  velX;
    u8   oamId;
    u8   tileIndex;
    u8   paletteSlot;
    u8   damage;
    u8   life;
    bool active;
    bool friendly;
    u8   hflip;
} Projectile;

#endif
