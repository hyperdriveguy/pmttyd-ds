#include "battle_command_input_continuation.hpp"
// Field offsets and masks in these notes are hex; counts, strides, states
// and icon IDs are decimal. Original addresses remain32-bit source identities.
namespace ttyd {
namespace {
int32_t sb(uint8_t v){return v<128?int32_t(v):int32_t(v)-256;}
int32_t sh(uint16_t v){return v<32768?int32_t(v):int32_t(v)-65536;}
bool same(const BattleCommandInputContinuationServices& a,const BattleCommandInputContinuationServices& b){
 const auto& x=a.actors.memory;const auto& y=b.actors.memory;
 return a.user==b.user&&a.call==b.call&&a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&
 a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event&&
 x.user==y.user&&x.word==y.word&&x.half==y.half&&x.storeWord==y.storeWord&&x.storeHalf==y.storeHalf&&x.storeByte==y.storeByte&&x.flushBlock==y.flushBlock&&x.systemCall==y.systemCall;
}
struct Running{bool& f;explicit Running(bool& v):f(v){f=true;}~Running(){f=false;}};
}
// B battle,U retained acting unit,M=B171c menu,pc original source site.
// M00 STATE differs from TYPE M540; M544 prior type,M550 operation byte,
// M568 reason-event ID. Private cursor output starts unknown, not guest zero.
struct BattleCommandInputContinuationCursor::State {
 BattleCommandInputContinuationServices services;
 uint32_t b=0,u=0,m=0,pc=0x801229f0,pending=0,state=0,type=0,buffer=0,weapon=0;
 int32_t id=0,part=0,progress=0;
 std::optional<uint32_t> privateCursor;
 std::optional<BattleCommandInputContinuationRequest> held;
 uint32_t word(uint32_t a)const{return services.actors.memory.word(services.actors.memory.user,a);}
 uint16_t half(uint32_t a)const{return services.actors.memory.half(services.actors.memory.user,a);}
 uint8_t byte(uint32_t a)const{return services.actors.byte(services.actors.user,a);}
 void store(uint32_t a,uint32_t n)const{services.actors.memory.storeWord(services.actors.memory.user,a,n);}
 void put(uint32_t a,uint8_t n)const{services.actors.memory.storeByte(services.actors.memory.user,a,n);}
 template<class Build> std::optional<uint32_t> call(uint32_t fn,Build build){
  pending=fn;
  if(!held){held=BattleCommandInputContinuationRequest{};held->function=fn;held->callsite=pc;build(*held);}
  if(!services.call)return {};
  const auto result=services.call(services.user,*held);
  if(result){held.reset();pending=0;}return result;
 }
};
BattleCommandInputContinuationCursor::BattleCommandInputContinuationCursor()=default;
BattleCommandInputContinuationCursor::~BattleCommandInputContinuationCursor()=default;
bool BattleCommandInputContinuationCursor::active()const{return bool(state_);}
uint32_t BattleCommandInputContinuationCursor::pending()const{return state_?state_->pending:0;}
uint32_t BattleCommandInputContinuationCursor::callsite()const{return state_?state_->pc:0;}
bool BattleCommandInputContinuationCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<int32_t> resumePackedBattleCommandInput(uint32_t battle,const BattleCommandInputContinuationServices& s,BattleCommandInputContinuationCursor& cursor){
 const auto& m=s.actors.memory;
 if(cursor.invoking_||!m.word||!m.half||!m.storeWord||!m.storeByte||!s.actors.byte)return {};
 if(cursor.state_&&(cursor.state_->b!=battle||!same(cursor.state_->services,s)))return {};
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleCommandInputContinuationCursor::State>();cursor.state_->services=s;cursor.state_->b=battle;}
 auto& c=*cursor.state_;
 const auto finish=[&](int32_t n){cursor.state_.reset();return std::optional<int32_t>(n);};
 for(;;)switch(c.pc){
 case 0x801229f0: {
  auto result=c.call(0x800f88cc,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.word(c.b+0x420);});
  if(!result)return {};
  c.u=*result;c.m=c.b+0x171c;if(!c.m)return finish(1);{c.pc=0x80122a14;continue;}
 }
 // Initial GetCursorPtr(M,14) owns this invocation's actual optional SP08.
 // Successful unsupported lookup may leave it unknown; only actual consuming
 // sites hold for missing knowledge. Never borrow a sibling owner's output.
 case 0x80122a14: {
  auto result=c.call(0x80124cb8,[&](auto& q){q.privateCursorOutput=&c.privateCursor;q.arguments[q.count++]=c.m;q.arguments[q.count++]=14;});
  if(!result)return {};
  {c.pc=0x80122a18;continue;}
 }
 case 0x80122a18: {
  auto result=c.call(0x80120024,[&](auto&){ });
  if(!result)return {};
  c.pc=*result?0x80122a28:0x80122a2c;continue;
 }
 case 0x80122a28: {
  auto result=c.call(0x8021690c,[&](auto& q){q.arguments[q.count++]=0;});
  if(!result)return {};
  {c.pc=0x80122a2c;continue;}
 }
 case 0x80122a2c: {
  auto result=c.call(0x8011f5f0,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x80122a44;continue;}
 }
 case 0x80122a44: {
  c.progress=0;{c.pc=0x80122a48;continue;}
 }
 // Original30-state jump table (0..29); unsigned out-of-range reaches common
 // tail. Targets below are source PCs, not callback addresses or frame counts.
 case 0x80122a48: {
  static constexpr uint32_t entries[]={0x80122a6c,0x80122b24,0x80122b38,0x80122b4c,0x80122b6c,0x80122b94,0x80122b94,0x80122c54,0x80122c68,0x80122c88,0x80122cec,0x80122d20,0x80122cb0,0x80122ccc,0x80122fd8,0x80122fd8,0x8012300c,0x80123020,0x80123040,0x80123064,0x80123064,0x8012309c,0x801230b0,0x801230d0,0x801230f8,0x801230f8,0x8012312c,0x80123144,0x80123158,0x801231ac};
  c.state=c.word(c.m);c.pc=c.state<30?entries[c.state]:0x801231dc;continue;
 }
 case 0x80122a6c: {
  if(c.byte(c.b+0x1c74)){
   const auto type=c.word(c.m+0x540);if(type==11||type==12)c.store(c.m+0x540,c.word(c.m+0x544));
   if(c.byte(c.b+0x1c74)==2)c.put(c.b+0x1c74,0);
  }
  c.put(c.m+0x551,0);c.store(c.b+0xef8,c.word(c.b+0xef8)|1);{c.pc=0x80122ac4;continue;}
 }
 case 0x80122ac4: {
  auto result=c.call(0x801264cc,[&](auto& q){q.arguments[q.count++]=c.u;});
  if(!result)return {};
  const auto mode=sb(c.byte(c.u+0x20));c.pc=mode>=5&&mode<=9?0x80122ae0:0x80122b00;continue;
 }
 case 0x80122ae0: {
  auto result=c.call(0x8013899c,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x80122ae4;continue;}
 }
 case 0x80122ae4: {
  auto result=c.call(0x80138990,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x80122ae8;continue;}
 }
 case 0x80122ae8: {
  auto result=c.call(0x80138988,[&](auto&){ });
  if(!result)return {};
  c.store(c.m,5);{c.pc=0x80122af0;continue;}
 }
 case 0x80122b0c: {
  auto result=c.call(0x8013899c,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x80122b10;continue;}
 }
 case 0x80122b10: {
  auto result=c.call(0x80138990,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x80122b14;continue;}
 }
 case 0x80122b14: {
  auto result=c.call(0x80138988,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x801231dc;continue;}
 }
 case 0x80122af0: {
{c.pc=0x801231dc;continue;}
 }
 case 0x80122b00: {
  auto result=c.call(0x80122794,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=0;});
  if(!result)return {};
  c.store(c.m,2);{c.pc=0x80122b0c;continue;}
 }
 case 0x80122b24: {
  auto result=c.call(0x80122794,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=1;});
  if(!result)return {};
  c.store(c.m,2);{c.pc=0x801231dc;continue;}
 }
 case 0x80122b38: {
  auto result=c.call(0x8012230c,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  if(*result){c.progress=1;}
  {c.pc=0x801231dc;continue;}
 }
 // Four reason-event create/wait pairs:3/4->1,8/9->6,17/18->15,22/23->20.
 // Store wait state BEFORE evtEntry(script80416dc4,priority0,flags32),publish
 // entry+15c ID at M568,then immediately query CURRENT M568. No null guard.
 case 0x80122b4c: {
  c.store(c.m,c.state+1);{c.pc=0x80122b60;continue;}
 }
 case 0x80122b60: {
  auto result=c.call(0x800374ac,[&](auto& q){q.arguments[q.count++]=0x80416dc4;q.arguments[q.count++]=0;q.arguments[q.count++]=32;});
  if(!result)return {};
  c.store(c.m+0x568,c.word(*result+0x15c));{c.pc=0x80122b70;continue;}
 }
 case 0x80122b6c: {
{c.pc=0x80122b70;continue;}
 }
 case 0x80122b70: {
  auto result=c.call(0x80036604,[&](auto& q){q.arguments[q.count++]=c.word(c.m+0x568);});
  if(!result)return {};
  if(!*result){c.store(c.m+0x568,0);c.progress=1;c.store(c.m,1);}{c.pc=0x801231dc;continue;}
 }
 case 0x80122c68: {
  c.store(c.m,c.state+1);{c.pc=0x80122c7c;continue;}
 }
 case 0x80122c7c: {
  auto result=c.call(0x800374ac,[&](auto& q){q.arguments[q.count++]=0x80416dc4;q.arguments[q.count++]=0;q.arguments[q.count++]=32;});
  if(!result)return {};
  c.store(c.m+0x568,c.word(*result+0x15c));{c.pc=0x80122c8c;continue;}
 }
 case 0x80122c88: {
{c.pc=0x80122c8c;continue;}
 }
 case 0x80122c8c: {
  auto result=c.call(0x80036604,[&](auto& q){q.arguments[q.count++]=c.word(c.m+0x568);});
  if(!result)return {};
  if(!*result){c.store(c.m+0x568,0);c.progress=1;c.store(c.m,6);}{c.pc=0x801231dc;continue;}
 }
 case 0x80123020: {
  c.store(c.m,c.state+1);{c.pc=0x80123034;continue;}
 }
 case 0x80123034: {
  auto result=c.call(0x800374ac,[&](auto& q){q.arguments[q.count++]=0x80416dc4;q.arguments[q.count++]=0;q.arguments[q.count++]=32;});
  if(!result)return {};
  c.store(c.m+0x568,c.word(*result+0x15c));{c.pc=0x80123044;continue;}
 }
 case 0x80123040: {
{c.pc=0x80123044;continue;}
 }
 case 0x80123044: {
  auto result=c.call(0x80036604,[&](auto& q){q.arguments[q.count++]=c.word(c.m+0x568);});
  if(!result)return {};
  if(!*result){c.store(c.m+0x568,0);c.progress=1;c.store(c.m,15);}{c.pc=0x801231dc;continue;}
 }
 case 0x801230b0: {
  c.store(c.m,c.state+1);{c.pc=0x801230c4;continue;}
 }
 case 0x801230c4: {
  auto result=c.call(0x800374ac,[&](auto& q){q.arguments[q.count++]=0x80416dc4;q.arguments[q.count++]=0;q.arguments[q.count++]=32;});
  if(!result)return {};
  c.store(c.m+0x568,c.word(*result+0x15c));{c.pc=0x801230d4;continue;}
 }
 case 0x801230d0: {
{c.pc=0x801230d4;continue;}
 }
 case 0x801230d4: {
  auto result=c.call(0x80036604,[&](auto& q){q.arguments[q.count++]=c.word(c.m+0x568);});
  if(!result)return {};
  if(!*result){c.store(c.m+0x568,0);c.progress=1;c.store(c.m,20);}{c.pc=0x801231dc;continue;}
 }
 // States5/6 share weapon setup. Type0+Bef4 bit200 or type1+bit400
 // directly selects row0/target; other paths preserve flag policy by entry
 // state and fresh item badge bytes U2f3/U2f4 before writing state7.
 case 0x80122b94: {
  c.type=c.word(c.m+0x540);
  if((c.type==0&&(c.word(c.b+0xef4)&0x200))||(c.type==1&&(c.word(c.b+0xef4)&0x400))){c.pc=0x80122bc8;continue;}
  c.pc=c.state==6?0x80122c34:((c.type==2&&(c.byte(c.u+0x2f3)||c.byte(c.u+0x2f4)))?0x80122c14:0x80122c24);continue;
 }
 case 0x80122bc8: {
  auto result=c.call(0x80123210,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.type;q.arguments[q.count++]=0;});
  if(!result)return {};
  {c.pc=0x80122bd0;continue;}
 }
 case 0x80122bd0: {
  auto result=c.call(0x80120fe4,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  c.store(c.m,10);{c.pc=0x80122bdc;continue;}
 }
 case 0x80122bdc: {
  auto result=c.call(0x8013898c,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x801231dc;continue;}
 }
 case 0x80122c14: {
  auto result=c.call(0x80122068,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.type;q.arguments[q.count++]=0;});
  if(!result)return {};
  {c.pc=0x80122c38;continue;}
 }
 case 0x80122c24: {
  auto result=c.call(0x80122068,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.type;q.arguments[q.count++]=1;});
  if(!result)return {};
  {c.pc=0x80122c38;continue;}
 }
 case 0x80122c34: {
  auto result=c.call(0x80122068,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.type;q.arguments[q.count++]=0;});
  if(!result)return {};
  {c.pc=0x80122c38;continue;}
 }
 case 0x80122c38: {
  auto result=c.call(0x80138990,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x80122c3c;continue;}
 }
 case 0x80122c3c: {
  auto result=c.call(0x80138988,[&](auto&){ });
  if(!result)return {};
  c.store(c.m,7);{c.pc=0x80122c48;continue;}
 }
 case 0x80122c48: {
  auto result=c.call(0x8011f5f0,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x80122c54;continue;}
 }
 case 0x80122c54: {
  auto result=c.call(0x80121d0c,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=c.word(c.m+0x540);});
  if(!result)return {};
  if(*result){c.progress=1;}
  {c.pc=0x801231dc;continue;}
 }
 case 0x80122cb0: {
  c.store(c.m,13);{c.pc=0x80122cbc;continue;}
 }
 case 0x80122cbc: {
  auto result=c.call(0x80121c98,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  {c.pc=0x80122cc0;continue;}
 }
 case 0x80122cc0: {
  auto result=c.call(0x8011f5f0,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x801231dc;continue;}
 }
 case 0x80122ccc: {
  auto result=c.call(0x80121af4,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  if(*result){c.progress=1;}
  {c.pc=0x801231dc;continue;}
 }
 // State10 resolves its own selected weapon (28-byte records at M80),
 // help ID at weapon70,then enables help and stores state11. A completed
 // cursor query with unknown output holds only at its first dereference.
 case 0x80122cec: {
  auto result=c.call(0x80124cb8,[&](auto& q){q.privateCursorOutput=&c.privateCursor;q.arguments[q.count++]=c.m;q.arguments[q.count++]=c.word(c.m+0x540);});
  if(!result)return {};
  {c.pc=0x80122cf0;continue;}
 }
 case 0x80122cf0: {
  if(!c.privateCursor){c.pending=0x80124cb8;return {};}
  c.weapon=c.word(c.m+0x80+28*c.word(*c.privateCursor));{c.pc=0x80122d08;continue;}
 }
 case 0x80122d08: {
  auto result=c.call(0x80216954,[&](auto& q){q.arguments[q.count++]=c.word(c.weapon+0x70);});
  if(!result)return {};
  {c.pc=0x80122d10;continue;}
 }
 case 0x80122d10: {
  auto result=c.call(0x8021690c,[&](auto& q){q.arguments[q.count++]=1;});
  if(!result)return {};
  c.store(c.m,11);{c.pc=0x801231dc;continue;}
 }
 case 0x80122d20: {
  c.type=c.word(c.m+0x540);c.buffer=c.b+0x428;{c.pc=0x80122d34;continue;}
 }
 case 0x80122d34: {
  auto result=c.call(0x80124cb8,[&](auto& q){q.privateCursorOutput=&c.privateCursor;q.arguments[q.count++]=c.m;q.arguments[q.count++]=c.type;});
  if(!result)return {};
  {c.pc=0x80122d38;continue;}
 }
 // State11 target bank B428: signed selected byte ab7 -> signed permutation
 // a6d ->36-byte pair row+4, signed half ID/part. Capture pair BEFORE sampling;
 // remap to first matching refreshed pair,or0,using signed fresh count a6c.
 case 0x80122d38: {
  const auto old=sb(c.byte(c.buffer+0xab7));
  const auto ordinal=sb(c.byte(c.buffer+0xa6d+uint32_t(old)));
  const auto row=c.buffer+4+36*uint32_t(ordinal);
  c.id=sh(c.half(row));c.part=sh(c.half(row+2));{c.pc=0x80122d64;continue;}
 }
 case 0x80122d64: {
  auto result=c.call(0x800ff5a0,[&](auto& q){q.arguments[q.count++]=c.buffer;});
  if(!result)return {};
  {c.pc=0x80122d68;continue;}
 }
 case 0x80122d68: {
  const auto count=sb(c.byte(c.buffer+0xa6c));int32_t selected=0;
  for(;selected<count;++selected){const auto permutation=sb(c.byte(c.buffer+0xa6d+uint32_t(selected)));
   const auto row=c.buffer+4+36*uint32_t(permutation);
   if(sh(c.half(row))==c.id&&sh(c.half(row+2))==c.part)break;
  }
  if(selected>=count)selected=0;
  c.put(c.buffer+0xab7,uint8_t(selected));c.pc=0x80122db4;continue;

 }
 case 0x80122db4: {
  if(!c.privateCursor){c.pending=0x80124cb8;return {};}
  c.weapon=c.word(c.m+0x80+28*c.word(*c.privateCursor));
  c.pc=(c.word(c.weapon+0x64)&0x01000000)?0x80122de8:0x80122e88;continue;
 }
 // Weapon flag01000000 permits independent forward80000 then reverse40000
 // repeat queries. Byte writes wrap before signed bound checks; sounds need
 // fresh count>=2. Do not collapse both directions into one input delta.
 case 0x80122de8: {
  auto result=c.call(0x80119f60,[&](auto& q){q.arguments[q.count++]=0x80000;});
  if(!result)return {};
  if(*result){c.put(c.buffer+0xab7,uint8_t(c.byte(c.buffer+0xab7)+1));
   if(sb(c.byte(c.buffer+0xab7))>=sb(c.byte(c.buffer+0xa6c)))c.put(c.buffer+0xab7,0);
   if(sb(c.byte(c.buffer+0xa6c))>=2){c.pc=0x80122e34;continue;}
  }{c.pc=0x80122e3c;continue;}
 }
 case 0x80122e34: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=0x802efe98;});
  if(!result)return {};
  {c.pc=0x80122e3c;continue;}
 }
 case 0x80122e3c: {
  auto result=c.call(0x80119f60,[&](auto& q){q.arguments[q.count++]=0x40000;});
  if(!result)return {};
  if(*result){c.put(c.buffer+0xab7,uint8_t(c.byte(c.buffer+0xab7)-1));
   if(sb(c.byte(c.buffer+0xab7))<0)c.put(c.buffer+0xab7,uint8_t(c.byte(c.buffer+0xa6c)-1));
   if(sb(c.byte(c.buffer+0xa6c))>=2){c.pc=0x80122e80;continue;}
  }{c.pc=0x80122e88;continue;}
 }
 case 0x80122e80: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=0x802efe98;});
  if(!result)return {};
  {c.pc=0x80122e88;continue;}
 }
 // Target cancel trigger200 precedes confirm100 and skips confirm/audience
 // entirely when taken. Accepted confirm stores state29 BEFORE target End,
 // keeps captured type for later costs,then still visits physical-X audience.
 case 0x80122e88: {
  auto result=c.call(0x8011a06c,[&](auto& q){q.arguments[q.count++]=0x200;});
  if(!result)return {};
  c.pc=*result?0x80122e98:0x80122f08;continue;
 }
 case 0x80122e98: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=2521;});
  if(!result)return {};
  {c.pc=0x80122ea0;continue;}
 }
 case 0x80122ea0: {
  auto result=c.call(0x801209e8,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  c.store(c.m,6);c.store(c.b+0xef4,c.word(c.b+0xef4)&~4u);{c.pc=0x80122ebc;continue;}
 }
 case 0x80122ebc: {
  auto result=c.call(0x8021690c,[&](auto& q){q.arguments[q.count++]=0;});
  if(!result)return {};
  {c.pc=0x80122ec4;continue;}
 }
 case 0x80122ec4: {
  auto result=c.call(0x801264cc,[&](auto& q){q.arguments[q.count++]=c.u;});
  if(!result)return {};
  const auto type=c.word(c.m+0x540);
  if((type==0&&(c.word(c.b+0xef4)&0x200))||(type==1&&(c.word(c.b+0xef4)&0x400))){c.store(c.m,1);{c.pc=0x80122efc;continue;}}{c.pc=0x801231dc;continue;}
 }
 case 0x80122efc: {
  auto result=c.call(0x80138988,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x801231dc;continue;}
 }
 case 0x80122f08: {
  auto result=c.call(0x8011a06c,[&](auto& q){q.arguments[q.count++]=0x100;});
  if(!result)return {};
  c.pc=*result?0x80122f18:0x80122fb0;continue;
 }
 case 0x80122f18: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=2520;});
  if(!result)return {};
  {c.pc=0x80122f24;continue;}
 }
 case 0x80122f24: {
  auto result=c.call(0x80138998,[&](auto& q){q.arguments[q.count++]=1;q.arguments[q.count++]=1;});
  if(!result)return {};
  c.store(c.m,29);{c.pc=0x80122f34;continue;}
 }
 // After target End, fresh flags OR4 and fresh mode/operation select item
 // operation2->mode8/literal803553a8 or3->mode5/literal80355468. Retained
 // entry type is not replaced by a callback mutation of current M540.
 case 0x80122f34: {
  auto result=c.call(0x801209e8,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  c.store(c.b+0xef4,c.word(c.b+0xef4)|4);
  if(c.type==2&&sb(c.byte(c.u+0x20))==0){const auto operation=c.byte(c.m+0x550);
   if(operation==2){c.put(c.u+0x20,8);{c.pc=0x80122f88;continue;}}
   if(operation==3){c.put(c.u+0x20,5);{c.pc=0x80122fa4;continue;}}
  }{c.pc=0x80122fac;continue;}
 }
 case 0x80122f88: {
  auto result=c.call(0x80126840,[&](auto& q){q.arguments[q.count++]=c.u;q.arguments[q.count++]=0x803553a8;});
  if(!result)return {};
  {c.pc=0x80122fac;continue;}
 }
 case 0x80122fa4: {
  auto result=c.call(0x80126840,[&](auto& q){q.arguments[q.count++]=c.u;q.arguments[q.count++]=0x80355468;});
  if(!result)return {};
  {c.pc=0x80122fac;continue;}
 }
 case 0x80122fac: {
  auto result=c.call(0x8021690c,[&](auto& q){q.arguments[q.count++]=2;});
  if(!result)return {};
  {c.pc=0x80122fb0;continue;}
 }
 case 0x80122fb0: {
  auto result=c.call(0x8011f524,[&](auto&){ });
  if(!result)return {};
  if(*result){c.put(c.b+0x1c74,1);{c.pc=0x80122fc8;continue;}}{c.pc=0x801231dc;continue;}
 }
 case 0x80122fc8: {
  auto result=c.call(0x801209e8,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  {c.pc=0x80122fd0;continue;}
 }
 case 0x80122fd0: {
  auto result=c.call(0x8021690c,[&](auto& q){q.arguments[q.count++]=0;});
  if(!result)return {};
  {c.pc=0x801231dc;continue;}
 }
 // Paired setup states14/15,19/20,24/25 retain entry state for flag1/0,
 // store16/21/26,restore,then run that Main immediately. Direct Main skips setup.
 // State12 cancel-check instead stores13 but returns through common tail.
 case 0x80122fd8: {
  c.pc=c.state==14?0x80122fe8:0x80122ff8;continue;
 }
 case 0x80122fe8: {
  auto result=c.call(0x801215b8,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=1;});
  if(!result)return {};
  c.store(c.m,16);{c.pc=0x80123004;continue;}
 }
 case 0x80122ff8: {
  auto result=c.call(0x801215b8,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=0;});
  if(!result)return {};
  c.store(c.m,16);{c.pc=0x80123004;continue;}
 }
 case 0x80123004: {
  auto result=c.call(0x8011f5f0,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x8012300c;continue;}
 }
 case 0x8012300c: {
  auto result=c.call(0x80121354,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  if(*result){c.progress=1;}
  {c.pc=0x801231dc;continue;}
 }
 case 0x80123064: {
  c.pc=c.state==19?0x80123078:0x80123088;continue;
 }
 case 0x80123078: {
  auto result=c.call(0x80121928,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=1;});
  if(!result)return {};
  c.store(c.m,21);{c.pc=0x80123094;continue;}
 }
 case 0x80123088: {
  auto result=c.call(0x80121928,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=0;});
  if(!result)return {};
  c.store(c.m,21);{c.pc=0x80123094;continue;}
 }
 case 0x80123094: {
  auto result=c.call(0x8011f5f0,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x8012309c;continue;}
 }
 case 0x8012309c: {
  auto result=c.call(0x8012165c,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  if(*result){c.progress=1;}
  {c.pc=0x801231dc;continue;}
 }
 case 0x801230f8: {
  c.pc=c.state==24?0x80123108:0x80123118;continue;
 }
 case 0x80123108: {
  auto result=c.call(0x801212bc,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=1;});
  if(!result)return {};
  c.store(c.m,26);{c.pc=0x80123124;continue;}
 }
 case 0x80123118: {
  auto result=c.call(0x801212bc,[&](auto& q){q.arguments[q.count++]=c.b;q.arguments[q.count++]=0;});
  if(!result)return {};
  c.store(c.m,26);{c.pc=0x80123124;continue;}
 }
 case 0x80123124: {
  auto result=c.call(0x8011f5f0,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x8012312c;continue;}
 }
 case 0x8012312c: {
  auto result=c.call(0x80121098,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  if(*result){c.progress=1;}
  {c.pc=0x801231dc;continue;}
 }
 case 0x80123144: {
  auto result=c.call(0x80120990,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  c.store(c.m,28);{c.pc=0x80123150;continue;}
 }
 case 0x80123150: {
  auto result=c.call(0x8011f5f0,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x80123158;continue;}
 }
 // Protect state28: cancel200 first (sound2521,End,state20); confirm100
 // stores29 before End with no imported target-state sound/help sequence.
 case 0x80123158: {
  auto result=c.call(0x8011a06c,[&](auto& q){q.arguments[q.count++]=0x200;});
  if(!result)return {};
  c.pc=*result?0x80123168:0x80123184;continue;
 }
 case 0x80123168: {
  auto result=c.call(0x800d98bc,[&](auto& q){q.arguments[q.count++]=2521;});
  if(!result)return {};
  {c.pc=0x80123170;continue;}
 }
 case 0x80123170: {
  auto result=c.call(0x80120834,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  c.store(c.m,20);{c.pc=0x801231dc;continue;}
 }
 case 0x80123184: {
  auto result=c.call(0x8011a06c,[&](auto& q){q.arguments[q.count++]=0x100;});
  if(!result)return {};
  if(*result){c.store(c.m,29);{c.pc=0x8012319c;continue;}}{c.pc=0x801231dc;continue;}
 }
 case 0x8012319c: {
  auto result=c.call(0x80120834,[&](auto& q){q.arguments[q.count++]=c.b;});
  if(!result)return {};
  {c.pc=0x801231dc;continue;}
 }
 // State29 selects attack with retained U/M before fresh flag clears:
 // M4 bit01000000,Bef8 bit1,then OffLight(0,0),direct return1. No damage,
 // turn consumption or original event completion is inferred from this return.
 case 0x801231ac: {
  auto result=c.call(0x80120268,[&](auto& q){q.arguments[q.count++]=c.u;q.arguments[q.count++]=c.m;});
  if(!result)return {};
  c.store(c.m+4,c.word(c.m+4)&~0x01000000u);c.store(c.b+0xef8,c.word(c.b+0xef8)&~1u);{c.pc=0x801231d0;continue;}
 }
 case 0x801231d0: {
  auto result=c.call(0x80138998,[&](auto& q){q.arguments[q.count++]=0;q.arguments[q.count++]=0;});
  if(!result)return {};
  return finish(1);
 }
 // Pending B1c74 reenters state dispatch WITHOUT clearing progress; ordinary
 // progress reentry clears it at22a44. No pending/progress returns engaged0:
 // this invocation completed,so the simulation owner may start its next tick.
 case 0x801231dc: {
  if(c.byte(c.b+0x1c74)){c.pc=0x801231e8;continue;}
  if(c.progress){c.pc=0x80122a44;continue;}
  return finish(0);
 }
 case 0x801231e8: {
  auto result=c.call(0x8011f5f0,[&](auto&){ });
  if(!result)return {};
  {c.pc=0x80122a48;continue;}
 }
 default:return {};
 }
}
}
