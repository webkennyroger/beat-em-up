#include "combat.h"
#include "anim.h"
#include "entity.h"

#define SAME_PLANE_Z  12

Projectile gProjectiles[MAX_PROJECTILES];

// ============================================================
//  Input buffer
// ============================================================

void update_input_buffer(Character *ent, u16 pad)
{
    ent->inputBuffer[ent->inputHead] = pad;
    ent->inputHead++;
    if (ent->inputHead >= INPUT_BUF_SIZE)
        ent->inputHead = 0;
}

u16 get_input_at(Character *ent, u8 frames_back)
{
    int idx = ent->inputHead - 1 - frames_back;
    if (idx < 0)
        idx += INPUT_BUF_SIZE;
    return ent->inputBuffer[idx];
}

void check_double_tap(Character *ent, u16 pad, u16 pad_down)
{
    u16 released = (ent->previous_buttons ^ pad) & ent->previous_buttons;

    if (released & (KEY_LEFT | KEY_RIGHT))
    {
        ent->lastTapButton = released & (KEY_LEFT | KEY_RIGHT);
        ent->tapTimer = DOUBLE_TAP_WINDOW;
    }

    if (ent->tapTimer > 0)
        ent->tapTimer--;
    else
        ent->lastTapButton = 0;

    if ((pad_down & KEY_LEFT) && (ent->lastTapButton & KEY_LEFT))
        ent->isRunning = 1;
    if ((pad_down & KEY_RIGHT) && (ent->lastTapButton & KEY_RIGHT))
        ent->isRunning = 1;

    if (ent->isRunning)
    {
        if (ent->hflip == 1 && !(pad & KEY_LEFT))
            ent->isRunning = 0;
        if (ent->hflip == 0 && !(pad & KEY_RIGHT))
            ent->isRunning = 0;
    }
}

int check_motion_special(Character *ent)
{
    int i, j;
    u16 KEY_FORWARD;

    if (!(padsDown(ent->entityIndex) & KEY_Y))
        return 0;

    KEY_FORWARD = (ent->hflip == 0) ? KEY_RIGHT : KEY_LEFT;

    for (i = 1; i < 8; i++)
    {
        if (get_input_at(ent, i) & KEY_FORWARD)
        {
            for (j = i + 1; j < 16; j++)
            {
                if (get_input_at(ent, j) & KEY_DOWN)
                    return 1;
            }
        }
    }
    return 0;
}

int check_desperation(Character *ent)
{
    u16 pad_down = padsDown(ent->entityIndex);
    if (pad_down & KEY_A)
    {
        if (ent->hp > (ent->specialCost ? ent->specialCost : 3))
        {
            ent->hp -= ent->specialCost ? ent->specialCost : 3;
            ent->isRunning = 0;
            ent->velX = 0;
            ent->velZ = 0;
            return 1;
        }
    }
    return 0;
}

// ============================================================
//  Hit detection & damage
// ============================================================

int check_overlap(s16 min1, s16 max1, s16 min2, s16 max2)
{
    return (min1 < max2 && max1 > min2);
}

bool check_attack_3d(Character *att, Character *def)
{
    SpriteFrame *af, *df;
    u8 hitboxW, hitboxH;
    int ax1, ax2, ay1, ay2;
    int dx1, dx2, dy1, dy2;
    int att_z1, att_z2, def_z1, def_z2;
    int aa1, aa2, da1, da2;

    if (!def->alive || def->state == STATE_DEAD)
        return 0;
    if (att->didHit)
        return 0;

    af = &att->frames[att->subState];
    if (!af->hasHitbox)
        return 0;

    df = &def->frames[def->subState];
    hitboxW = af->hitboxX2 - af->hitboxX1;
    hitboxH = af->hitboxY2 - af->hitboxY1;

    ax1 = att->x + (att->hflip ? (63 - af->hitboxX2) : af->hitboxX1);
    ax2 = ax1 + hitboxW;
    ay1 = att->y + att->z - att->a - af->hitboxY2;
    ay2 = ay1 + hitboxH;

    dx1 = def->x;
    dx2 = def->x + 63;
    dy1 = def->y + def->z - def->a - 63;
    dy2 = def->y + def->z - def->a;

    if (!check_overlap(ax1, ax2, dx1, dx2))
        return 0;

    att_z1 = att->z - 12;
    att_z2 = att->z + 12;
    def_z1 = def->z - 12;
    def_z2 = def->z + 12;
    if (!check_overlap(att_z1, att_z2, def_z1, def_z2))
        return 0;

    aa1 = att->a;
    aa2 = att->a + 64;
    da1 = def->a;
    da2 = def->a + 64;
    if (!check_overlap(aa1, aa2, da1, da2))
        return 0;

    return 1;
}

void apply_damage_directed(Character *target, u8 dmg, s8 knockback_dir)
{
    if (target->state == STATE_DEAD)
        return;
    if (target->isInvincible)
        return;

    if (target->hp > dmg)
    {
        target->hp -= dmg;
        target->isHurt = true;
        target->hurtTimer = target->hurtDuration;
        target->isInvincible = true;
        anim_set_state(target, STATE_HURT);
    }
    else
    {
        target->hp = 0;
        target->alive = false;
        target->knockdown = 1;
        anim_set_state(target, STATE_DEAD);
    }

    target->hflip = (knockback_dir > 0) ? 0 : 1;
    target->velX = KNOCKBACK_FORCE * knockback_dir;
    target->velZ = 0;
}

