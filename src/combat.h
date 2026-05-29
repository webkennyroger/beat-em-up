#ifndef COMBAT_H
#define COMBAT_H

#include "types.h"

extern Projectile gProjectiles[MAX_PROJECTILES];

void update_input_buffer(Character *ent, u16 pad);
u16  get_input_at(Character *ent, u8 frames_back);
void check_double_tap(Character *ent, u16 pad, u16 pad_down);
int  check_motion_special(Character *ent);
int  check_desperation(Character *ent);

int  check_overlap(s16 min1, s16 max1, s16 min2, s16 max2);
bool check_attack_3d(Character *attacker, Character *defender);
void apply_damage_directed(Character *target, u8 dmg, s8 knockback_dir);
void apply_damage(Character *attacker, Character *target);
void combat_check_hits(Character *entity);

void collision_init_projectiles(void);
void collision_spawn_projectile(int x, int y, int velX, u8 damage, bool friendly);
void collision_update_projectiles(void);
void collision_resolve_overlap(void);

#endif
