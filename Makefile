ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: bitmaps all

export ROMNAME := legendarios

all: bitmaps $(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx

GFXCONV := gfx4snes

assets/hud/hud.pic assets/hud/hud.pal assets/hud/hud.map: assets/hud/hud.bmp
	@echo convert hud ... $(notdir $@)
	$(GFXCONV) -s 8 -o 0 -u 4 -e 0 -p -m -g -t bmp -i $<

assets/hud/hud_font.pic assets/hud/hud_font.pal: assets/hud/hud_font.bmp
	@echo convert hud font ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t bmp -i $<

assets/backgrounds/stage1/stage1_bg1.pic assets/backgrounds/stage1/stage1_bg1.map assets/backgrounds/stage1/stage1_bg1.pal: assets/backgrounds/stage1/stage1_bg1.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -y -s 8 -o 16 -u 16 -e 2 -p -m -t bmp -i $<

assets/backgrounds/stage1/stage1_bg2.pic assets/backgrounds/stage1/stage1_bg2.map assets/backgrounds/stage1/stage1_bg2.pal: assets/backgrounds/stage1/stage1_bg2.bmp
	@echo convert bitmap ... $(notdir $<)
	$(GFXCONV) -g -y -s 8 -o 16 -u 16 -e 4 -p -m -t bmp -i $<

assets/build/sprites/guy/idle/guy_idle.pic assets/build/sprites/guy/idle/guy_idle.pal: assets/build/sprites/guy/idle/guy_idle.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/walk/guy_walk.pic assets/build/sprites/guy/walk/guy_walk.pal: assets/build/sprites/guy/walk/guy_walk.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/punch/guy_punch.pic assets/build/sprites/guy/punch/guy_punch.pal: assets/build/sprites/guy/punch/guy_punch.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/kick/guy_kick.pic assets/build/sprites/guy/kick/guy_kick.pal: assets/build/sprites/guy/kick/guy_kick.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/damage/guy_damage.pic assets/build/sprites/guy/damage/guy_damage.pal: assets/build/sprites/guy/damage/guy_damage.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/grab/guy_grab.pic assets/build/sprites/guy/grab/guy_grab.pal: assets/build/sprites/guy/grab/guy_grab.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/jump/guy_jump.pic assets/build/sprites/guy/jump/guy_jump.pal: assets/build/sprites/guy/jump/guy_jump.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/dash/guy_dash.pic assets/build/sprites/guy/dash/guy_dash.pal: assets/build/sprites/guy/dash/guy_dash.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/dash_kick/guy_dash_kick.pic assets/build/sprites/guy/dash_kick/guy_dash_kick.pal: assets/build/sprites/guy/dash_kick/guy_dash_kick.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/super/guy_super.pic assets/build/sprites/guy/super/guy_super.pal: assets/build/sprites/guy/super/guy_super.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/super/guy_superpreto.pic assets/build/sprites/guy/super/guy_superpreto.pal: assets/build/sprites/guy/super/guy_superpreto.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/jump_kick/guy_jump_kick.pic assets/build/sprites/guy/jump_kick/guy_jump_kick.pal: assets/build/sprites/guy/jump_kick/guy_jump_kick.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/low_kick/guy_low_kick.pic assets/build/sprites/guy/low_kick/guy_low_kick.pal: assets/build/sprites/guy/low_kick/guy_low_kick.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/elbow_drop/guy_elbow_drop.pic assets/build/sprites/guy/elbow_drop/guy_elbow_drop.pal: assets/build/sprites/guy/elbow_drop/guy_elbow_drop.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.pic assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.pal: assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/sprites/enemies/hunter/hunter.pic assets/sprites/enemies/hunter/hunter.pal: assets/sprites/enemies/hunter/hunter.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/sprites/enemies/may/may.pic assets/sprites/enemies/may/may.pal: assets/sprites/enemies/may/may.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

assets/sprites/enemies/andore/andore.pic assets/sprites/enemies/andore/andore.pal: assets/sprites/enemies/andore/andore.png
	@echo convert sprite ... $(notdir $@)
	$(GFXCONV) -s 16 -o 0 -u 16 -p -t png -i $<

bitmaps : \
	assets/hud/hud.pic \
	assets/hud/hud.pal \
	assets/hud/hud.map \
	assets/backgrounds/stage1/stage1_bg1.pic \
	assets/backgrounds/stage1/stage1_bg1.map \
	assets/backgrounds/stage1/stage1_bg1.pal \
	assets/backgrounds/stage1/stage1_bg2.pic \
	assets/backgrounds/stage1/stage1_bg2.map \
	assets/backgrounds/stage1/stage1_bg2.pal
