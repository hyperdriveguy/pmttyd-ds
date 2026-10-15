#include <memory>
#include "source_font_width.hpp"
#include "source_string_font.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
using namespace ttyd;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
uint32_t be32(const std::vector<uint8_t>&b,size_t a){assert(a+4<=b.size());return uint32_t(b[a])<<24|uint32_t(b[a+1])<<16|uint32_t(b[a+2])<<8|b[a+3];}
uint16_t be16(const std::vector<uint8_t>&b,size_t a){assert(a+2<=b.size());return uint16_t(uint16_t(b[a])<<8|b[a+1]);}
struct Event{uint32_t fn;std::vector<uint32_t>a;bool operator==(const Event&o)const{return fn==o.fn&&a==o.a;}};
struct Fixture{
 static constexpr uint32_t Gp=0x20000,Bfn=0x30000;
 std::vector<uint8_t>bfn;EventScriptAssets originals;std::map<uint32_t,uint32_t>words;std::vector<std::optional<uint8_t>>bytes;
 std::shared_ptr<const void>owner=std::make_shared<int>(7);SourceMatrixFontText text;SourceFontWidthCursor cursor;
 std::vector<Event>trace,ops;std::optional<Event>pending;std::function<void(Fixture&,const Event&)>onOp;
 int hold=-1,byteHold=-1;unsigned failures=0,byteFailures=0;bool alive=true,reentry=false,leaveWidth=false,leaveScale=false;
 uint32_t lineAddress=0,scanId=5,extra=3;float scale=1.5f;int32_t namedId=8;SourceFontWidthPrivate*frame=nullptr;const SourceFontWidthCall*request=nullptr;
 Fixture(){assert(originals.load("romfs/trio-attack-events.pmevt"));std::ifstream file("romfs/source-font/papermarioset_US.bfn",std::ios::binary);assert(file);bfn.assign(std::istreambuf_iterator<char>(file),{});assert(bfn.size()==66560);uint32_t at=32,info=0;for(unsigned i=0;i<be32(bfn,12);++i){if(be32(bfn,at)==0x494e4631)info=at;at+=be32(bfn,at+4);}assert(info);words={{0x8041e978,Bfn},{0x8041e9a0,Bfn+info},{0x80415fd0,Gp},{Gp+0x16c,1}};
  text.user=this;text.lifetime=owner;text.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&f=*static_cast<Fixture*>(p);if(int(a)==f.byteHold&&f.byteFailures++<2)return {};if(a>=f.bytes.size()||!f.bytes[a])return {};auto v=*f.bytes[a];f.trace.push_back({4,{a,v}});return v;};text.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto&f=*static_cast<Fixture*>(p);if(uint64_t(a)+1>=f.bytes.size()||!f.bytes[a]||!f.bytes[a+1])return {};auto v=uint16_t(uint16_t(*f.bytes[a])<<8|*f.bytes[a+1]);f.trace.push_back({6,{a,v}});return v;};setText("AB");}
 void setText(const std::string&s){bytes.clear();for(uint8_t b:s)bytes.push_back(b);bytes.push_back(uint8_t(0));}
 uint32_t word(uint32_t a){uint32_t v;if(a>=Bfn&&uint64_t(a)+4<=uint64_t(Bfn)+bfn.size())v=be32(bfn,a-Bfn);else{auto i=words.find(a);if(i!=words.end())v=i->second;else assert(originals.readWord(a,v));}trace.push_back({1,{a,v}});return v;}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.byte=[](void*p,uint32_t a){auto&f=*static_cast<Fixture*>(p);assert(a>=Bfn&&uint64_t(a)<uint64_t(Bfn)+f.bfn.size());auto v=f.bfn[a-Bfn];f.trace.push_back({3,{a,v}});return v;};a.memory.user=this;a.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};a.memory.half=[](void*p,uint32_t a){auto&f=*static_cast<Fixture*>(p);auto v=be16(f.bfn,a-Bfn);f.trace.push_back({2,{a,v}});return v;};a.memory.storeWord=[](void*p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->words[a]=v;};a.memory.storeHalf=[](void*p,uint32_t a,uint16_t v){auto&f=*static_cast<Fixture*>(p);f.words[a]=v;f.trace.push_back({5,{a,v}});};return a;}
 bool operation(uint32_t fn,std::vector<uint32_t>a){Event op{fn,std::move(a)};if(!pending){pending=op;trace.push_back(op);ops.push_back(op);if(onOp)onOp(*this,op);}else assert(*pending==op);if(reentry){assert(!cursor.reset());assert(!run());}if(int(ops.size()-1)==hold&&failures++<2)return false;pending.reset();return true;}
 SourceFontWidthServices services(){SourceFontWidthServices s;s.common.actors=actors();s.common.user=this;s.common.lifetime=owner;s.common.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.common.camera=[](void*p)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x8000f2e4,{}))return {};return 0x10000;};s.common.truncate=[](void*p,float x)->std::optional<int32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x80075714,{bits(x)}))return {};assert(std::isfinite(x)&&x>=-2147483648.f&&double(x)<2147483648.);return int32_t(x);};s.fma=[](void*p,float a,float b,float c)->std::optional<float>{auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x80075708,{bits(a),bits(b),bits(c)}))return {};return std::fma(a,b,c);};
 s.call=[](void*p,const SourceFontWidthCall&q)->std::optional<int32_t>{auto&f=*static_cast<Fixture*>(p);if(f.pending){assert(f.frame==q.frame&&f.request==&q);}else{f.frame=q.frame;f.request=&q;}std::vector<uint32_t>a{q.callsite};for(unsigned i=0;i<q.count;++i)a.push_back(q.arguments[i]);
  // Explicit test child models, not implementations of original scanf/icons.
  // Partial outputs occur ONCE before a hold, and retain identical cell owner.
  if(!f.pending&&q.function==0x8026c344){assert(q.count==7);auto&b=*q.frame;if(q.callsite==0x80075650)b.write(0x1c,4,f.scanId);else{b.write(0x20,1,'x');b.write(0x21,1,0);}if(!f.leaveScale)b.write(0xc,4,bits(f.scale));b.write(0x10,4,f.extra);b.write(0x14,4,88);b.write(0x18,4,77);}
  if(!f.pending&&q.function==0x8001a19c){assert(q.arguments[0]==0xa&&q.arguments[1]==8);if(!f.leaveWidth)q.frame->write(0xa,2,20);q.frame->write(8,2,9);}
  if(!f.operation(q.function,a))return {};
  if(q.function==0x8026d464){assert(q.arguments[1]==0x80420408);const std::string literal="icon";for(unsigned i=0;;++i){const auto b=q.frame->read(0xc0+i,1);assert(b);const auto c=i<literal.size()?uint8_t(literal[i]):uint8_t(0);if(*b!=c)return int32_t(*b)-c;if(!c)return 0;}}
  if(q.function==0x80081a1c)return f.namedId;
  return 0;};return s;}
 std::optional<uint32_t>run(){return resumeSourceFontWidthLine(text,lineAddress,services(),cursor);}
 uint32_t glyphWidth(uint8_t code){const auto a=actors();const auto g=sourceMatrixFontMap(code,a);const auto p=sourceMatrixFontWidth(g,a);return bfn[p-Bfn+1];}
};
using Setup=std::function<void(Fixture&)>;
void retained(const Setup&setup){Fixture expected;setup(expected);const auto value=expected.run();assert(value);for(unsigned gate=0;gate<expected.ops.size();++gate){Fixture f;setup(f);f.hold=int(gate);f.reentry=true;assert(!f.run());const auto prefix=f.trace;const auto frame=f.frame;const auto cells=frame?frame->bytes:SourceFontWidthPrivate{}.bytes;auto s=f.services();s.common.actors.audienceLifecycle.user=&expected;assert(!resumeSourceFontWidthLine(f.text,f.lineAddress,s,f.cursor));auto copy=f.text;assert(!resumeSourceFontWidthLine(copy,f.lineAddress,f.services(),f.cursor));assert(!f.run()&&f.trace==prefix&&(!frame||frame->bytes==cells));assert(f.run()==value&&f.trace==expected.trace&&f.words==expected.words);}}
int main(){
 Fixture plain;auto aw=plain.glyphWidth('A'),bw=plain.glyphWidth('B');assert(plain.run()==aw+bw+2);
 Fixture empty;empty.setText("");empty.lineAddress=0x90000;assert(empty.run()==0&&empty.words[empty.lineAddress]==0);
 Fixture lines;lines.setText("AB\nA\n");lines.lineAddress=0x90000;assert(lines.run()==aw+bw+2&&lines.words[lines.lineAddress]==2);
 Fixture wrapper;assert(resumeSourceFontWidth(wrapper.text,wrapper.services(),wrapper.cursor)==aw+bw+2);
 retained([](auto&f){f.setText("A<icon 5,1.5,0,0,3>\nB");});
 retained([](auto&f){f.setText("<icon named>\nA");f.namedId=-9;});
 retained([](auto&f){f.setText("A<R>\nB");f.lineAddress=0x90000;});
 retained([](auto&f){f.setText("A</r>\nB");f.lineAddress=0x90000;});
 Fixture icon;icon.setText("<icon 5>");assert(icon.run()==31); //20*1.5+3 minus US spacing2.
 Fixture r;r.setText("A<R>\nB");r.lineAddress=0x90000;assert(r.run()==(aw>bw?aw:bw)&&r.words[r.lineAddress]==1);
 Fixture slash;slash.setText("A</r>\nB");slash.lineAddress=0x90000;assert(slash.run()==aw+bw+2&&slash.words[slash.lineAddress]==0);
 Fixture unknown;unknown.bytes={uint8_t('A'),std::nullopt};assert(!unknown.run());auto prefix=unknown.trace;assert(!unknown.run()&&unknown.trace==prefix);unknown.bytes[1]=uint8_t(0);assert(unknown.run()==aw);
 for(unsigned i=0;i<3;++i){Fixture f;f.byteHold=int(i);assert(!f.run()&&!f.run());assert(f.run()==aw+bw+2);}
 // Every actual command byte can hold independently without replaying prior
 // source reads or successful original children.
 for(unsigned i=0;i<std::string("<icon 5>").size()+1;++i){Fixture f;f.setText("<icon 5>");f.byteHold=int(i);assert(!f.run()&&!f.run());assert(f.run()==31);}
 Fixture halfWait;halfWait.setText(std::string("\x81\x40",2));halfWait.words[Fixture::Gp+0x16c]=0;halfWait.text.half=nullptr;assert(!halfWait.run()&&halfWait.cursor.pending()==0x8007558c);prefix=halfWait.trace;assert(!halfWait.run()&&halfWait.trace==prefix);
 Fixture output;output.setText("<icon 5>");output.leaveWidth=true;assert(!output.run()&&output.cursor.pending()==0x800756b0);prefix=output.trace;assert(output.frame&&!output.frame->read(0xa,2));assert(!output.run()&&output.trace==prefix);output.frame->write(0xa,2,20);assert(output.run()==31); //No icon child replay.
 Fixture scale;scale.setText("<icon 5>");scale.leaveScale=true;assert(!scale.run()&&scale.cursor.pending()==0x800756f0);prefix=scale.trace;assert(!scale.run()&&scale.trace==prefix);scale.frame->write(0xc,4,bits(2));assert(scale.run()==41);
 Fixture stale;stale.setText("<icon 5>");stale.hold=0;assert(!stale.run());stale.alive=false;assert(!stale.run()&&stale.cursor.stale());prefix=stale.trace;stale.alive=true;assert(!stale.run()&&stale.trace==prefix);
 Fixture late;late.setText("<icon 5>");late.onOp=[](auto&f,const Event&e){if(e.fn==0x80075714)f.words[0x80416020]=7;};assert(late.run()==26);
 Fixture wrap;wrap.setText("<icon 5>");wrap.scale=3276.8f;wrap.extra=2;assert(wrap.run()==0); //Raw65538 compares low16=2, finalspacing subtractionwraps0.
 Fixture multi;multi.setText(std::string("\x81\x40",2));multi.words[Fixture::Gp+0x16c]=0;multi.words[0x8041601c]=2;assert(multi.run());bool lhz=false;for(const auto&e:multi.trace)if(e.fn==6)lhz=true;assert(lhz);
 Fixture ffff;ffff.setText(std::string("\x81\x40",2));ffff.words[Fixture::Gp+0x16c]=0;const auto info=ffff.words[0x8041e9a0]-Fixture::Bfn;ffff.bfn[info+0x12]=255;ffff.bfn[info+0x13]=255;assert(ffff.run()==0); //Unmappedffff bypasses width and final spacing.
 Fixture nullCommand;nullCommand.setText("<R "+std::string(127,'x'));assert(!nullCommand.run()&&nullCommand.cursor.pending()==0x80075804);prefix=nullCommand.trace;assert(!nullCommand.run()&&nullCommand.trace==prefix); //Actual null command return cannot read private offset0.
 Fixture noArgs;noArgs.setText("<icon>");assert(!noArgs.run()&&noArgs.cursor.pending()==0x80075614);prefix=noArgs.trace;assert(!noArgs.run()&&noArgs.trace==prefix); //No invented argument terminator.
 Fixture retainedArgs;retainedArgs.setText("<icon 5><icon>");assert(retainedArgs.run()==64); //Second parser leaves previous args intact.
 Fixture neg;neg.setText("<icon 5>");neg.extra=uint32_t(-40);assert(neg.run()==65524); //-10 truncation then unsigned16 max/final2.
 Fixture noLineWriter;noLineWriter.lineAddress=0x90000;auto noWriter=noLineWriter.services();noWriter.common.actors.memory.storeHalf=nullptr;assert(!resumeSourceFontWidthLine(noLineWriter.text,noLineWriter.lineAddress,noWriter,noLineWriter.cursor)&&noLineWriter.cursor.pending()==0x80075850);
 Fixture missing;missing.setText("<icon 5>");auto s=missing.services();s.call=nullptr;assert(!resumeSourceFontWidthLine(missing.text,0,s,missing.cursor)&&missing.cursor.pending()==0x8026d464);assert(!missing.run());
 // Genuine centered parent now consumes the whole source width child. All
 // glyph/matrix callbacks here are explicit source-controller test providers.
 struct Centered {Fixture f;SourceStringFontCursor parent;std::vector<SourceMatrixFontGlyph>glyphs;
  SourceStringFontServices services(){auto s=SourceStringFontServices{};s.common=f.services().common;s.common.user=this;
   s.common.current=[](void*p){return static_cast<Centered*>(p)->f.alive;};s.common.camera=[](void*)->std::optional<uint32_t>{return 0x10000;};
   s.common.matrix=[](void*,BattleAudienceMatrixRequest&q){BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,q);};
   s.common.loadMatrix=[](void*,const BattleActMatrix&,uint32_t i){return i==0;};s.common.truncate=[](void*,float v)->std::optional<int32_t>{return int32_t(v);};
   s.common.glyph=[](void*p,const SourceMatrixFontGlyph&q){static_cast<Centered*>(p)->glyphs.push_back(q);return true;};
   s.width=[](void*p,const SourceMatrixFontText&t,uint32_t a){auto&c=*static_cast<Centered*>(p);return resumeSourceFontWidthLine(t,a,c.f.services(),c.f.cursor);};return s;}
 } centered;centered.f.words[0x8041e9a4]=0;for(unsigned i=0;i<12;++i)centered.f.words[0x10000+0x11c+4*i]=bits(i==0||i==5||i==10?1.f:0.f);
 BattleActMatrix identity{1,0,0,0,0,1,0,0,0,0,1,0};assert(resumeSourceStringFont(identity,centered.f.text,centered.services(),centered.parent));assert(centered.glyphs.size()==2&&centered.glyphs[0].x==int32_t(-float(aw+bw+2)*.5f));
 // Every non-control byte uses the original loaded US mapping and WID bytes.
 for(unsigned code=1;code<256;++code){if(code=='<'||code=='\n')continue;Fixture f;f.setText(std::string(1,char(code)));const auto expected=f.glyphWidth(uint8_t(code));assert(f.run()==expected);}
 SourceFontWidthPrivate cells;assert(!cells.read(0x140,1)&&!cells.write(0x13f,2,0));assert(cells.write(0x20,4,0x12345678)&&cells.read(0x21,2)==0x3456);assert(cells.write(0x21,1,0xff)&&cells.read(0x20,4)==0x12ff5678);
 std::cout<<"Whole source font width, original US BFN, private outputs and retained waits PASS\n";
}
