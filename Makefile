.DEFAULT_GOAL := all
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

DEVKITARM ?= /opt/devkitpro/devkitARM
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export DEVKITPRO ?= /opt/devkitpro
export DEVKITARM ?= $(DEVKITPRO)/devkitARM
export PATH := $(DEVKITARM)/bin:$(DEVKITPRO)/tools/bin:$(PATH)
TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
# GRAPHICS is a list of directories containing graphics files
# GFXBUILD is the directory where converted graphics files will be placed
#   If set to $(BUILD), it will statically link in the converted
#   files as if they were data files.
#
# NO_SMDH: if set to anything, no SMDH file is generated.
# ROMFS is the directory which contains the RomFS, relative to the Makefile (Optional)
# APP_TITLE is the name of the app stored in the SMDH file (Optional)
# APP_DESCRIPTION is the description of the app stored in the SMDH file (Optional)
# APP_AUTHOR is the author of the app stored in the SMDH file (Optional)
# ICON is the filename of the icon (.png), relative to the project folder.
#   If not set, it attempts to use one of the following (in this order):
#     - <Project name>.png
#     - icon.png
#     - <libctru folder>/default_icon.png
#---------------------------------------------------------------------------------
TARGET		?=	ttyd-2ds
BUILD		?=	build
SOURCES		:=	$(patsubst $(TOPDIR)/%,%,$(shell find $(TOPDIR)/source -type d -print | sort))
DATA		:=	data
INCLUDES	:=	$(patsubst $(TOPDIR)/%,%,$(shell find $(TOPDIR)/include -type d -print | sort))
GRAPHICS	:=	gfx
GFXBUILD	:=	$(BUILD)
ASSET_MANIFEST ?= assets/manifest.json
SOUNDTRACK ?= switch
SWITCH_AUDIO_LIBRARY ?= local-assets/switch/audio
ROMFS		:=	romfs
#GFXBUILD	:=	$(ROMFS)/gfx

#---------------------------------------------------------------------------------
APP_TITLE := TTYD 2DS Prototype
APP_DESCRIPTION := Rogueport development slice
APP_AUTHOR := pm-port contributors

# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS	:=	-g -Wall -O2 -mword-relocations \
			-ffunction-sections \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -D__3DS__

ifeq ($(DEBUG_SHORT_BOOT),1)
CFLAGS += -DTTYD_DEBUG_SHORT_BOOT
endif

ifeq ($(DEBUG_OPENING_DEPARTURE),1)
CFLAGS += -DTTYD_DEBUG_OPENING_DEPARTURE
endif
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++17

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS	:= -lcitro2d -lcitro3d -lctru -lm

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:= $(CTRULIB)


#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PICAFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.v.pica)))
SHLISTFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.shlist)))
GFXFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.t3s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
ifeq ($(GFXBUILD),$(BUILD))
#---------------------------------------------------------------------------------
export T3XFILES :=  $(GFXFILES:.t3s=.t3x)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
export ROMFS_T3XFILES	:=	$(patsubst %.t3s, $(GFXBUILD)/%.t3x, $(GFXFILES))
export T3XHFILES		:=	$(patsubst %.t3s, $(BUILD)/%.h, $(GFXFILES))
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_SOURCES 	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES)) \
			$(PICAFILES:.v.pica=.shbin.o) $(SHLISTFILES:.shlist=.shbin.o) \
			$(addsuffix .o,$(T3XFILES))

export OFILES := $(OFILES_BIN) $(OFILES_SOURCES)

export HFILES	:=	$(PICAFILES:.v.pica=_shbin.h) $(SHLISTFILES:.shlist=_shbin.h) \
			$(addsuffix .h,$(subst .,_,$(BINFILES))) \
			$(GFXFILES:.t3s=.h)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

export _3DSXDEPS	:=	$(if $(NO_SMDH),,$(OUTPUT).smdh)

ifeq ($(strip $(ICON)),)
	icons := $(wildcard *.png)
	ifneq (,$(findstring $(TARGET).png,$(icons)))
		export APP_ICON := $(TOPDIR)/$(TARGET).png
	else
		ifneq (,$(findstring icon.png,$(icons)))
			export APP_ICON := $(TOPDIR)/icon.png
		endif
	endif
else
	export APP_ICON := $(TOPDIR)/$(ICON)
endif

ifeq ($(strip $(NO_SMDH)),)
	export _3DSXFLAGS += --smdh=$(CURDIR)/$(TARGET).smdh
