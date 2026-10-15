#include <memory>
#include <cstring>
#include "battle_font_gx_state.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
#include "fixtures/battle_font_gpu_source_fixture.hpp"
namespace {
struct GxFixture {
 Fixture actual;BattleFontGxState gx;
 EventScriptAssets originals;
 std::map<uint32_t,uint32_t> words;
 std::map<uint32_t,uint8_t> bytes{{0x8041e9a8,255}};
 std::shared_ptr<const void> owner=std::make_shared<int>(0);
 bool source=true,publication=true,textureReady=true,reenter=false,retireTexture=false;
 std::vector<SourceFontStateCall> accepted;
 unsigned textureQueries=0;int gate=-1;unsigned attempts=0,calls=0;
 SourceFontDrawStartCursor cursor,nested;
 SourceFontStateCursor helper;
 uint32_t color=0x1248ab80;
 GxFixture(){CHECK(originals.load("romfs/trio-attack-events.pmevt"));words={{0x80415fd0,0x20000},{0x2016c,1},{0x80429578,0x12345678}};
  for(unsigned i=0;i<12;++i)words[0x40000+0x11c+4*i]=bits(actual.loaded[i]);
 }
 static uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
 bool bind(){auto b=actual.binding(8);b.ordinaryProfile=[this]{return gx.ordinaryProfile();};CHECK(actual.sink.bind(b));
 BattleFontGxBinding binding;binding.sink=&actual.sink;binding.font=actual.resource.lease();binding.resources=owner;
 binding.sourceCurrent=[this]{return source;};binding.publicationCurrent=[this]{return publication;};
 binding.staticTextureCurrent=[this](uint32_t object,const BattleFontLease&font){CHECK(object==0x8041e97c&&font.generation==actual.resource.lease().generation);++textureQueries;if(reenter){SourceFontStateCall q;q.function=0x802b4520;CHECK(!gx.request(q));CHECK(!gx.loadMatrix(actual.loaded,0));CHECK(!gx.tevColor(1,0));}if(retireTexture)gx.retire();return textureReady;};
 return gx.bind(std::move(binding));}
 SourceFontDrawStartServices services(){SourceFontDrawStartServices s;s.common.user=this;s.common.lifetime=owner;s.common.current=[](void*p){return static_cast<GxFixture*>(p)->source;};
 s.common.actors.memory.user=this;s.common.actors.memory.word=[](void*p,uint32_t a){auto&f=*static_cast<GxFixture*>(p);auto it=f.words.find(a);if(it!=f.words.end())return it->second;uint32_t n=0;CHECK(f.originals.readWord(a,n));return n;};
 s.common.actors.user=this;s.common.actors.byte=[](void*p,uint32_t a){return static_cast<GxFixture*>(p)->bytes.at(a);};
 s.common.actors.memory.storeWord=[](void*p,uint32_t a,uint32_t n){static_cast<GxFixture*>(p)->words[a]=n;};
 s.common.actors.memory.storeByte=[](void*p,uint32_t a,uint8_t n){static_cast<GxFixture*>(p)->bytes[a]=n;};
 s.camera=[](void*)->std::optional<uint32_t>{return 0x40000;};
 s.common.matrix=[](void*,BattleAudienceMatrixRequest&q){BattleAudienceAuxServices m;bindAudienceAuxMathAdapter(m);return m.matrix(nullptr,q);};
 s.common.request=[](void*p,const SourceFontStateCall&q){auto&f=*static_cast<GxFixture*>(p);if(q.function==0x80074bfc){CHECK(bool(q.color));return resumeSourceFontDrawStart(*q.color,f.services(),f.nested);}if(int(f.calls)==f.gate&&f.attempts++<2)return false;const bool ok=f.gx.request(q);if(ok){++f.calls;f.accepted.push_back(q);}return ok;};
 s.fog=[](void*p,const SourceFontDrawStartFog&q){auto&f=*static_cast<GxFixture*>(p);if(int(f.calls)==f.gate&&f.attempts++<2)return false;const bool ok=f.gx.fog(q);if(ok)++f.calls;return ok;};return s;}
 bool run(){return resumeSourceFontDrawStart(color,services(),cursor);}
 SourceFontStateCall request(uint32_t fn,std::vector<uint32_t> args={}){SourceFontStateCall q;q.function=fn;q.integers=std::move(args);return q;}
};
}
int main(){
 BattleFontGxState unavailable;CHECK(!unavailable.bind({}));CHECK(!unavailable.ordinaryProfile());
 GxFixture f;CHECK(f.bind());CHECK(!f.gx.ordinaryProfile());CHECK(!f.actual.sink.glyph(f.actual.glyph('A'))&&f.actual.randoms==0);CHECK(f.run());CHECK(f.calls==31&&f.gx.ordinaryProfile());
 const auto state=f.gx.snapshot();CHECK(state&&state->fog&&state->fog->color==0x12345678&&state->positionMatrix==f.actual.loaded&&state->color==f.color);
 CHECK(f.actual.sink.glyph(f.actual.glyph('A',4,-8))&&f.actual.randoms==2);
 auto moved=f.actual.loaded;moved[3]=99;CHECK(f.gx.loadMatrix(moved,0));CHECK(f.gx.tevColor(1,0xabcdef00));CHECK(f.actual.sink.glyph(f.actual.glyph('B'))&&f.actual.randoms==4);
 CHECK(f.actual.sink.publish());const auto packets=f.actual.sink.snapshot();CHECK(packets&&packets->size()==2&&packets->at(0).matrix==f.actual.loaded&&packets->at(1).matrix==moved&&packets->at(0).quad.rgba==f.color&&packets->at(1).quad.rgba==0xabcdef00);
 for(unsigned gate=0;gate<31;++gate){GxFixture held;held.gate=int(gate);CHECK(held.bind());CHECK(!held.run());CHECK(!held.gx.ordinaryProfile());const auto prefix=held.gx.snapshot();CHECK(bool(prefix));CHECK(!held.run());CHECK(held.gx.snapshot()->fixedState==prefix->fixedState);CHECK(held.run());CHECK(held.calls==31&&held.gx.ordinaryProfile());}
 GxFixture missingTexture;missingTexture.textureReady=false;CHECK(missingTexture.bind());CHECK(!missingTexture.run()&&!missingTexture.gx.ordinaryProfile());const auto prefix=missingTexture.calls;CHECK(!missingTexture.run()&&missingTexture.calls==prefix);missingTexture.textureReady=true;CHECK(missingTexture.run()&&missingTexture.gx.ordinaryProfile());missingTexture.textureReady=false;CHECK(!missingTexture.gx.ordinaryProfile());missingTexture.textureReady=true;CHECK(!missingTexture.gx.ordinaryProfile());
 for(auto call:std::vector<SourceFontStateCall>{f.request(0x802b5084,{2}),f.request(0x802b8fcc,{2}),f.request(0x802b8e30,{1,255,255,4}),f.request(0x802b4e04,{1,1,5,33,0,125}),f.request(0xdeadbeef),f.request(0x802b7880,{0x8041e980,0})}){GxFixture bad;CHECK(bad.bind()&&bad.run());CHECK(!bad.gx.request(call)&&!bad.gx.ordinaryProfile());CHECK(!bad.actual.sink.glyph(bad.actual.glyph('A'))&&bad.actual.randoms==0);CHECK(bad.cursor.reset()&&bad.run()&&bad.gx.ordinaryProfile());}
 for(auto malformed:f.accepted){GxFixture bad;CHECK(bad.bind()&&bad.run());if(malformed.integers.empty())malformed.integers.push_back(1);else malformed.integers.back()^=0x100;CHECK(!bad.gx.request(malformed)&&!bad.gx.ordinaryProfile());CHECK(!bad.actual.sink.glyph(bad.actual.glyph('A'))&&bad.actual.randoms==0);}
 GxFixture language;language.words[0x2016c]=0;CHECK(language.bind());CHECK(!language.run()&&language.cursor.pending()==0x802ba3d0&&!language.gx.ordinaryProfile());language.words[0x2016c]=1;CHECK(!language.run()); // Original retained SDK matrix is already captured; no reread/substitution.
 GxFixture matrix;CHECK(matrix.bind()&&matrix.run());auto q=matrix.request(0x802ba3d0,{30,1});q.matrix=matrix.gx.snapshot()->textureMatrix;(*q.matrix)[5]=1.f/512;CHECK(!matrix.gx.request(q)&&!matrix.gx.ordinaryProfile());
 GxFixture fog;CHECK(fog.bind()&&fog.run());auto wrong=*fog.gx.snapshot()->fog;wrong.type=1;CHECK(!fog.gx.fog(wrong)&&!fog.gx.ordinaryProfile());
 GxFixture helper;CHECK(helper.bind());SourceFontStateInput input;input.function=0x80077b4c;CHECK(bool(resumeSourceFontState(input,helper.services().common,helper.helper))&&helper.gx.ordinaryProfile());input.function=0x80077844;input.argument=3;CHECK(bool(resumeSourceFontState(input,helper.services().common,helper.helper)));CHECK(helper.gx.snapshot()->color==helper.words.at(0x8041e9ac));CHECK(helper.actual.sink.glyph(helper.actual.glyph('A'))&&helper.actual.randoms==2);input.function=0x800778e0;CHECK(!resumeSourceFontState(input,helper.services().common,helper.helper)&&helper.helper.pending()==0x802b5084&&!helper.gx.ordinaryProfile());
 GxFixture live;CHECK(live.bind()&&live.run());live.source=false;CHECK(!live.gx.ordinaryProfile());live.source=true;CHECK(!live.gx.request(live.request(0x802b8880,{0})));
 GxFixture lease;CHECK(lease.bind()&&lease.run());lease.actual.resource.retire();CHECK(!lease.gx.ordinaryProfile()&&!lease.gx.snapshot());
 GxFixture pub;CHECK(pub.bind()&&pub.run());pub.publication=false;CHECK(!pub.gx.ordinaryProfile());pub.publication=true;CHECK(!pub.gx.ordinaryProfile());
 GxFixture recursive;CHECK(recursive.bind());recursive.reenter=true;CHECK(recursive.run());recursive.reenter=false;CHECK(recursive.gx.ordinaryProfile());
 GxFixture retire;CHECK(retire.bind());retire.retireTexture=true;CHECK(!retire.run()&&!retire.gx.ordinaryProfile());
 std::cout<<"Finite real font GX state, actual glyph sink/RNG packets, setup holds and unsupported profile rejection passed\n";
}
