#include "control.h"
#include "anim.h"
#include "combat.h"
#include "entity.h"

void control_player(Character *entity)
{
    u16 pad, pad_down;

    if (!entity->alive || entity->state == STATE_HURT ||
        entity->state == STATE_DEAD || entity->state == STATE_DASH)
        return;

    pad = padsCurrent(entity->entityIndex);
    pad_down = padsDown(entity->entityIndex);

    update_input_buffer(entity, pad);
    check_double_tap(entity, pad, pad_down);
    entity->current_buttons = pad;

    if (entity->a == 0 && check_desperation(entity))
    {
        anim_set_state(entity, STATE_SPECIAL2);
        return;
    }

    if (entity->a == 0 && !entity->isRunning && check_motion_special(entity))
    {
        entity->velX = 0;
        entity->velZ = 0;
        anim_set_state(entity, STATE_SPECIAL1);
        return;
    }

    if (entity->a == 0)
    {
        entity->velX = 0;
        entity->velZ = 0;
    }

    if (!(entity->state == STATE_PUNCH || entity->state == STATE_KICK ||
          entity->state == STATE_SPECIAL1 || entity->state == STATE_SPECIAL2 ||
          entity->state == STATE_JUMP_KICK || entity->state == STATE_DASH_ATTACK))
    {
        u8 current_speed = (entity->isRunning) ? DASH_SPEED_X : PLAYER_SPEED_X;

        if (entity->a == 0)
        {
            if (pad & KEY_LEFT)
            {
                entity->velX = -current_speed;
                entity->hflip = 1;
            }
            if (pad & KEY_RIGHT)
            {
                entity->velX = current_speed;
                entity->hflip = 0;
            }
            if (pad & KEY_UP)
                entity->velZ = -PLAYER_SPEED_Z;
            if (pad & KEY_DOWN)
                entity->velZ = PLAYER_SPEED_Z;

            if (entity->isRunning && (entity->velX != 0))
                anim_set_state(entity, STATE_DASH);
            else if (entity->velX != 0 || entity->velZ != 0)
                anim_set_state(entity, STATE_WALK);
            else
            {
                if (entity->state == STATE_WALK || entity->state == STATE_DASH)
                    anim_set_state(entity, STATE_IDLE);
                entity->isRunning = 0;
            }

            if (pad_down & KEY_B)
            {
                entity->velA = JUMP_VEL;
                anim_set_state(entity, STATE_JUMP);
            }
        }
        else
        {
            if (pad_down & KEY_B)
                anim_set_state(entity, STATE_JUMP_KICK);
        }
    }

    if ((pad_down & KEY_Y) && entity->a == 0)
    {
        if (entity->state < STATE_PUNCH && entity->state != STATE_DASH)
        {
            entity->velX = 0;
            entity->velZ = 0;
            entity->comboQueued = 0;
            entity->attackId++;
            entity->didHit = 0;

            if (entity->isRunning)
            {
                anim_set_state(entity, STATE_DASH_ATTACK);
                entity->isRunning = 0;
            }
            else
                anim_set_state(entity, STATE_PUNCH);
        }
        else if (entity->state == STATE_PUNCH)
            entity->comboQueued = 1;
        else if (entity->state == STATE_KICK)
            entity->comboQueued = 1;
    }

    if ((pad_down & KEY_A) && entity->a == 0 && entity->state != STATE_PUNCH)
    {
        entity->velX = 0;
        entity->velZ = 0;
        anim_set_state(entity, STATE_KICK);
    }

    if (entity->state == STATE_PUNCH || entity->state == STATE_KICK ||
        entity->state == STATE_SPECIAL1 || entity->state == STATE_DASH_ATTACK ||
        entity->state == STATE_JUMP_KICK)
    {
        combat_check_hits(entity);
    }
}

void control_enemy(Character *entity)
{
    int i;
    Character *target = NULL;
    s16 dx, dz;
    u16 dist_x, dist_z;
    u8 hostile_mask;
    u8 speed;

    if (!entity->alive || entity->state == STATE_HURT ||
        entity->state == STATE_DEAD)
        return;

    hostile_mask = entity->hostileMask;

    if (gCharacters[0].alive && (gCharacters[0].entityType & hostile_mask))
        target = &gCharacters[0];
    else
    {
        for (i = 0; i < MAX_CHARS; i++)
        {
            if (gCharacters[i].alive &&
                (gCharacters[i].entityType & hostile_mask) &&
                gCharacters[i].hp > 0)
            {
                target = &gCharacters[i];
                break;
            }
        }
    }

    if (target == NULL)
    {
        entity->velX = 0;
        entity->velZ = 0;
        return;
    }

    dx = target->x - entity->x;
    dz = target->z - entity->z;
    dist_x = (dx < 0) ? -dx : dx;
    dist_z = (dz < 0) ? -dz : dz;

    if (entity->state == STATE_PUNCH)
    {
        entity->velX = 0;
        entity->velZ = 0;
    }
    else if (dist_x < ATTACK_RANGE_X && dist_z < ATTACK_RANGE_Z)
    {
        entity->velX = 0;
        entity->velZ = 0;

        entity->hflip = (dx > 0) ? 0 : 1;

        if (entity->reactionTimer == 0)
        {
            entity->velX = 0;
            entity->velZ = 0;
            entity->comboQueued = 0;
            entity->attackId++;
            entity->didHit = 0;
            anim_set_state(entity, STATE_PUNCH);

            if (entity->stats && entity->stats->aggression >= 100)
                entity->reactionTimer = 10;
            else if (entity->stats)
                entity->reactionTimer = BASE_REACTION_TIME - entity->stats->aggression;
            else
                entity->reactionTimer = 60;
        }
        else
            entity->reactionTimer--;
    }
    else
    {
        speed = entity->walkSpeed ? entity->walkSpeed : 1;

        entity->velX = 0;
        entity->velZ = 0;

        if (dx > 10)
        {
            entity->velX = speed;
            entity->hflip = 0;
        }
        else if (dx < -10)
        {
            entity->velX = -speed;
            entity->hflip = 1;
        }

        if (dz > 4)
            entity->velZ = speed;
        else if (dz < -4)
            entity->velZ = -speed;

        if (entity->velX != 0 || entity->velZ != 0)
            anim_set_state(entity, STATE_WALK);
        else
            anim_set_state(entity, STATE_IDLE);

        entity->reactionTimer = 20;
    }

    if (entity->state == STATE_PUNCH || entity->state == STATE_KICK)
        combat_check_hits(entity);
}
