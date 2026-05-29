#include "physics.h"
#include "anim.h"

void physics_update(Character *ent)
{
    if (!ent->alive)
        return;

    ent->x += ent->velX;

    ent->z += ent->velZ;

    if (ent->z < MIN_Z) ent->z = MIN_Z;
    if (ent->z > MAX_Z) ent->z = MAX_Z;

    if (ent->x > SCREEN_MAX_X) ent->x = SCREEN_MAX_X;
    if (ent->x < SCREEN_MIN_X) ent->x = SCREEN_MIN_X;

    if (ent->a > 0 || ent->velA != 0)
    {
        ent->a += ent->velA;
        ent->velA -= GRAVITY;

        if (ent->a <= 0)
        {
            ent->a = 0;
            ent->velA = 0;

            if (ent->state == STATE_JUMP || ent->state == STATE_JUMP_KICK)
                anim_set_state(ent, STATE_IDLE);
        }
    }
}
