.include "hdr.asm"

.section ".rodata1" superfree

; ── Backgrounds + HUD + Guy palette + idle ──
BG1_tiles:
.incbin "assets/backgrounds/stage1/stage1_bg1.pic"
BG1_tiles_end:

BG1_pal:
.incbin "assets/backgrounds/stage1/stage1_bg1.pal"
BG1_pal_end:

BG1_map:
.incbin "assets/backgrounds/stage1/stage1_bg1.map"
BG1_map_end:

BG2_tiles:
.incbin "assets/backgrounds/stage1/stage1_bg2.pic"
BG2_tiles_end:

BG2_pal:
.incbin "assets/backgrounds/stage1/stage1_bg2.pal"
BG2_pal_end:

HUD_tiles:
.incbin "assets/hud/hud.pic"
HUD_tiles_end:

HUD_pal:
.incbin "assets/hud/hud.pal"
HUD_pal_end:

HUD_map:
.incbin "assets/hud/hud.map"
HUD_map_end:

guy_palette:
.incbin "assets/build/sprites/guy/idle/guy_idle.pal"
guy_palette_end:

guy_idle_tiles:
.incbin "assets/build/sprites/guy/idle/guy_idle.pic"
guy_idle_tiles_end:

.ends

; BG2_map moved to its own section to keep rodata1 under 32KB
.section ".rodata1b" superfree

BG2_map:
.incbin "assets/backgrounds/stage1/stage1_bg2.map"
BG2_map_end:

.ends

.section ".rodata2" superfree

guy_walk_tiles:
.incbin "assets/build/sprites/guy/walk/guy_walk.pic"
guy_walk_tiles_end:

.ends

.section ".rodata3" superfree

guy_punch_tiles:
.incbin "assets/build/sprites/guy/punch/guy_punch.pic"
guy_punch_tiles_end:

guy_kick_tiles:
.incbin "assets/build/sprites/guy/kick/guy_kick.pic"
guy_kick_tiles_end:

.ends

.section ".rodata4" superfree

guy_damage_tiles:
.incbin "assets/build/sprites/guy/damage/guy_damage.pic"
guy_damage_tiles_end:

guy_grab_tiles:
.incbin "assets/build/sprites/guy/grab/guy_grab.pic"
guy_grab_tiles_end:

guy_jump_tiles:
.incbin "assets/build/sprites/guy/jump/guy_jump.pic"
guy_jump_tiles_end:

.ends

.section ".rodata5" superfree

guy_jump_kick_tiles:
.incbin "assets/build/sprites/guy/jump_kick/guy_jump_kick.pic"
guy_jump_kick_tiles_end:

guy_dash_tiles:
.incbin "assets/build/sprites/guy/dash/guy_dash.pic"
guy_dash_tiles_end:

.ends

.section ".rodata6" superfree

guy_dash_kick_tiles:
.incbin "assets/build/sprites/guy/dash/guy_dash_kick.pic"
guy_dash_kick_tiles_end:

.ends

.section ".rodata7" superfree

guy_low_kick_tiles:
.incbin "assets/build/sprites/guy/low_kick/guy_low_kick.pic"
guy_low_kick_tiles_end:

guy_elbow_drop_tiles:
.incbin "assets/build/sprites/guy/elbow_drop/guy_elbow_drop.pic"
guy_elbow_drop_tiles_end:

.ends

.section ".rodata8" superfree

guy_senpuukyaku_tiles:
.incbin "assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.pic"
guy_senpuukyaku_tiles_end:

.ends

.section ".rodata9" superfree

guy_super_tiles:
.incbin "assets/build/sprites/guy/super/guy_super.pic"
guy_super_tiles_end:

.ends

.section ".rodata10" superfree

guy_superpreto_tiles:
.incbin "assets/build/sprites/guy/super/guy_superpreto.pic"
guy_superpreto_tiles_end:

.ends

.section ".rodata11" superfree

hunter_idle_tiles:
.incbin "assets/sprites/enemies/hunter/hunter.pic"
hunter_idle_tiles_end:

hunter_palette:
.incbin "assets/sprites/enemies/hunter/hunter.pal"
hunter_palette_end:

.ends

.section ".rodata12" superfree

may_idle_tiles:
.incbin "assets/sprites/enemies/may/may.pic"
may_idle_tiles_end:

may_palette:
.incbin "assets/sprites/enemies/may/may.pal"
may_palette_end:

.ends

.section ".rodata13" superfree

andore_idle_tiles:
.incbin "assets/sprites/enemies/andore/andore.pic"
andore_idle_tiles_end:

andore_palette:
.incbin "assets/sprites/enemies/andore/andore.pal"
andore_palette_end:

.ends
