#include "item_get_windows.hpp"
#include "item_get_text.hpp"
#include "item_presentation.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>
namespace ttyd {
namespace {
std::shared_ptr<const MessageResource> assets;
bool decode(const MessageFont& font,const char* text,ColorWindowText& out) {
 if(!text)return false;
 uint64_t color=uint64_t(1)<<32;
 for(const char* p=text;*p;) {
  if(*p=='<') {
   const char* end=std::strchr(p,'>');if(!end || end-p>128)return false;
   const std::string tag(p+1,end);
   if(tag=="/col" || tag=="/red")color=uint64_t(2)<<32;
   else if(tag=="red")color=0xe00000ff;
   else if(tag.size()>4 && tag.compare(0,4,"col ")==0) {
    const auto value=tag.substr(4);if(value.empty() || value.size()>8)return false;
    uint32_t parsed=0;
    for(unsigned char c:value){unsigned digit;
     if(c>='0' && c<='9')digit=c-'0';else if(c>='a' && c<='f')digit=c-'a'+10;
     else if(c>='A' && c<='F')digit=c-'A'+10;else return false;
     parsed=(parsed<<4)|digit;
    }
    color=parsed;
   } else return false;
   p=end+1;if(*p=='\n')++p; // Original non-R command LF suppression.
   continue;
  }
  const unsigned char c=*p++;out.plain.push_back(char(c));
  if(c=='\n')continue;
  const uint16_t code=font.mapping[c];if(code==0xffff)continue;
  if(code>=font.glyphs.size())return false;
  out.glyphColors.push_back(color);
 }
 return true;
}
}
bool compileColorWindowText(const MessageFont& font,const char* text,ColorWindowText& out) {
 ColorWindowText next;if(!decode(font,text,next))return false;
 out=std::move(next);return true;
}
bool measureColorWindowFont(const MessageFont& font,const char* text,uint32_t spacing,WindowFontMeasurement& out) {
 ColorWindowText decoded;if(!decode(font,text,decoded))return false;
 return measurePlainWindowFont(font,decoded.plain.c_str(),spacing,out);
}
bool drawColorWindowFont(const MessageFont& font,const char* text,const WindowFontStyle& style,
                        const WindowFontDrawServices& services,int32_t x,int32_t y) {
 ColorWindowText decoded;if(!decode(font,text,decoded))return false;
 return drawColorWindowFont(font,decoded,style,services,x,y);
}
bool drawColorWindowFont(const MessageFont& font,const ColorWindowText& decoded,const WindowFontStyle& style,
                        const WindowFontDrawServices& services,int32_t x,int32_t y) {
 WindowFontMeasurement checked;if(!measurePlainWindowFont(font,decoded.plain.c_str(),style.spacing,checked))return false;
 assert(services.emitQuad && services.random);
 struct Context {const ColorWindowText* decoded;const WindowFontStyle* style;const WindowFontDrawServices* services;size_t quad=0;} c{&decoded,&style,&services};
 WindowFontDrawServices adapter{&c,[](void* user,const WindowFontQuad& source){
  auto& c=*static_cast<Context*>(user);auto q=source;const size_t count=c.style->edge?5:1;
  if(c.quad%count==count-1){const uint64_t value=c.decoded->glyphColors.at(c.quad/count);const uint32_t color=value>>32==1?c.style->color:value>>32==2?c.style->palette0:uint32_t(value);q.rgba=(color&0xffffff00u)|((color&255)*c.style->alpha/255);}
  ++c.quad;c.services->emitQuad(c.services->user,q);
 },[](void* user,uint32_t bound){auto& c=*static_cast<Context*>(user);return c.services->random(c.services->user,bound);}};
 return drawPlainWindowFont(font,decoded.plain.c_str(),WindowFontConsumer::MessageDirect,style,adapter,x,y);
}
bool loadItemGetWindowAssets(const char* root) {
 if(!root)return false;
 MessageResource next;
 if(!next.load((std::string(root)+"/messages/global.txt").c_str()) || !next.find("msg_window_item_get"))return false;
 assets=std::make_shared<MessageResource>(std::move(next));return true;
}
bool ItemGetWindows::prepare(const MessageFont& font) {
 if(retained() || !assets)return false;
 const auto* item=itemPresentation(95);if(!item)return false;
 const char* name=assets->find(item->name()),*description=assets->find(item->description());
 if(!name || !description)return false;
 auto title=expandUsItemGetText(assets->find("msg_window_item_get"),name);
 WindowFontMeasurement measured;
 ColorWindowText titleFont,descriptionFont;
 if(!compileColorWindowText(font,title.c_str(),titleFont) || !compileColorWindowText(font,description,descriptionFont) ||
    !measurePlainWindowFont(font,titleFont.plain.c_str(),2,measured) || !measurePlainWindowFont(font,descriptionFont.plain.c_str(),2,measured))return false;
 titleFont_=std::move(titleFont);descriptionFont_=std::move(descriptionFont);
 messages_=assets;font_=&font;title_=std::move(title);description_=description;displaySlots_=0;windows_={};entries_={};return true;
}
bool ItemGetWindows::retained() const {for(const auto& w:windows_)if(w.sequence.flags&1)return true;return false;}
int32_t ItemGetWindows::enter(uint32_t definition,const char* name) {
 if(!font_ || !name || !*name || (definition!=0x80313dc8 && definition!=0x80313df0))return -1;
 for(unsigned slot=0;slot<2;++slot)if(!(windows_[slot].sequence.flags&1)) {
  auto& w=windows_[slot];const bool help=definition==0x80313df0;
  // Entry overwrites these fields, not the old transform/alpha state.
  w.sequence.flags=1;w.sequence.action=0;w.sequence.clock=0;w.sequence.sequence=0;
  w.rectangle=help?ItemSelectionRectangle{-240,-130,500,80}:ItemSelectionRectangle{-175,120,350,42};
  w.decoration=0;w.helpPage=w.helpLines=0;help_[slot]=help;entries_[slot]=name;
  // Active Open resets help publication; transition bit is set by Seq later.
  w.sequence.action=1;w.sequence.clock=0;w.helpText=nullptr;return int32_t(slot);
 }
 return -1;
}
void ItemGetWindows::closeAutoDelete(int32_t slot) {
 if(slot<0 || slot>=2)return;
 auto& w=windows_[size_t(slot)];if(!(w.sequence.flags&1))return;
 w.sequence.action=2;w.sequence.clock=0;w.sequence.flags|=8;
}
int32_t ItemGetWindows::action(int32_t slot) const {
 assert(slot>=0 && slot<2);return (windows_[size_t(slot)].sequence.flags>>2)&1;
}
void ItemGetWindows::managerTick(uint32_t repeat,const ItemSelectionSequenceServices& services) {
 displaySlots_=0;
 for(unsigned slot=0;slot<2;++slot) {
  auto& w=windows_[slot];if(!(w.sequence.flags&1))continue;
  tickItemSelectionSequence(w.sequence,services);
  if(help_[slot] && !(w.sequence.flags&4)) {
   if(w.helpText){WindowFontMeasurement m;const bool valid=measurePlainWindowFont(*font_,descriptionFont_.plain.c_str(),2,m);assert(valid);w.helpLines=int32_t(m.lineFeeds)+1;}
   if(w.helpLines>2) {
    if(repeat&0x100)w.helpPage=std::max<int32_t>(w.helpPage-2,0);
    else if((repeat&0x200) && (w.helpLines+1)/2-1>(w.helpPage+1)/2)w.helpPage+=2;
   }
  }
  if(w.sequence.flags&2)displaySlots_|=1u<<slot;
 }
}
ItemGetWindowDisplay ItemGetWindows::display(unsigned slot) {
 assert(slot<2 && (displaySlots_&(1u<<slot)));auto& w=windows_[slot];ItemGetWindowDisplay out;
 out.frame=w;out.help=help_[slot];
 if(help_[slot]) {
  if(w.sequence.flags&4)return out;
  w.helpText=description_;out.text=w.helpText;out.compiled=&descriptionFont_;out.draw=itemSelectionHelpDraw(w,palette0);
 } else {
  WindowFontMeasurement m;const bool valid=measurePlainWindowFont(*font_,titleFont_.plain.c_str(),2,m);assert(valid);
  if(int32_t(m.width)+20>w.rectangle.width){w.rectangle.width=int32_t(m.width)+20;w.rectangle.x=int32_t(-float(w.rectangle.width)/2);}
  w.rectangle.height=42+24*int32_t(m.lineFeeds);
  if(w.sequence.flags&4)return out;
  out.text=title_.c_str();out.compiled=&titleFont_;out.draw={float(int32_t(-float(m.width)/2)),float(w.rectangle.y-8),1,1,0,0x000000ff,false};
 }
 out.content=true;return out;
}
bool CoinGetWindow::cacheTitle(int32_t amount) {
 const auto* source=messages_->find("msg_window_coin_get");if(!source)return false;
 auto title=expandUsCoinGetText(source,amount);ColorWindowText compiled;WindowFontMeasurement measured;
 if(!compileColorWindowText(*font_,title.c_str(),compiled) || !measurePlainWindowFont(*font_,compiled.plain.c_str(),2,measured))return false;
 title_=std::move(title);compiled_=std::move(compiled);measured_=measured;amount_=amount;return true;
}
bool CoinGetWindow::prepare(const MessageFont& font,int32_t amount) {
 if(retained() || !assets)return false;
 CoinGetWindow next;next.messages_=assets;next.font_=&font;if(!next.cacheTitle(amount))return false;
 *this=std::move(next);return true;
}
int32_t CoinGetWindow::enter(uint32_t definition,const int32_t* parameter) {
 if(!font_ || title_.empty() || !parameter || retained() || definition!=0x80415e84)return -1;
 auto& sequence=window_.sequence;
 sequence.flags=1;sequence.action=1;sequence.clock=0;sequence.sequence=0;
 window_.rectangle={-175,120,350,42};window_.decoration=0;
 parameter_=parameter;displayed_=false;return 0;
}
void CoinGetWindow::closeAutoDelete(int32_t slot) {
 if(slot!=0 || !retained())return;
 window_.sequence.action=2;window_.sequence.clock=0;window_.sequence.flags|=8;
}
int32_t CoinGetWindow::action(int32_t slot) const {
 assert(slot==0);return (window_.sequence.flags>>2)&1;
}
void CoinGetWindow::managerTick(const ItemSelectionSequenceServices& services) {
 displayed_=false;if(!retained())return;
 tickItemSelectionSequence(window_.sequence,services);
 displayed_=(window_.sequence.flags&2)!=0;
 if(!retained())parameter_=nullptr;
}
ItemGetWindowDisplay CoinGetWindow::display() {
 assert(displayed_);ItemGetWindowDisplay out;out.frame=window_;
 // Source coingetDisp tests this before reading amount or measuring/mutating
 // geometry. Unlike item95, its text has no sequence transition-bit gate.
 if(!parameter_ || !parameter_[1])return out;
 if(parameter_[0]!=amount_ && !cacheTitle(parameter_[0]))return out;
 const int32_t width=uint16_t(measured_.width);
 if(width+20>window_.rectangle.width){window_.rectangle.width=width+20;window_.rectangle.x=int32_t(-float(width+20)/2);}
 window_.rectangle.height=42+24*int32_t(measured_.lineFeeds);
 out.text=title_.c_str();out.compiled=&compiled_;
 out.draw={float(int32_t(-float(width)/2)),float(window_.rectangle.y-8),1,1,0,0x000000ff,false};
 out.content=true;return out;
}
}
