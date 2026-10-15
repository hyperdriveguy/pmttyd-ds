#include "native_source_resource_path_formatter.hpp"
#include "source_mario_dvd_root.hpp"
#include <cassert>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t word(const std::vector<uint8_t>&b,unsigned a){assert(a+4<=b.size());return uint32_t(b[a])<<24|uint32_t(b[a+1])<<16|uint32_t(b[a+2])<<8|b[a+3];}
const std::map<uint32_t,uint8_t>&original(){
 static const auto result=[](){std::map<uint32_t,uint8_t>m;std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);auto raw=[&](uint32_t a,unsigned n){std::vector<uint8_t>b(n);iso.seekg(a);iso.read(reinterpret_cast<char*>(b.data()),n);assert(iso);return b;};assert(raw(0,8)==std::vector<uint8_t>({'G','8','M','E','0','1',0,0}));const auto dol=word(raw(0x420,4),0);const auto h=raw(dol,256);
  for(auto span:{std::pair<uint32_t,unsigned>{0x8041f494,2},{0x802c3094,20},{0x802c2b30,0x8b},{0x80416038,28}}){bool found=false;for(unsigned i=0;i<18;++i){const auto base=word(h,0x48+4*i),length=word(h,0x90+4*i);if(base<=span.first&&uint64_t(span.first)+span.second<=uint64_t(base)+length){assert(!found);found=true;const auto b=raw(dol+word(h,4*i)+span.first-base,span.second);for(unsigned j=0;j<span.second;++j)m[span.first+j]=b[j];}}assert(found);}return m;
 }();return result;
}
struct Fixture {
 NativeSourceResourcePathFormatter formatter;SourceFontPath font;SourceWindowTexturePath window;
 std::map<uint32_t,uint8_t>memory=original();std::vector<uint32_t>reads;
 std::shared_ptr<const void>lifetime=std::make_shared<int>(1);
 bool live=true,immutable=true,isolated=true,reenter=false,retireAtOutput=false;
 size_t holdAt=size_t(-1);unsigned attempts=0,destinationChecks=0;uint32_t inputBudget=256,outputCapacity=256;
 SourceFontSetupCall fontCall;SourceWindowTextureSetupCall windowCall;const SourceFontPath*extraFont=nullptr;const SourceWindowTexturePath*extraWindow=nullptr;
 Fixture(){font.cells={{0,0xaa},{80,0xbb},{400,0xcc}};window.cells=font.cells;select(0);}
 uint32_t sourceWord(uint32_t at){uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|memory.at(at+i);return v;}
 void select(unsigned kind,uint32_t name=0x802c2b44){fontCall={};fontCall.function=0x8026a0b8;fontCall.callsite=kind==1?0x80075094:0x80075048;fontCall.integers={kind==1?0x802c2bacu:0x802c2ba0u,sourceMarioDvdRoot(OriginalMarioDvdSourceR2),name};fontCall.formatDestination=&font;windowCall={};windowCall.function=0x8026a0b8;windowCall.callsite=0x8007fc30;windowCall.arguments={0x802c3094,sourceMarioDvdRoot(OriginalMarioDvdSourceR2),0,0,0};windowCall.count=2;windowCall.destination=&window;}
 NativeResourcePathFormatterServices services(){NativeResourcePathFormatterServices s;s.user=this;s.lifetime=lifetime;s.stringBudget=inputBudget;s.outputCapacity=outputCapacity;
  s.current=[](void*p){return static_cast<Fixture*>(p)->live;};
  s.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&f=*static_cast<Fixture*>(p);if(f.reads.size()==f.holdAt&&f.attempts++<2)return {};auto it=f.memory.find(a);if(it==f.memory.end())return {};f.reads.push_back(a);return it->second;};
  s.immutableInput=[](void*p,uint32_t,uint32_t){return static_cast<Fixture*>(p)->immutable;};
  s.privateDestination=[](void*p,NativeResourcePathKind kind,const void*destination,uint32_t capacity){auto&f=*static_cast<Fixture*>(p);++f.destinationChecks;if(f.reenter){assert(!f.formatter.request(f.fontCall));assert(!f.formatter.request(f.windowCall));assert(!f.formatter.reset());assert(!f.formatter.bind(f.services()));}if(f.retireAtOutput&&f.destinationChecks==3)f.formatter.retire();return f.isolated&&capacity==f.outputCapacity&&(kind==NativeResourcePathKind::Font?(destination==&f.font||destination==f.extraFont):(destination==&f.window||destination==f.extraWindow));};return s;
 }
 void bind(){assert(formatter.bind(services()));assert(!formatter.bind(services()));}
 std::optional<uint32_t>request(unsigned kind){return kind==2?formatter.request(windowCall):formatter.request(fontCall);}
 uint32_t run(unsigned kind){for(unsigned i=0;i<1000;++i){auto n=request(kind);if(n)return *n;}assert(false);return 0;}
 std::string text(unsigned kind){const auto&cells=kind==2?window.cells:font.cells;std::string s;for(uint32_t i=0;i<outputCapacity;++i){auto it=cells.find(i);assert(it!=cells.end());if(!it->second)return s;s+=char(it->second);}assert(false);return {};}
};
void originalsAndHolds(){for(unsigned kind=0;kind<3;++kind)for(uint32_t name:{0x802c2b30u,0x802c2b44u,0x802c2b58u}){
 Fixture immediate;immediate.select(kind,name);immediate.bind();const auto count=immediate.run(kind);const auto reads=immediate.reads.size();const auto text=immediate.text(kind);assert(count==text.size());assert(text.rfind("./",0)==0);assert(kind==2?text=="./f/msgWindow.tpl":text.find("papermarioset_")!=std::string::npos);assert(!immediate.formatter.active());
 for(size_t at=0;at<reads;++at){Fixture held;held.select(kind,name);held.bind();held.holdAt=at;const auto before=kind==2?held.window.cells:held.font.cells;assert(!held.request(kind));assert((kind==2?held.window.cells:held.font.cells)==before);assert(!held.request(kind));assert((kind==2?held.window.cells:held.font.cells)==before);assert(held.run(kind)==count&&held.reads==immediate.reads&&held.text(kind)==text&&held.attempts==3);}
 assert((kind==2?immediate.window.cells:immediate.font.cells).at(400)==0xcc);
 }}
