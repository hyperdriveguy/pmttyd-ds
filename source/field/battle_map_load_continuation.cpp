#include "battle_map_load_continuation.hpp"
namespace ttyd {
namespace {
constexpr uint32_t WorkGlobal=0x8041e660,ActiveGroup=0x8041e664,PrimaryHeap=0x8041e808;
constexpr uint32_t GroupBytes=0x2f4,EntryBytes=0x178;
constexpr uint32_t Synchronize=0x80005ce4,MapFree=0x8002fdc4,ArchiveContains=0x80078444,SmartEntry=0x801526d4,Clear=0x800050b4,LoadEntry=0x80028f18;
int32_t signedWord(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000ll);}
bool same(const BattleMapLoadServices&a,const BattleMapLoadServices&b){return a.user==b.user&&a.current==b.current&&a.call==b.call&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.memory.user==b.memory.user&&a.memory.word==b.memory.word&&a.memory.storeWord==b.memory.storeWord;}
enum class Step{Sync,Capture,Loop,FreeTail,ZeroTail,ArchiveQuery,SmartD,SmartT,FreeT,FreeD,ZeroDT,SmartArchive,FreeArchive,ZeroArchive,Next,BattleBank,ClearBank,LoadBase,PublishBase,LoadForeground,PublishForeground,Done};
}
struct BattleMapLoadCursor::State {
 BattleMapLoadServices owner;
 Step step=Step::Sync;
 uint32_t base=0,foreground=0,capturedWork=0,entry=0,offset=0,index=0,tailEntry=0,battleWork=0,pending=0,tailPointer=0,dPointer=0;
 std::optional<BattleMapLoadRequest> held;
 bool invoking=false,invalid=false;
 bool current(){if(invalid||!owner.current(owner.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t a)const{return owner.memory.word(owner.memory.user,a);}
 void store(uint32_t a,uint32_t v)const{owner.memory.storeWord(owner.memory.user,a,v);}
 template<class Make> bool call(uint32_t site,uint32_t function,unsigned count,Make make,bool needsResult=false){
  pending=function;if(!current())return false;
  if(!held){BattleMapLoadRequest q;q.function=function;q.callsite=site;q.count=count;q.arguments=make();held=std::move(q);}
  return owner.call&&owner.call(owner.user,*held)&&current()&&(!needsResult||held->result.has_value());
 }
 void accepted(){held.reset();pending=0;}
};
uint32_t BattleMapLoadCursor::pending()const{return state?state->pending:0;}
bool BattleMapLoadCursor::complete()const{return state&&state->step==Step::Done&&!state->invalid;}
bool resumePackedBattleMapLoad(uint32_t base,uint32_t foreground,const BattleMapLoadServices&s,BattleMapLoadCursor&c){
 if(!c.state){if(!s.lifetime||!s.current||!s.memory.word||!s.memory.storeWord)return false;c.state=std::make_shared<BattleMapLoadCursor::State>();c.state->owner=s;c.state->base=base;c.state->foreground=foreground;}
 auto&p=*c.state;if(p.invoking||!same(p.owner,s)||!p.current())return false;
 p.invoking=true;struct Guard{bool&v;~Guard(){v=false;}}guard{p.invoking};
 auto none=[] {return std::array<uint32_t,3>{};};
 while(p.current())switch(p.step){
 case Step::Sync:if(!p.call(0x80028944,Synchronize,0,none))return false;p.accepted();p.step=Step::Capture;break;
 case Step::Capture:p.capturedWork=p.word(WorkGlobal);p.store(ActiveGroup,0);p.entry=p.capturedWork;p.step=Step::Loop;break;
 case Step::Loop:
  if(signedWord(p.index)>=signedWord(p.word(p.capturedWork))){p.step=Step::BattleBank;break;}
  // r29/r27 remain captured; original r26 uses fresh current globals per turn.
  {const auto group=p.word(ActiveGroup);const auto work=p.word(WorkGlobal);p.tailEntry=work+group*GroupBytes+p.offset;}
  p.tailPointer=p.word(p.tailEntry+0x16c);p.step=p.tailPointer?Step::FreeTail:Step::ZeroTail;break;
 case Step::FreeTail:if(!p.call(0x80028984,MapFree,2,[&]{return std::array<uint32_t,3>{p.word(PrimaryHeap),p.tailPointer,0};}))return false;p.accepted();p.step=Step::ZeroTail;break;
 case Step::ZeroTail:
  p.store(p.tailEntry+0x168,0);p.store(p.tailEntry+0x16c,0);
  p.store(p.entry+0x170,0);p.store(p.entry+0x178,0);p.store(p.entry+0x174,0);
  p.dPointer=p.word(p.entry+0x7c);p.step=p.dPointer?Step::ArchiveQuery:Step::Next;break;
 case Step::ArchiveQuery:
  if(!p.call(0x800289ac,ArchiveContains,1,[&]{return std::array<uint32_t,3>{p.dPointer,0,0};},true))return false;
  p.step=*p.held->result?Step::SmartArchive:Step::SmartD;p.accepted();break;
 case Step::SmartD:
  if(!p.call(0x800289c0,SmartEntry,2,[&]{return std::array<uint32_t,3>{p.word(p.entry+0x7c),p.word(p.entry+0x80),0};},true))return false;
  p.store(p.entry+0x170,*p.held->result);p.accepted();p.step=Step::SmartT;break;
 case Step::SmartT:
  if(!p.call(0x800289d0,SmartEntry,2,[&]{return std::array<uint32_t,3>{p.word(p.entry+0x84),p.word(p.entry+0x88),0};},true))return false;
  p.store(p.entry+0x178,*p.held->result);p.accepted();p.step=Step::FreeT;break;
 case Step::FreeT:if(!p.call(0x800289e0,MapFree,2,[&]{return std::array<uint32_t,3>{p.word(PrimaryHeap),p.word(p.entry+0x84),0};}))return false;p.accepted();p.step=Step::FreeD;break;
 case Step::FreeD:if(!p.call(0x800289ec,MapFree,2,[&]{return std::array<uint32_t,3>{p.word(PrimaryHeap),p.word(p.entry+0x7c),0};}))return false;p.accepted();p.step=Step::ZeroDT;break;
 case Step::ZeroDT:p.store(p.entry+0x84,0);p.store(p.entry+0x7c,0);p.step=Step::Next;break;
 case Step::SmartArchive:
  if(!p.call(0x80028a08,SmartEntry,2,[&]{return std::array<uint32_t,3>{p.word(p.entry+0x8c),p.word(p.entry+0x90),0};},true))return false;
  p.store(p.entry+0x174,*p.held->result);p.accepted();p.step=Step::FreeArchive;break;
 case Step::FreeArchive:if(!p.call(0x80028a18,MapFree,2,[&]{return std::array<uint32_t,3>{p.word(PrimaryHeap),p.word(p.entry+0x8c),0};}))return false;p.accepted();p.step=Step::ZeroArchive;break;
 case Step::ZeroArchive:p.store(p.entry+0x8c,0);p.step=Step::Next;break;
 case Step::Next:p.offset+=EntryBytes;p.entry+=EntryBytes;++p.index;p.step=Step::Loop;break;
 case Step::BattleBank:p.battleWork=p.word(WorkGlobal)+GroupBytes;p.store(ActiveGroup,1);p.step=Step::ClearBank;break;
 case Step::ClearBank:if(!p.call(0x80028a58,Clear,3,[&]{return std::array<uint32_t,3>{p.battleWork,0,GroupBytes};}))return false;p.accepted();p.step=Step::LoadBase;break;
 case Step::LoadBase:if(!p.call(0x80028a68,LoadEntry,3,[&]{return std::array<uint32_t,3>{p.battleWork,0,p.base};}))return false;p.accepted();p.step=Step::PublishBase;break;
 case Step::PublishBase:p.store(p.battleWork,1);p.step=p.foreground?Step::LoadForeground:Step::Done;break;
 case Step::LoadForeground:if(!p.call(0x80028a88,LoadEntry,3,[&]{return std::array<uint32_t,3>{p.battleWork,1,p.foreground};}))return false;p.accepted();p.step=Step::PublishForeground;break;
 case Step::PublishForeground:p.store(p.battleWork,2);p.step=Step::Done;break;
 case Step::Done:return true;
 }
 return false;
}
}
