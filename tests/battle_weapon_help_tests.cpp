#include "battle_weapon_help.hpp"
#include <cassert>
#include <cstring>
#include <string>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Capture{uint32_t function,site;std::vector<uint32_t> integers;std::vector<float> reals;std::optional<uint32_t> color,guest;std::string text;std::optional<BattleWeaponHelpBytes> bytes;};
struct Fixture {
 BattleWeaponHelpCursor cursor;std::shared_ptr<const void> lifetime=std::make_shared<int>(1);
 std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> input;
 std::vector<Capture> calls;std::vector<uint32_t> reads;unsigned events=0,pauseAt=0,spills=0;
 bool alive=true,reenter=false,retire=false,nullMessage=false,allowSpill=false;
 const BattleWeaponHelpCall* held=nullptr;uint32_t key=0x6000;float x=12,y=100;
 Fixture(const std::string&text="A\nB\nC"){
  for(auto p:std::initializer_list<std::pair<uint32_t,float>>{{0x80422484,400},{0x80422488,50},{0x8042246c,10},{0x8042248c,.8f},{0x8042244c,20},{0x80422490,4},{0x80422494,28},{0x80422498,52}})words[p.first]=bits(p.second);
  words[0x80422428]=0xffffffcf;for(unsigned i=0;i<text.size();++i)input[0x7000+i]=uint8_t(text[i]);input[0x7000+text.size()]=0;
 }
 bool event(){return ++events!=pauseAt;}
 BattleWeaponHelpServices services(){BattleWeaponHelpServices s;s.user=this;s.lifetime=lifetime;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.actors.memory.user=this;
  s.actors.memory.word=[](void*p,uint32_t a){auto&f=*static_cast<Fixture*>(p);auto i=f.words.find(a);assert(i!=f.words.end());return i->second;};
  s.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&f=*static_cast<Fixture*>(p);f.reads.push_back(a);if(!f.event())return {};auto i=f.input.find(a);return i==f.input.end()?std::nullopt:std::optional<uint8_t>(i->second);};
  s.request=[](void*p,const BattleWeaponHelpCall&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(f.reenter)assert(!f.cursor.reset()&&!f.run());if(f.retire)f.alive=false;
   if(!f.event()){f.held=&q;return {};}
   if(f.held){assert(&q==f.held);f.held=nullptr;}
   Capture c{q.function,q.callsite,q.integers,q.reals,q.privateColor,q.guestText,{},{}};
   if(q.privateText){c.bytes=*q.privateText;uint32_t at=q.privateOffset;for(;;){auto b=q.privateText->byte(at++);if(!b)return {};if(!*b)break;c.text.push_back(char(*b));}}
   f.calls.push_back(c);return q.function==0x80081b8c?(f.nullMessage?0:0x7000):0;
  };
  if(allowSpill)s.stackStore=[](void*p,uint32_t offset,uint8_t){auto&f=*static_cast<Fixture*>(p);assert(offset>=204);++f.spills;return f.event();};
  return s;
 }
 bool run(){return resumePackedBattleWeaponHelp(key,x,y,services(),cursor);}
 std::vector<Capture> fonts()const{std::vector<Capture> result;for(const auto&c:calls)if(c.function==0x800773d0)result.push_back(c);return result;}
};
}
int main(){
 {Fixture f;assert(f.run()&&!f.cursor.active());auto fonts=f.fonts();assert(fonts.size()==3&&fonts[0].text=="A"&&fonts[1].text=="B"&&fonts[2].text=="C");assert(f.calls[0].color==0xffffffcf&&f.calls[0].integers==std::vector<uint32_t>{0}&&(f.calls[0].reals==std::vector<float>{12,100,400,50,10}));for(unsigned i=0;i<3;++i)assert((fonts[i].reals==std::vector<float>{32,96-24*float(i)}));assert(f.calls[4].function==0x80081b8c&&f.calls[4].integers[0]==f.key);}
 // Hold every source read/child boundary, preserve the source prefix exactly.
 {Fixture base;assert(base.run());for(unsigned event=1;event<=base.events;++event){Fixture f;f.pauseAt=event;assert(!f.run());assert(f.run()&&f.calls.size()==base.calls.size());for(unsigned i=0;i<f.calls.size();++i){const auto&a=f.calls[i];const auto&b=base.calls[i];assert(a.function==b.function&&a.site==b.site&&a.reals==b.reals&&a.text==b.text);}}}
 // A held window keeps copied private color/dimensions; later font scalar stays fresh.
 {Fixture f;f.pauseAt=1;assert(!f.run());f.words[0x80422428]=0;f.words[0x80422484]=bits(999);f.words[0x8042248c]=bits(.5f);assert(f.run()&&f.calls[0].color==0xffffffcf&&f.calls[0].reals[2]==400&&f.calls[3].reals[0]==.5f);}
 // Before nonnull message lookup completes, none of the private bytes are initialized.
 {Fixture f;f.pauseAt=5;assert(!f.run());for(auto byte:f.cursor.privateBytes()->cells)assert(!byte);assert(f.run());}
 // Strip ONE tag per outer iteration; adjacent tags preserve the second '<'.
 {Fixture f("<red><blue>X\n<foo>A<bar>B\n\x81\x82");assert(f.run());auto fonts=f.fonts();assert(fonts[0].text=="<blue>X"&&fonts[1].text=="AB"&&fonts[2].text==std::string("\x81\x82",2));}
 // First load checks '<'; the second source byte load copies freshly.
 {Fixture f("AB");f.pauseAt=7;assert(!f.run()&&f.reads.size()==2&&f.reads[0]==0x7000&&f.reads[1]==0x7000);f.input[0x7000]='Z';assert(f.run()&&f.fonts()[0].text=="ZB");}
 // Only four row heads are initialized; no unknown suffix is fabricated.
 {Fixture f("A");assert(f.run());auto fonts=f.fonts();assert(fonts[1].text.empty()&&fonts[2].text.empty());const auto&b=*fonts[0].bytes;unsigned known=0;for(auto c:b.cells)known+=c.has_value();assert(known==5&&b.byte(0)=='A'&&b.byte(1)==0&&!b.byte(2)&&b.byte(48)==0&&b.byte(144)==0);unsigned scales=0;for(const auto&c:f.calls)scales+=c.function==0x80077758;assert(scales==2);}
 // Empty lines still invoke real FontDrawString; fourth line is parsed but not drawn.
 {Fixture f("\n\n\nFOUR");assert(f.run());auto fonts=f.fonts();assert(fonts.size()==3&&fonts[0].text.empty()&&fonts[1].text.empty()&&fonts[2].text.empty()&&fonts[0].bytes->byte(144)=='F');}
 // No invented48-byte row limit or terminator: long rows overwrite row heads.
 {Fixture f(std::string(60,'x'));assert(f.run());auto fonts=f.fonts();assert(fonts[0].text.size()==60&&fonts[1].text.size()==12&&fonts[2].text.empty());}
 // Original out-of-buffer writes hold without a true source-stack alias owner.
 {Fixture f(std::string(193,'z'));assert(!f.run()&&f.cursor.pending()==0x80117d14&&f.fonts().empty());const auto*b=f.cursor.privateBytes();assert(b&&b->byte(191)=='z'&&!b->byte(192));}
 {Fixture f(std::string(193,'z'));f.allowSpill=true;assert(f.run()&&f.spills==2);auto fonts=f.fonts();assert(fonts[0].text.size()==193&&fonts[0].bytes->byte(193)==0);}
 // Unterminated markup ignores NUL while scanning to '>'; missing mapped byte holds.
 {Fixture f("<tag");assert(!f.run()&&f.cursor.pending()==0x80117cc4&&f.calls.size()==5);f.input[0x7005]='>';f.input[0x7006]='Q';f.input[0x7007]=0;assert(f.run()&&f.fonts()[0].text=="Q");}
 // Null search retains original guest fallback bytes/address; never US paraphrase.
 {Fixture f;f.nullMessage=true;assert(f.run()&&f.reads.empty());auto fonts=f.fonts();assert(fonts.size()==1&&fonts[0].guest==0x802ef788&&(fonts[0].reals==std::vector<float>{22,96}));}
 {Fixture f;auto s=f.services();s.request=nullptr;assert(!resumePackedBattleWeaponHelp(f.key,f.x,f.y,s,f.cursor)&&f.cursor.pending()==0x8007b44c);}
 {Fixture f;f.pauseAt=1;assert(!f.run());auto s=f.services();assert(!resumePackedBattleWeaponHelp(f.key,-0.f,f.y,s,f.cursor));s.lifetime=std::make_shared<int>(1);assert(!resumePackedBattleWeaponHelp(f.key,f.x,f.y,s,f.cursor));f.alive=false;assert(!f.run()&&f.cursor.stale());f.alive=true;assert(!f.run()&&f.calls.empty());assert(f.cursor.reset());f.held=nullptr;assert(f.run());}
 {Fixture f;f.reenter=true;assert(f.run());}
 {Fixture f;f.retire=true;assert(!f.run()&&f.cursor.stale()&&f.calls.size()==1);}
}
