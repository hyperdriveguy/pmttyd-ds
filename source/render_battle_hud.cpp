#include "platform.hpp"
#include "battle_menu_layout.hpp"
#include "message_icons.hpp"
#include "harbor_story.hpp"
#include <cstdio>
#include <algorithm>
namespace ttyd {
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
    if(read(0x84101108,limits) && read(0x8410110c,resources) && read(0x84101110,flower)) {
        maxHp=int16_t(limits>>16);hp=int16_t(resources>>16);
        maxFp=int16_t(resources&65535);fp=int16_t(flower&65535);
    }
    auto icon=[&](uint32_t id,float x,float y,float scale=.5f) {
        const auto* art=messageIcon(id);if(!art)return;
        messageIconImage(id,g.ticks,x,y+art->frame(g.ticks).height*scale*.5f,scale,255);
    };
    auto number=[&](int value,float right,float y,bool small) {
        // Original suu ji/suuji_s tables: alternate large/small IDs, one digit
        // at a time, right aligned. Negative/uninitialized resources are hidden.
        if(value<0)return;
        value=std::min(value,999999);
        do {icon(481+2*(value%10)+(small?1:0),right,y);right-=small?8:10;value/=10;}while(value);
    };
    // Native half-size/crop of the original status-window art. Positions are
    // adapted to400x240; slide/blink/AP-bar callbacks remain separate owners.
    icon(472,102,20);icon(427,67,20);icon(480,110,17);
    number(hp,99,20,false);number(maxHp,127,20,true);
    if(maxFp>0) {
        icon(473,192,20);icon(441,158,20);icon(480,200.5f,17);
        number(fp,189,20,false);number(maxFp,217,20,true);
    }
}
void Renderer::battleMenuRows(const Game& g) {
    const auto& commands=g.trioCommands;
    const bool target=commands.state==TrioBattleCommands::State::Target;
    const int rows=target?commands.count():commands.actor==2?1:2;
    const unsigned selected=target?commands.candidates.bytes[0xab7]:commands.command;
    // Original type0 frame, US font and animated selection hand; native reflow
    // does not claim the original rotating action-class menu choreography.
    sourceWindowFrame(type0WindowFrame(-284,64,568,float(rows*60+16),10,{255,255,255,230}),true);
    for(int row=0;row<rows;++row) {
        const float y=BattleMenuLayout::Top+row*BattleMenuLayout::RowHeight+6;
        const auto* label=target?commands.targetName(commands.target(row)[0]):commands.commandName(row);
        // This plain-label adapter uses the unchanged source atlas, mapping,
        // WID1 glyph widths and spacing2. Tagged story text stays in msgDisp.
        float x=50;
        if(!target)if(const auto icon=commands.commandIcon(unsigned(row),g.progress.originalPouch.get())) {
            if(const auto* art=messageIcon(*icon)) {
                const float height=art->frame(g.ticks).height*.5f;
                messageIconImage(*icon,g.ticks,62,y+9+height*.5f,1,255);x=84;
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
        if(unsigned(row)==selected)messageIconImage(504,g.ticks,32,y+15,.5f,255);
    }
    text(target?"D-pad / Circle Pad: select   A: confirm   B: back":"D-pad / Circle Pad: select   A: targets",18,198,.38f,C2D_Color32(30,20,20,255));
}
}