endif

ifneq ($(ROMFS),)
	export _3DSXFLAGS += --romfs=$(CURDIR)/$(ROMFS)
endif

.PHONY: all clean assets-ready audio-assets audio-map test inventory

audio-map:
	@python3 tools/audio_map.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)" --switch local-assets/switch/audio

.PHONY: event-assets
event-assets:
	@python3 tools/pack_event_scripts.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)" --output romfs/events.pmevt

audio-assets:
	@python3 tools/extract_audio.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"

test:
	@cmake -S . -B build-host -DCMAKE_CXX_COMPILER=/usr/bin/g++
	@cmake --build build-host
	@ctest --test-dir build-host --output-on-failure
	@python3 -m unittest discover -s tests -p "test_*.py"

inventory:
	@python3 tools/iso_assets.py inventory /home/carson/games/roms/G8ME01.iso --output artifacts/iso-inventory.json

#---------------------------------------------------------------------------------
all: assets-ready $(BUILD) $(GFXBUILD) $(DEPSDIR) $(ROMFS_T3XFILES) $(T3XHFILES)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

assets-ready: assets-base
	@python3 tools/asset_pipeline.py build --manifest "$(ASSET_MANIFEST)"

.PHONY: assets-base assets-check assets-import
assets-check:
	@python3 tools/asset_pipeline.py check --manifest "$(ASSET_MANIFEST)"

# Explicit import only; never overwrite checked-in edits during a normal build.
assets-import: assets-base
	@python3 tools/import_editable_assets.py

assets-base:
	@python3 tools/asset_pipeline.py prepare
	@python3 tools/battle_class_surface_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/opening_map_transition_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/opening_book_page_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/harbor_tutorial_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/battle_curtain_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/battle_startup_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/harbor_event_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/opening_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/opening_title_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/opening_heading_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/opening_house_sound_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/demo_map_visibility_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/demo_effect_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/shadow_texture_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/build_assets.py
	@python3 tools/door_assets.py
	@python3 tools/message_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/font_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/message_indicator_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/message_window_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/harbor_actor_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/professor_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/professor_event_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/professor_scene_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/professor_fence_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/trio_battle_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/trio_attack_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/audience_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/underground_event_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/underground_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/message_icon_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/battle_class_icon_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/item_presentation_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/pouch_revision_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/goomba_quest_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/field_item_get_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/field_actor_assets.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)"
	@python3 tools/audio_map.py --iso "$(or $(TTYD_ISO),/home/carson/games/roms/G8ME01.iso)" --output local-assets/audio-map/native --romfs romfs

	@python3 tools/switch_soundtrack.py --soundtrack "$(SOUNDTRACK)" --library "$(SWITCH_AUDIO_LIBRARY)" --output romfs

$(BUILD):
	@mkdir -p $@

ifneq ($(GFXBUILD),$(BUILD))
$(GFXBUILD):
	@mkdir -p $@
endif

ifneq ($(DEPSDIR),$(BUILD))
$(DEPSDIR):
	@mkdir -p $@
endif

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).3dsx $(OUTPUT).smdh $(TARGET).elf $(GFXBUILD)

#---------------------------------------------------------------------------------
$(GFXBUILD)/%.t3x	$(BUILD)/%.h	:	%.t3s
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@tex3ds -i $< -H $(BUILD)/$*.h -d $(DEPSDIR)/$*.d -o $(GFXBUILD)/$*.t3x

#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# main targets
all: $(OUTPUT).3dsx
#---------------------------------------------------------------------------------
$(OUTPUT).3dsx	:	$(OUTPUT).elf $(_3DSXDEPS) $(wildcard $(TOPDIR)/romfs/*)

$(OFILES_SOURCES) : $(HFILES)

$(OUTPUT).elf	:	$(OFILES)

#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
.PRECIOUS	:	%.t3x %.shbin
#---------------------------------------------------------------------------------
%.t3x.o	%_t3x.h :	%.t3x
#---------------------------------------------------------------------------------
	$(SILENTMSG) $(notdir $<)
	$(bin2o)

#---------------------------------------------------------------------------------
%.shbin.o %_shbin.h : %.shbin
#---------------------------------------------------------------------------------
	$(SILENTMSG) $(notdir $<)
	$(bin2o)

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------

.PHONY: debug-short
debug-short:
	@$(MAKE) all DEBUG_SHORT_BOOT=1 BUILD=build-debug-short TARGET=ttyd-2ds-debug-short