void apply_damage(Character *attacker, Character *target)
{
    s8 dir = (attacker->x < target->x) ? 1 : -1;
    apply_damage_directed(target, 1, dir);
}

void combat_check_hits(Character *entity)
{
    int i;
    u8 is_attacking;

    is_attacking = (entity->state == STATE_PUNCH ||
                    entity->state == STATE_KICK ||
                    entity->state == STATE_SPECIAL1 ||
                    entity->state == STATE_SPECIAL2 ||
                    entity->state == STATE_DASH_ATTACK ||
                    entity->state == STATE_JUMP_KICK ||
                    entity->state == STATE_LOW_KICK ||
                    entity->state == STATE_ELBOW_DROP ||
                    entity->state == STATE_SUPER ||
                    entity->state == STATE_MEGA_CRASH ||
                    entity->state == STATE_TURN_ATTACK);

    if (!is_attacking)
        return;

    for (i = 0; i < MAX_CHARS; i++)
    {
        Character *target = &gCharacters[i];

        if (i != entity->entityIndex && target->alive && target->hp > 0)
        {
            if (entity->isEnemy != target->isEnemy || i == 0)
            {
                if (target->lastHitId == entity->attackId)
                    continue;

                if (check_attack_3d(entity, target))
                {
                    entity->didHit = 1;
                    target->lastHitId = entity->attackId;
                    apply_damage(entity, target);
                }
            }
        }
    }
}

// ============================================================
//  Projectile system
// ============================================================

void collision_init_projectiles(void)
{
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        gProjectiles[i].active = false;
        gProjectiles[i].life = 0;
    }
}

void collision_spawn_projectile(int x, int y, int velX, u8 damage, bool friendly)
{
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        Projectile *p = &gProjectiles[i];
        if (p->active) continue;

        p->x = x;
        p->y = y;
        p->velX = velX;
        p->damage = damage;
        p->life = 60;
        p->active = true;
        p->friendly = friendly;
        p->hflip = (velX < 0);

        p->oamId = 120 + i * 4;
        oamSet(p->oamId, x, y, 3, p->hflip, 0, 0, 0);
        oamSetEx(p->oamId, OBJ_SMALL, OBJ_SHOW);
        return;
    }
}

void collision_update_projectiles(void)
{
    int i, j;

    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        Projectile *p = &gProjectiles[i];
        if (!p->active) continue;

        p->x += p->velX;
        p->life--;

        oamSetXY(p->oamId, p->x, p->y);

        if (p->life == 0 || p->x < -32 || p->x > 256)
        {
            p->active = false;
            oamSetVisible(p->oamId, OBJ_HIDE);
            continue;
        }

        for (j = 0; j < MAX_CHARS; j++)
        {
            Character *target = &gCharacters[j];
            int px1, px2, py1, py2;
            int tx1, tx2, ty1, ty2;

            if (!target->alive) continue;
            if (target->isInvincible) continue;
            if (p->friendly && !target->isEnemy) continue;
            if (!p->friendly && target->isEnemy) continue;

            px1 = p->x; px2 = p->x + 16;
            py1 = p->y; py2 = p->y + 16;

            tx1 = target->x; tx2 = target->x + 63;
            ty1 = target->y + target->z - target->a - 63;
            ty2 = target->y + target->z - target->a;

            if (check_overlap(px1, px2, tx1, tx2) &&
                check_overlap(py1, py2, ty1, ty2))
            {
                s8 kb_dir = (p->velX > 0) ? 1 : -1;
                apply_damage_directed(target, p->damage, kb_dir);

                p->active = false;
                p->life = 0;
                oamSetVisible(p->oamId, OBJ_HIDE);
                break;
            }
        }
    }
}

// ============================================================
//  Overlap resolution
// ============================================================

void collision_resolve_overlap(void)
{
    int i, j;

    for (i = 0; i < MAX_CHARS; i++)
    {
        if (!gCharacters[i].alive) continue;

        for (j = i + 1; j < MAX_CHARS; j++)
        {
            Character *p1 = &gCharacters[i];
            Character *p2 = &gCharacters[j];
            int dx, dz;

            if (!gCharacters[j].alive) continue;

            dx = p1->x - p2->x;
            if (dx < 0) dx = -dx;
            dz = p1->z - p2->z;
            if (dz < 0) dz = -dz;

            if (dx < 20 && dz < SAME_PLANE_Z)
            {
                if (p1->x < p2->x)  { p1->x--; p2->x++; }
                else                 { p1->x++; p2->x--; }

                if (p1->z < p2->z)
                {
                    if (p1->oamAddress < p2->oamAddress)
                        char_swap_oam(p1, p2);
                }
                else
                {
                    if (p2->oamAddress < p1->oamAddress)
                        char_swap_oam(p1, p2);
                }
            }
        }
    }
}