void suffixAndRawBytes(){Fixture f;f.select(1);f.bind();f.run(1);const auto previous=f.font.cells;f.select(0);f.run(0);const auto n=f.text(0).size();for(const auto&cell:previous)if(cell.first>n)assert(f.font.cells.at(cell.first)==cell.second);assert(f.font.cells.at(uint32_t(n))==0);assert(!f.font.byte(399));
 Fixture raw;raw.memory[0x9000]=0x80;raw.memory[0x9001]=0xff;raw.memory[0x9002]=0;raw.select(0,0x9000);raw.bind();raw.run(0);const auto s=raw.text(0);assert(uint8_t(s[4])==0x80&&uint8_t(s[5])==0xff); // plain source bytes, no Unicode conversion
}
void unavailable(){
 Fixture unknown;unknown.bind();const auto before=unknown.font.cells;unknown.memory.erase(0x8041f495);assert(!unknown.request(0)&&unknown.formatter.pendingAddress()==0x8041f495&&unknown.font.cells==before);unknown.memory[0x8041f495]=0;assert(unknown.run(0)==unknown.text(0).size());
 Fixture null;null.select(0,0);null.bind();assert(!null.request(0)&&null.font.cells==before);
 Fixture input;input.immutable=false;input.bind();assert(!input.request(0)&&input.reads.empty()&&input.font.cells==before);input.immutable=true;assert(input.run(0));
 Fixture alias;alias.isolated=false;alias.bind();assert(!alias.request(0)&&alias.reads.empty()&&alias.font.cells==before);
 Fixture overflow;overflow.outputCapacity=2;overflow.bind();assert(!overflow.request(0)&&overflow.font.cells==before);
 Fixture measured;measured.bind();const auto count=measured.run(0);Fixture exact;exact.outputCapacity=count+1;exact.bind();assert(exact.run(0)==count);Fixture noNul;noNul.outputCapacity=count;noNul.bind();assert(!noNul.request(0)&&noNul.font.cells==before);
 Fixture unterminated;unterminated.inputBudget=3;unterminated.bind();assert(!unterminated.request(0)&&unterminated.reads.size()==3&&unterminated.font.cells==before);
 Fixture wrapped;wrapped.select(0,0xffffffff);wrapped.memory[0xffffffff]='A';wrapped.bind();assert(!wrapped.request(0)&&wrapped.font.cells==before);assert(wrapped.formatter.pendingAddress()==0xffffffff);
 Fixture altered;altered.memory[0x802c2ba0]='!';altered.bind();assert(!altered.request(0)&&altered.font.cells==before);
 Fixture missing;auto s=missing.services();s.privateDestination=nullptr;assert(!missing.formatter.bind(s));s=missing.services();s.byte=nullptr;assert(!missing.formatter.bind(s));s=missing.services();s.stringBudget=0;assert(!missing.formatter.bind(s));s=missing.services();s.immutableInput=nullptr;assert(!missing.formatter.bind(s));s=missing.services();s.stringBudget=0xffffffffu;assert(!missing.formatter.bind(s));
}
void identityAndLifetime(){Fixture f;f.bind();f.holdAt=2;assert(!f.request(0));const auto reads=f.reads;auto q=f.fontCall;q.integers[1]++;assert(!f.formatter.request(q)&&f.reads==reads);SourceFontPath other;q=f.fontCall;q.formatDestination=&other;assert(!f.formatter.request(q)&&f.reads==reads);q=f.fontCall;q.reals={0};assert(!f.formatter.request(q));auto w=f.windowCall;w.arguments[2]=1;assert(!f.formatter.request(w));f.live=false;assert(!f.request(0)&&f.formatter.stale());f.live=true;assert(!f.request(0));assert(f.formatter.reset()&&!f.request(0));
 Fixture recursive;recursive.bind();recursive.reenter=true;assert(recursive.run(0));Fixture retire;retire.bind();retire.retireAtOutput=true;const auto before=retire.font.cells;assert(!retire.request(0)&&retire.formatter.stale()&&retire.font.cells==before);
}
// Actual source parents reach this finite adapter with their real typed local
// buffers, then hold at real missing DVD Open. Fixture entry/global/archive
// conditions are explicit; no coldprocess/fullSetup/file readiness is claimed.
struct Parent {
 Fixture f;SourceWindowTextureSetupCursor window;SourceFontSetupCursor font;
 SourceWindowTextureSetupServices ws;SourceFontSetupServices fs;
 unsigned roots=0,formats=0,opens=0;std::string opened;
 Parent(){f.bind();ws.objects.gx.user=this;ws.objects.gx.lifetime=f.lifetime;ws.objects.gx.current=[](void*p){return static_cast<Parent*>(p)->f.live;};ws.objects.gx.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=static_cast<Parent*>(p)->f;uint32_t n=0;for(unsigned i=0;i<4;++i){const auto it=f.memory.find(a+i);if(it==f.memory.end())return {};n=n<<8|it->second;}return n;};ws.objects.gx.store=[](void*p,uint32_t a,uint32_t n,uint32_t v){auto&f=static_cast<Parent*>(p)->f;for(unsigned i=0;i<n;++i)f.memory[a+i]=uint8_t(v>>(8*(n-i-1)));return true;};
  ws.request=[](void*p,const SourceWindowTextureSetupCall&q)->std::optional<uint32_t>{auto&s=*static_cast<Parent*>(p);if(q.function==0x800782e4)return 0;if(q.function==0x80007388){++s.roots;return sourceMarioDvdRoot(OriginalMarioDvdSourceR2);}if(q.function==0x8026a0b8){++s.formats;s.f.windowCall=q;if(!s.f.extraWindow)s.f.extraWindow=q.destination;assert(s.f.extraWindow==q.destination);return s.f.formatter.request(q);}if(q.function==0x80144e88){++s.opens;s.opened.clear();assert(q.path);for(uint32_t i=0;;++i){const auto b=q.path->byte(i);assert(b);if(!*b)break;s.opened+=char(*b);}return {};}return {};};
  fs.user=this;fs.lifetime=f.lifetime;fs.current=ws.objects.gx.current;fs.actors.memory.user=this;fs.actors.memory.word=[](void*p,uint32_t a){return static_cast<Parent*>(p)->f.sourceWord(a);};fs.actors.memory.half=[](void*p,uint32_t a){auto&f=static_cast<Parent*>(p)->f;return uint16_t(uint16_t(f.memory.at(a))<<8|f.memory.at(a+1));};fs.actors.memory.storeWord=[](void*p,uint32_t a,uint32_t v){auto&f=static_cast<Parent*>(p)->f;for(unsigned i=0;i<4;++i)f.memory[a+i]=uint8_t(v>>(24-8*i));};fs.actors.memory.storeByte=[](void*p,uint32_t a,uint8_t v){static_cast<Parent*>(p)->f.memory[a]=v;};for(unsigned i=0;i<4;++i)f.memory[0x8041e978+i]=0;
  fs.request=[](void*p,const SourceFontSetupCall&q)->std::optional<uint32_t>{auto&s=*static_cast<Parent*>(p);if(q.function==0x80007388){++s.roots;return sourceMarioDvdRoot(OriginalMarioDvdSourceR2);}if(q.function==0x8026a0b8){++s.formats;s.f.fontCall=q;if(!s.f.extraFont)s.f.extraFont=q.formatDestination;assert(s.f.extraFont==q.formatDestination);return s.f.formatter.request(q);}if(q.function==0x80144e88){++s.opens;s.opened.clear();assert(q.path);for(uint32_t i=0;;++i){const auto b=q.path->byte(i);assert(b);if(!*b)break;s.opened+=char(*b);}return {};}return {};};
  // The proof recognizes only these actual live parents' private buffers,
  // exposed by their real typed original child callsites within this scope.
 }
};
void parentHolds(){Parent window;for(unsigned i=0;i<3;++i){assert(!resumeSourceWindowTextureSetup(window.ws,window.window));assert(window.window.pending()==0x80144e88&&window.roots==1&&window.formats==1&&window.opens==i+1&&window.opened=="./f/msgWindow.tpl");}Parent font;for(unsigned i=0;i<3;++i){assert(!resumeSourceFontSetup(1,font.fs,font.font));assert(font.font.pending()==0x80144e88&&font.roots==1&&font.formats==1&&font.opens==i+1&&font.opened=="./f/papermarioset_US.bfn");}}
}
int main(){originalsAndHolds();suffixAndRawBytes();unavailable();identityAndLifetime();parentHolds();std::cout<<"native finite resource formatter tests PASS\n";return 0;}
