#include "platform.hpp"
#include "battle_menu_layout.hpp"
#include "message_icons.hpp"
#include "harbor_story.hpp"
#include <cstdio>
#include <algorithm>
namespace ttyd {
// Native resource fallback maxHP 10 is harness initialization. Pouch offsets
// 70/72/74/76(hex) are HP/maxHP/FP/maxFP halfwords. Packed Mario words below
// are emulated battle-record addresses, never dereferenced as native pointers.
void Renderer::battleHud(const Game& g) {
    if(g.trioBattleActive && !g.trioAttack.statusWindowVisible())return;
    if(g.harborTutorial.running() && !g.harborTutorial.statusVisible)return;
    int hp=g.progress.hp,maxHp=10,fp=0,maxFp=0;
    if(g.progress.originalPouch) {
        hp=g.progress.originalPouch->half(0x70);maxHp=g.progress.originalPouch->half(0x72);
        fp=g.progress.originalPouch->half(0x74);maxFp=g.progress.originalPouch->half(0x76);
    }
    if(g.harborTutorial.running()) {
        hp=g.harborTutorial.actors[1].hp;maxHp=g.harborTutorial.actors[1].maxHp;
    }
    // Read the executing packed Mario owner: HP/maxHP and shared FP must not
    // lag until the field-pouch export at battle teardown.
    uint32_t limits=0,resources=0,flower=0;
    auto read=[&](uint32_t at,uint32_t& value){return g.harborTutorial.running()?
        g.harborTutorial.readPackedWord(at,value):g.trioAttack.readWord(at,value);};
    // Unit+108 limits high 16=maxHP; +10c high 16=HP/low 16=maxFP;
    // +110 low 16=FP. Signed16 extraction retains uninitialized resource values.
    if(read(0x84101108,limits) && read(0x8410110c,resources) && read(0x84101110,flower)) {
        maxHp=int16_t(limits>>16);hp=int16_t(resources>>16);
        maxFp=int16_t(resources&65535);fp=int16_t(flower&65535);
    }
    // Source statusGetValue reads coins and Star Points from the pouch in
    // both field and battle modes. The executing VM owns changes (including
    // reward counting) before its eventual field export; never freeze these
    // counters at the field snapshot once a packed pouch exists.
    int coins=-1,starPoints=-1;
    if(g.progress.originalPouch) {
        coins=g.progress.originalPouch->half(0x78);
        starPoints=g.progress.originalPouch->half(0x96);
    }
    uint32_t pouch=0;
    if(read(0x8041eb00,pouch) && pouch) {
        uint32_t coinPair=0,pointPair=0;
        coins=read(pouch+0x78,coinPair)?int16_t(coinPair>>16):-1;
        starPoints=read(pouch+0x94,pointPair)?int16_t(pointPair&65535):-1;
    }
    auto icon=[&](uint32_t id,float x,float y,float scale=.5f) {
        const auto* art=messageIcon(id);if(!art)return;
        messageIconImage(id,g.ticks,x,y+art->frame(g.ticks).height*scale*.5f,scale,255);
    };
    auto number=[&](int value,float right,float y,bool small) {
        // Original suu ji/suuji_s tables: alternate large/small IDs, one digit
        // at a time, right aligned. Negative/uninitialized resources are hidden.
        if(value<0)return;
        // Native cap six decimal digits; icon 481+2*digit is large, next ID
        // is small. Right-aligned pitches 10/8 are native pixels.
        value=std::min(value,999999);
        do {icon(481+2*(value%10)+(small?1:0),right,y);right-=small?8:10;value/=10;}while(value);
    };
    // Native half-size/crop of the original status-window art. Positions are
    // adapted to400x240; slide/blink/AP-bar callbacks remain separate owners.
    // Original HP group uses IDs 472/427/480; FP group 473/441/480.
    // Coordinates below are native UI layout; they do not implement source
    // status slide/blink choreography.
    icon(472,102,20);icon(427,67,20);icon(480,110,17);
    number(hp,99,20,false);number(maxHp,127,20,true);
    if(maxFp>0) {
        icon(473,192,20);icon(441,158,20);icon(480,200.5f,17);
        number(fp,189,20,false);number(maxFp,217,20,true);
    }
    if(coins>=0 || starPoints>=0) {
        // Original green status banner, star-point symbol, coin and multiply
        // mark are already in the shared original atlas. The right-hand group
        // is a native compact reflow; no AP capacity/fill or audience count is
        // inferred from these independent pouch counters.
        icon(475,335,16,1);
        if(starPoints>=0) {
            icon(443,289,20);icon(478,301,20);
            number(starPoints,321,20,false);
        }
        if(coins>=0) {
            icon(444,339,20);icon(478,351,20);
            number(coins,378,20,false);
        }
    }
}
// Upper command reflow: actor2 has one command, others two; target mode
// reads selected index from packed candidate byte+ab7. Frame uses source UI
// units, then native half-scale. Icon 504 is the original selection hand.
// Font 512x256 atlas uses WID1 widths+spacing 2; 0xffff means unmapped byte.
void Renderer::battleMenuRows(const Game& g) {
    const auto& commands=g.trioCommands;
    const bool target=commands.state==TrioBattleCommands::State::Target;
    const int rows=target?commands.count():commands.actor==2?1:2;
    const unsigned selected=target?commands.candidates.bytes[0xab7]:commands.command;
    // Original type0 frame, US font and animated selection hand; native reflow
    // does not claim the original rotating action-class menu choreography.
    sourceWindowFrame(type0WindowFrame(-284,64,568,float(rows*60+16),10,{255,255,255,230}));
    for(int row=0;row<rows;++row) {
        const float y=BattleMenuLayout::Top+row*BattleMenuLayout::RowHeight+6;
        const auto* label=target?commands.targetName(commands.target(row)[0]):commands.commandName(row);
        // This plain-label adapter uses the unchanged source atlas, mapping,
        // WID1 glyph widths and spacing2. Tagged story text stays in msgDisp.
        // Preserve the existing half-size original art, centered in400px.
        // This is the320px list's horizontal layout translated by40px.
        float x=90;
        if(!target)if(const auto icon=commands.commandIcon(unsigned(row),g.progress.originalPouch.get())) {
            if(const auto* art=messageIcon(*icon)) {
                const float height=art->frame(g.ticks).height*.5f;
                messageIconImage(*icon,g.ticks,102,y+9+height*.5f,1,255);x=124;
            }
        }
        const auto& font=harborMessageFont();
        for(const auto* p=reinterpret_cast<const uint8_t*>(label);*p;++p) {
            const auto code=font.mapping[*p];if(code==0xffff || code>=font.glyphs.size())continue;
            const auto& glyph=font.glyphs[code];
            Tex3DS_SubTexture sub{};sub.width=glyph.width;sub.height=glyph.height;
            sub.left=glyph.x/512.f;sub.right=(glyph.x+glyph.width)/512.f;
            sub.top=1-glyph.y/256.f;sub.bottom=1-(glyph.y+glyph.height)/256.f;
            C2D_Image image{&fontTexture.texture,&sub};C2D_ImageTint tint;
            C2D_PlainImageTint(&tint,C2D_Color32(30,20,20,255),1);
            C2D_DrawParams params={{x,y+1,glyph.width*.5f,glyph.height*.5f},{0,0},.6f,0};
            C2D_DrawImage(image,&params,&tint);x+=(glyph.width+2)*.5f;
        }
        if(unsigned(row)==selected)messageIconImage(504,g.ticks,72,y+15,.5f,255);
    }
    text(target?"D-pad / Circle Pad: select   A: confirm   B: back":"D-pad / Circle Pad: select   A: targets",58,198,.38f,C2D_Color32(30,20,20,255));
}
}
