#include "anim.h"

static u8 _find_first_frame(Character *ent, u8 state)
{
    u16 i;
    for (i = 0; i < ent->totalFrames; i++)
    {
        if (ent->frames[i].state == state)
            return i;
    }
    return 0;
}

void anim_set_state(Character *ent, u8 new_state)
{
    if (ent->state == new_state && ent->subState == _find_first_frame(ent, new_state))
        return;

    ent->prevState = ent->state;
    ent->prevSubState = ent->subState;
    ent->state = new_state;
    ent->subState = _find_first_frame(ent, new_state);
    ent->frameCounter = 0;

    ent->didHit = 0;
    ent->comboQueued = 0;
}

void anim_update(Character *ent)
{
    SpriteFrame *f;

    if (!ent->alive)
        return;

    f = &ent->frames[ent->subState];

    ent->frameCounter++;

    if (ent->frameCounter >= f->duration)
    {
        if (f->duration < 900)
        {
            u8 next_state = f->nextState;
            u8 next_sub = f->nextSubState;

            if (ent->state == STATE_PUNCH && f->nextState == STATE_IDLE)
            {
                if (ent->comboQueued && ent->didHit)
                {
                    ent->attackId++;
                    anim_set_state(ent, STATE_KICK);
                    return;
                }
            }
            else if (ent->state == STATE_KICK && f->nextState == STATE_IDLE)
            {
                if (ent->comboQueued && ent->didHit)
                {
                    ent->attackId++;
                    anim_set_state(ent, STATE_SPECIAL1);
                    return;
                }
            }

            if (ent->state == STATE_SPECIAL1 || ent->state == STATE_SPECIAL2 ||
                ent->state == STATE_DASH_ATTACK || ent->state == STATE_JUMP_KICK ||
                ent->state == STATE_LOW_KICK || ent->state == STATE_ELBOW_DROP ||
                ent->state == STATE_SUPER || ent->state == STATE_MEGA_CRASH ||
                ent->state == STATE_TURN_ATTACK)
            {
                if (next_state == STATE_IDLE)
                {
                    anim_set_state(ent, STATE_IDLE);
                    return;
                }
            }

            if (ent->state == STATE_DEAD)
            {
                ent->subState = next_sub;
                ent->frameCounter = 0;
                return;
            }

            if (ent->state == STATE_HURT && f->duration == 999)
            {
                return;
            }

            ent->subState = next_sub;
            ent->frameCounter = 0;
        }
    }
}
