#include "source_gx_inherited_state.hpp"
#include <cassert>
#include <vector>
#include <functional>
#include <iostream>
using namespace ttyd;
namespace {
struct Owner{bool live=true,cold=true;unsigned reads=0,colds=0;std::function<void()>callback;};
bool live(void*p){auto&s=*static_cast<Owner*>(p);++s.reads;if(s.callback)s.callback();return s.live;}
bool cold(void*p){auto&s=*static_cast<Owner*>(p);++s.colds;return s.cold;}
SourceGxInheritedBinding binding(const std::shared_ptr<Owner>&p){return {p,p.get(),live,cold};}
SourceGxInheritedCall call(uint32_t fn,std::initializer_list<uint32_t>a){SourceGxInheritedCall q;q.function=fn;q.count=unsigned(a.size());std::copy(a.begin(),a.end(),q.integers.begin());return q;}
const SourceGxInheritedCall&find(const SourceGxInheritedSnapshot&s,uint32_t fn,uint32_t index=0){for(unsigned i=0;i<s.size();++i){const auto&q=s.evidence()[i];if(q.function==fn&&q.integers[0]==index)return q;}assert(false);return s.evidence()[0];}
void coldProfile(){
 auto p=std::make_shared<Owner>();SourceGxInheritedState s;assert(!s.current());assert(!s.initializeCold());assert(!s.capture());assert(s.bind(binding(p)));assert(!s.bind(binding(p)));assert(!s.capture());
 assert(s.initializeCold());assert(s.coldComplete());assert(p->colds==1);assert(s.initializeCold());assert(p->colds==1);
 auto one=s.capture();auto all=s.capture(16);assert(one&&all&&one->current()&&all->current());assert(!s.capture(0)&&!s.capture(17));assert(all->size()==84);
 for(unsigned stage=0;stage<16;++stage){const auto&a=find(*all,0x802b8be0,stage);assert(a.callsite==0x802b3090&&a.integers[1]==6);const auto&b=find(*all,0x802b8c3c,stage);assert(b.callsite==0x802b309c&&b.integers[1]==0);assert(find(*all,0x802b8c98,stage).callsite==0x802b30ac);assert(find(*all,0x802b87d0,stage).callsite==0x802b3130);}
 assert((find(*all,0x802b8ce0).integers==std::array<uint32_t,8>({0,0,1,2,3,0,0,0})));
 const auto&fog=find(*all,0x802b8ff4);assert(fog.callsite==0x802b31a8&&fog.fog&&fog.integers[1]==0);assert((fog.floatBits==std::array<uint32_t,4>({0,0x3f800000,0x3dcccccd,0x3f800000})));
 // Cold cull was earlier in the omitted parent. This bounded contribution does
 // not invent it or any unowned preceding GX state.
 for(unsigned i=0;i<all->size();++i)assert(all->evidence()[i].function!=0x802b5c8c);
 assert(s.apply(call(0x802b5c8c,{0})));assert(s.capture());
 // Real camera4 alpha suppression affects new captures, not copied camera8
 // packets. Genuine camera8 DisplayQueue alpha1 restores the current profile.
 assert(s.apply(call(0x802b93ac,{0})));assert(!s.capture());assert(one->current());assert(s.apply(call(0x802b93ac,{1})));assert(s.capture());
 assert(s.apply(call(0x802b9380,{0})));assert(!s.capture());assert(all->current());assert(s.apply(call(0x802b9380,{1})));assert(s.capture());
 assert(s.apply(call(0x802b8c98,{2,1,0})));assert(s.capture(1)&&!s.capture(3));assert(all->current());assert(s.apply(call(0x802b8c98,{2,0,0})));assert(s.capture(3));
 assert(s.apply(call(0x802b8ce0,{0,3,2,1,0})));assert(!s.capture());assert(one->current());assert(s.apply(call(0x802b8ce0,{0,0,1,2,3})));assert(s.capture());
 assert(s.apply(call(0x802b87ac,{1})));assert(!s.capture());assert(s.apply(call(0x802b87ac,{0})));assert(s.capture());
 auto f=call(0x802b8ff4,{2,0xff});f.fog=true;assert(s.apply(f));assert(!s.capture());f.integers[0]=0;assert(s.apply(f));assert(s.capture());
 assert(s.apply(call(0x802b9540,{1,255})));assert(!s.capture());assert(s.apply(call(0x802b9540,{0,0})));assert(s.capture());
 assert(s.apply(call(0x802b9440,{1,0})));assert(!s.capture());assert(s.apply(call(0x802b9440,{0,0})));assert(s.capture());
 assert(s.apply(call(0x802b9514,{0})));assert(!s.capture());assert(s.apply(call(0x802b9514,{1})));assert(s.capture());
 assert(s.apply(call(0x802b957c,{0,1})));assert(!s.capture());assert(s.apply(call(0x802b957c,{1,1})));assert(s.capture());
 assert(s.invalidate());assert(!s.capture());assert(s.initializeCold());assert(!s.capture());assert(one->current()); // no cold replay to repair unknown history
 s.retire();assert(!one->current()&&!s.current()&&!s.capture()&&!s.initializeCold());
}
void malformedAtomic(){auto p=std::make_shared<Owner>();SourceGxInheritedState s;assert(s.bind(binding(p))&&s.initializeCold());
 std::vector<SourceGxInheritedCall>bad{call(0,{1}),call(0x802b8c98,{16,0,0}),call(0x802b8c98,{0,4,0}),call(0x802b8ce0,{0,0,1,2,4}),call(0x802b87d0,{16}),call(0x802b87ac,{5}),call(0x802b857c,{0,9,0}),call(0x802b9380,{2}),call(0x802b9540,{1,256}),call(0x802b9440,{8,0}),call(0x802b957c,{2,0}),call(0x802b8ff4,{0,0})};
 for(auto q:bad){assert(!s.apply(q));assert(s.capture());}
 auto q=call(0x802b93ac,{1});q.count=9;assert(!s.apply(q));q=call(0x802b93ac,{1});q.integers[7]=1;assert(!s.apply(q));q=call(0x802b93ac,{1});q.fog=true;assert(!s.apply(q));assert(s.capture());
}
void admissionLifetime(){
 auto p=std::make_shared<Owner>();SourceGxInheritedState s;auto b=binding(p);b.coldCurrent=nullptr;assert(s.bind(b));assert(!s.initializeCold());
 SourceGxInheritedState t;p->cold=false;assert(t.bind(binding(p)));assert(!t.initializeCold()&&!t.capture());p->cold=true;assert(t.initializeCold());auto packet=t.capture();assert(packet);
 p->live=false;assert(!packet->current());p->live=true;assert(!packet->current()&&!t.current());
 SourceGxInheritedState u;assert(u.bind(binding(p)));assert(u.apply(call(0x802b9380,{1})));assert(!u.initializeCold()); // cannot overwrite actual reached history with cold defaults
 std::shared_ptr<const SourceGxInheritedSnapshot>old;
 {SourceGxInheritedState v;assert(v.bind(binding(p))&&v.initializeCold());old=v.capture();assert(old->current());}assert(!old->current());
 SourceGxInheritedState replacement;assert(replacement.bind(binding(p))&&replacement.initializeCold());assert(replacement.capture()->current()&&!old->current());
}
void strongProof(){
 std::weak_ptr<Owner>weak;std::shared_ptr<const SourceGxInheritedSnapshot>packet;
 {auto p=std::make_shared<Owner>();weak=p;SourceGxInheritedState s;assert(s.bind(binding(p))&&s.initializeCold());packet=s.capture();p.reset();assert(!weak.expired()&&packet->current());}
 assert(!weak.expired()&&!packet->current());packet.reset();assert(weak.expired());
 auto p=std::make_shared<Owner>();SourceGxInheritedState s;auto b=binding(p);
 b.coldCurrent=[](void*p){auto&o=*static_cast<Owner*>(p);if(o.callback)o.callback();return true;};
 assert(s.bind(b));p->callback=[&]{s.retire();};assert(!s.initializeCold());p->callback={};assert(!s.coldComplete()&&!s.capture());
}
void reentry(){
 auto p=std::make_shared<Owner>();SourceGxInheritedState s;assert(s.bind(binding(p))&&s.initializeCold());auto snapshot=s.capture();unsigned called=0;
 p->callback=[&]{++called;assert(!s.current());assert(!s.capture());assert(!s.apply(call(0x802b9380,{0})));assert(!s.initializeCold());assert(!s.invalidate());assert(!snapshot->current());};
 assert(s.current()&&called==1);assert(s.capture()&&called==2);p->callback={};assert(s.capture());
 p->callback=[&]{s.retire();};assert(!snapshot->current());p->callback={};assert(!s.current()&&!snapshot->current());
}
}
int main(){coldProfile();malformedAtomic();admissionLifetime();strongProof();reentry();std::cout<<"source GX inherited state tests passed\n";}
