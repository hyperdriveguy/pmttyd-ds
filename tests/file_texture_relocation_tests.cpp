#include "file_texture_relocation.hpp"
#include <cassert>
#include <functional>
#include <fstream>
#include <iterator>
#include <unordered_map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000;
  std::unordered_map<uint32_t,uint8_t> bytes;
  std::vector<uint32_t> reads, writes;
  std::function<void(uint32_t,uint32_t)> afterStore;
  unsigned panicCalls = 0, countReads = 0, countLimit = 0;
  bool panicReady = false;
  uint8_t byte(uint32_t a) { return bytes[a]; }
  uint32_t word(uint32_t a) {
    uint32_t n = 0;
    for(unsigned i=0;i<4;++i)n=(n<<8)|byte(a+i);
    return n;
  }
  void put(uint32_t a,uint32_t n) {
    for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(n>>(24-8*i));
  }
  Fixture() { put(B,0x0020af30);put(B+8,0x100); }
  FileTextureRelocationServices services() {
    FileTextureRelocationServices s;
    s.memory.user=this;s.user=this;
    s.memory.word=[](void* p,uint32_t a) {
      auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);
      if(a==B+4 && f.countLimit && ++f.countReads>f.countLimit)return 0u;
      return f.word(a);
    };
    s.memory.storeWord=[](void* p,uint32_t a,uint32_t n) {
      auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.put(a,n);
      if(f.afterStore)f.afterStore(a,n);
    };
    s.memory.storeByte=[](void* p,uint32_t a,uint8_t n) {
      auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.bytes[a]=n;
    };
    s.byte=[](void* p,uint32_t a) { return static_cast<Fixture*>(p)->byte(a); };
    s.panic=[](void* p,uint32_t file,uint32_t line,uint32_t format) {
      auto& f=*static_cast<Fixture*>(p);++f.panicCalls;
      assert(file==0x802f9470 && line==112 && format==0x802f947c);
      return f.panicReady;
    };
    return s;
  }
  bool run() { FileTextureRelocationCursor c;return resumePackedFileTextureRelocation(B,services(),c); }
};
int main(int argc, char** argv) {
  if(argc == 2) {
    std::ifstream input(argv[1], std::ios::binary);
    assert(input);
    std::vector<uint8_t> raw{std::istreambuf_iterator<char>(input), {}};
    assert(raw.size() >= 12);
    Fixture original;
    for(unsigned i=0;i<raw.size();++i)original.bytes[Fixture::B+i]=raw[i];
    const auto table=original.word(Fixture::B+8), count=original.word(Fixture::B+4);
    assert(original.word(Fixture::B)==0x0020af30 && table<Fixture::B && count<65536);
    struct Entry {uint32_t pointer,data;uint8_t flag;};
    std::vector<Entry> expected;
    for(unsigned i=0;i<count;++i)for(unsigned slot=0;slot<2;++slot) {
      const auto q=original.word(Fixture::B+table+8*i+4*slot);
      expected.push_back({q,q?original.word(Fixture::B+q+8):0,
                         uint8_t(q?original.byte(Fixture::B+q+(slot?2:0x23)):0)});
    }
    assert(original.run());
    assert(original.word(Fixture::B+8)==Fixture::B+table);
    for(unsigned i=0;i<count;++i)for(unsigned slot=0;slot<2;++slot) {
      const auto before=expected[2*i+slot];
      const auto after=original.word(Fixture::B+table+8*i+4*slot);
      assert(after==(before.pointer?Fixture::B+before.pointer:0));
      if(after) {
        assert(original.word(after+8)==(before.flag?before.data:Fixture::B+before.data));
        assert(original.byte(after+(slot?2:0x23))==(before.flag?before.flag:1));
      }
    }
    assert(!original.panicCalls);
    return 0;
  }

  Fixture f;f.put(Fixture::B+4,1);
  f.put(Fixture::B+0x100,0x200);f.put(Fixture::B+0x104,0x300);
  f.put(Fixture::B+0x208,0);f.put(Fixture::B+0x308,0x400);
  assert(f.run());
  assert(f.word(Fixture::B+8)==Fixture::B+0x100);
  assert(f.word(Fixture::B+0x100)==Fixture::B+0x200);
  assert(f.word(Fixture::B+0x208)==Fixture::B);
  assert(f.word(Fixture::B+0x308)==Fixture::B+0x400);
  assert(f.byte(Fixture::B+0x223)==1 && f.byte(Fixture::B+0x302)==1);
  const auto prior=f.writes.size();assert(f.run() && f.writes.size()==prior);
  for(uint8_t flag:{1,0x80,0xff}) {
    Fixture marked;marked.put(Fixture::B+4,1);
    marked.put(Fixture::B+0x100,0x200);marked.put(Fixture::B+0x104,0x300);
    marked.bytes[Fixture::B+0x223]=flag;marked.bytes[Fixture::B+0x302]=flag;
    marked.put(Fixture::B+0x208,0x123);marked.put(Fixture::B+0x308,0x456);
    assert(marked.run());
    assert(marked.word(Fixture::B+0x208)==0x123 && marked.word(Fixture::B+0x308)==0x456);
    assert(marked.word(Fixture::B+0x100)==Fixture::B+0x200);
  }
  // A returning panic falls through, with no signature reread on retained wait.
  Fixture bad;bad.put(Fixture::B,0);bad.put(Fixture::B+4,0);
  FileTextureRelocationCursor cursor;auto services=bad.services();
  assert(!resumePackedFileTextureRelocation(Fixture::B,services,cursor));
  assert(cursor.active && bad.reads.size()==1 && bad.writes.empty());
  auto other=services;other.memory.user=&f;
  assert(!resumePackedFileTextureRelocation(Fixture::B,other,cursor));
  assert(!resumePackedFileTextureRelocation(Fixture::B+4,services,cursor));
  assert(bad.panicCalls==1);
  bad.put(Fixture::B,0x0020af30);bad.panicReady=true;
  assert(resumePackedFileTextureRelocation(Fixture::B,services,cursor));
  assert(!cursor.active && bad.panicCalls==2 && bad.reads[1]==Fixture::B+8);
  Fixture noPanic;noPanic.put(Fixture::B,0);auto unavailable=noPanic.services();unavailable.panic=nullptr;
  assert(!resumePackedFileTextureRelocation(Fixture::B,unavailable,cursor));
  assert(noPanic.writes.empty());
  for(uint32_t table:{Fixture::B,Fixture::B+1,0xffffffffu}) {
    Fixture guarded;guarded.put(Fixture::B+8,table);assert(guarded.run());
    assert(guarded.reads.size()==2 && guarded.writes.empty());
  }
  Fixture zeroRoot;zeroRoot.put(Fixture::B+8,0);assert(zeroRoot.run());
  assert(zeroRoot.word(Fixture::B+8)==Fixture::B); // Relative null root is not skipped.
  // Root changes after a descriptor data store: final flag uses fresh root/slot.
  Fixture alias;alias.put(Fixture::B+4,1);alias.put(Fixture::B+0x100,0x200);
  alias.put(Fixture::B+0x500,Fixture::B+0x600);
  alias.afterStore=[&](uint32_t a,uint32_t) {
    if(a==Fixture::B+0x208)alias.put(Fixture::B+8,Fixture::B+0x500);
  };
  assert(alias.run());assert(alias.byte(Fixture::B+0x623)==1);
  assert(alias.byte(Fixture::B+0x223)==0);
  // First marker aliases and replaces second descriptor before its fresh read.
  Fixture live;live.put(Fixture::B+4,1);live.put(Fixture::B+0x100,0x200);
  live.afterStore=[&](uint32_t a,uint32_t) {
    if(a==Fixture::B+0x208)live.put(Fixture::B+0x104,0x300);
  };
  assert(live.run() && live.byte(Fixture::B+0x302)==1);
  // Live count stops traversal after first record, despite initial count2.
  Fixture count;count.put(Fixture::B+4,2);count.put(Fixture::B+0x100,0x200);
  count.afterStore=[&](uint32_t a,uint32_t) {
    if(a==Fixture::B+0x208)count.put(Fixture::B+4,0);
  };
  assert(count.run());assert(count.reads.back()==Fixture::B+4);
  // COUNT65536 revisits low16 index0. Fixture count provider eventually exits;
  // production has no artificial cap or ordinary uint32-index termination.
  Fixture wrap;wrap.put(Fixture::B+4,65536);wrap.countLimit=65537;
  assert(wrap.run());assert(wrap.countReads==65538);
  unsigned firstSlotReads=0;
  for(auto a:wrap.reads)if(a==Fixture::B+0x100)++firstSlotReads;
  assert(firstSlotReads==2);
  Fixture maximum;maximum.put(Fixture::B+4,65535);assert(maximum.run());
  // Modular descriptor/data sums, without an absolute-pointer range test.
  Fixture modular;modular.put(Fixture::B+4,1);modular.put(Fixture::B+0x100,0xffff1000);
  modular.put(0x1008,0xffff2000);assert(modular.run());
  assert(modular.word(Fixture::B+0x100)==0x1000 && modular.word(0x1008)==0x2000);
}
