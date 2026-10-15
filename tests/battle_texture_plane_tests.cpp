#include "battle_texture_plane.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
using namespace ttyd;
namespace {
using F=BattleTexturePlaneCall;
struct Event {
  bool math=false;
  BattleTexturePlaneRequest q;
  uint32_t function=0;
  BattleActMatrix left{},right{};
  bool operator==(const Event& r) const {
    return math==r.math && q.kind==r.q.kind && q.function==r.q.function &&
        q.integers==r.q.integers && q.reals==r.q.reals && q.matrix==r.q.matrix &&
        function==r.function && left==r.left && right==r.right;
  }
};
uint32_t bits(float value){uint32_t n;std::memcpy(&n,&value,4);return n;}
struct Fixture {
  static constexpr uint32_t B=0x10000,Cam=0x20000,Descriptor=0x30000,File=0x40000,Palette=0x50000,Handle=0x6789;
  std::map<uint32_t,uint32_t> words;
  std::vector<uint32_t> reads;
  std::vector<Event> attempted,completed;
  BattleActMatrix base{1,0,0,3,0,1,0,4,0,0,1,5};
  uint32_t rgba=0x12345678,format=0,width=80,height=32;
  int stop=-1;
  bool released=false,mutate=false;
  Fixture() {
    words[0x8041eb90]=B;words[B+0x163f8]=Descriptor;words[Descriptor+0xa0]=File;words[File]=Palette;
    const BattleActMatrix view{1,0,0,10,0,1,0,20,0,0,1,30};
    for(unsigned i=0;i<12;++i)words[Cam+0x11c+4*i]=bits(view[i]);
    words[0x80422234]=bits(.5f);words[0x80422240]=bits(0);words[0x8042224c]=bits(1);
    words[0x802ee400]=0x43300000;words[0x802ee404]=0;
  }
  bool event(const Event& e) {
    attempted.push_back(e);
    if(int(completed.size())==stop && !released)return false;
    completed.push_back(e);return true;
  }
  BattleTexturePlaneServices services() {
    BattleTexturePlaneServices s;s.user=this;s.actors.memory.user=this;
    s.actors.memory.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words.at(a);};
    s.request=[](void* p,const BattleTexturePlaneRequest& q)->std::optional<uint32_t> {
      auto& f=*static_cast<Fixture*>(p);
      if(!f.event({false,q,0,{},{}}))return {};
      if(q.kind!=BattleTexturePlaneKind::Call)return 0;
      switch(q.function) {
      case F::Cull:assert(q.integers==std::vector<uint32_t>{0});break;
      case F::ClearDescriptors:if(f.mutate)f.base[3]=7;break;
      case F::CurrentCamera: {
        assert(q.integers.empty());
        if(f.mutate){f.words[0x8041eb90]=B+0x100000;f.words[B+0x100000+0x163f8]=Descriptor;}
        return Cam;
      }
      case F::LoadMatrix:
        assert(q.integers==std::vector<uint32_t>{0});
        assert(q.matrix && (*q.matrix)[3]==(f.mutate?17:13) && (*q.matrix)[7]==24);
        if(f.mutate)f.rgba=0xaabbccdd;
        break;
      case F::TextureObject:assert(q.integers==std::vector<uint32_t>({Palette,4}));return Handle;
      case F::LoadTexture:assert(q.integers==std::vector<uint32_t>({Handle,0}));break;
      case F::TextureFormat:assert(q.integers==std::vector<uint32_t>{Handle});return f.format;
      case F::TextureWidth:assert(q.integers==std::vector<uint32_t>{Handle});return f.width;
      case F::TextureHeight:
        assert(q.integers==std::vector<uint32_t>{Handle});
        if(f.mutate)f.words[0x80422234]=bits(1);
        return f.height;
      default:break;
      }
      return 0;
    };
    s.matrix=[](void* p,BattleAudienceMatrixRequest& q) {
      auto& f=*static_cast<Fixture*>(p);
      if(!f.event({true,{},q.function,q.left,q.right}))return false;
      if(q.function==0x8029dd30)q.output=q.left;
      else {
        assert(q.function==0x8029dd64);
        for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col) {
          float value=col==3?q.left[4*row+3]:0;
          for(unsigned k=0;k<3;++k)value+=q.left[4*row+k]*q.right[4*k+col];
          q.output[4*row+col]=value;
        }
      }
      return true;
    };
    return s;
  }
  bool run(BattleTexturePlaneCursor& c){return resumePackedBattleTexturePlane2(base,4,rgba,services(),c);}
  std::vector<Event> fifo() const {
    std::vector<Event> result;for(const auto& e:completed)if(!e.math && e.q.kind!=BattleTexturePlaneKind::Call)result.push_back(e);return result;
  }
};
void verifyQuad(const Fixture& f,uint32_t rgba,float hw,float hh) {
  const auto fifo=f.fifo();assert(fifo.size()==36);
  for(unsigned v=0;v<4;++v) {
    assert(fifo[v*9].q.reals==std::vector<float>{v==0 || v==3?-hw:hw});
    assert(fifo[v*9+1].q.reals==std::vector<float>{v<2?hh:-hh});
    assert(fifo[v*9+2].q.reals==std::vector<float>{0});
    for(unsigned c=0;c<4;++c)assert(fifo[v*9+3+c].q.integers==std::vector<uint32_t>{uint8_t(rgba>>(24-8*c))});
    assert(fifo[v*9+7].q.reals==std::vector<float>{v==1 || v==2?1.f:0.f});
    assert(fifo[v*9+8].q.reals==std::vector<float>{v>=2?1.f:0.f});
  }
}
}
int main() {
  for(uint32_t format:{0u,1u,0xffffffffu}) {
    Fixture baseline;baseline.format=format;BattleTexturePlaneCursor c;assert(baseline.run(c) && !c.active());
    verifyQuad(baseline,0x12345678,40,16);
    unsigned colorIn=0,alphaIn=0,tevOp=0;
    for(const auto& e:baseline.completed)if(!e.math && e.q.kind==BattleTexturePlaneKind::Call) {
      if(e.q.function==F::ColorIn){++colorIn;assert(e.q.integers==std::vector<uint32_t>({0,15,15,15,10}));}
      if(e.q.function==F::AlphaIn){++alphaIn;assert(e.q.integers==std::vector<uint32_t>({0,7,5,4,7}));}
      if(e.q.function==F::TevOp){++tevOp;assert(e.q.integers==std::vector<uint32_t>({0,0}));}
    }
    assert(colorIn==(format?0u:1u) && alphaIn==(format?0u:1u) && tevOp==(format?1u:0u));
    for(size_t stop=0;stop<baseline.completed.size();++stop) {
      Fixture held;held.format=format;held.stop=int(stop);auto services=held.services();BattleTexturePlaneCursor cursor;
      assert(!held.run(cursor) && cursor.active());
      const auto operations=held.completed.size(),reads=held.reads.size();
      assert(!held.run(cursor) && held.completed.size()==operations && held.reads.size()==reads);
      auto otherBase=held.base;auto otherColor=held.rgba;
      assert(!resumePackedBattleTexturePlane2(otherBase,4,held.rgba,services,cursor));
      assert(!resumePackedBattleTexturePlane2(held.base,4,otherColor,services,cursor));
      assert(!resumePackedBattleTexturePlane2(held.base,5,held.rgba,services,cursor));
      auto wrong=services;wrong.user=&baseline;
      assert(!resumePackedBattleTexturePlane2(held.base,4,held.rgba,wrong,cursor));
      assert(held.completed.size()==operations && held.reads.size()==reads);
      held.released=true;assert(held.run(cursor) && !cursor.active());
      assert(held.completed==baseline.completed && held.reads==baseline.reads && held.words==baseline.words);
    }
  }
  // Base read occurs after Init; color after matrix load; fresh B at PlainGX;
  // half multiplier rereads after Height rather than caching Width's value.
  Fixture late;late.mutate=true;BattleTexturePlaneCursor lc;assert(late.run(lc));
  verifyQuad(late,0xaabbccdd,40,32);
  // Mutating borrowed inputs after their original capture sites must not alter
  // an already admitted SDK operation or the private four copied RGBA words.
  Fixture reference;BattleTexturePlaneCursor rc;assert(reference.run(rc));
  for(size_t stop=0;stop<reference.completed.size();++stop) {
    const auto& event=reference.completed[stop];
    if(event.math && event.function==0x8029dd30) {
      Fixture held;held.stop=int(stop);BattleTexturePlaneCursor cursor;
      assert(!held.run(cursor));held.base[3]=99;held.released=true;
      assert(held.run(cursor));assert(held.completed==reference.completed);
    }
    if(!event.math && event.q.kind==BattleTexturePlaneKind::Call && event.q.function==F::TextureObject) {
      Fixture held;held.stop=int(stop);BattleTexturePlaneCursor cursor;
      assert(!held.run(cursor));held.rgba=0xffffffff;held.released=true;
      assert(held.run(cursor));assert(held.completed==reference.completed);
    }
    // Each original lfs is captured before its potentially held FIFO store.
    if(!event.math && event.q.kind==BattleTexturePlaneKind::FifoFloat &&
        event.q.reals==std::vector<float>{0}) {
      Fixture held;held.stop=int(stop);BattleTexturePlaneCursor cursor;
      assert(!held.run(cursor));const auto reads=held.reads.size();
      held.words[0x80422240]=bits(2);assert(!held.run(cursor));
      assert(held.reads.size()==reads);held.released=true;assert(held.run(cursor));
      assert(held.completed[stop]==event);break;
    }
  }
  Fixture dimensions;dimensions.width=0xffff0050;dimensions.height=0xabcd0020;
  BattleTexturePlaneCursor dc;assert(dimensions.run(dc));verifyQuad(dimensions,0x12345678,40,16);
  Fixture absent;auto unavailable=absent.services();unavailable.request=nullptr;BattleTexturePlaneCursor ac;
  assert(!resumePackedBattleTexturePlane2(absent.base,4,absent.rgba,unavailable,ac));
  assert(absent.completed.empty() && absent.reads.empty());
  std::cout<<"Original TexPlane2 Init/camera/PlainGX, format branches, FIFO and retained prefixes passed\n";
}
