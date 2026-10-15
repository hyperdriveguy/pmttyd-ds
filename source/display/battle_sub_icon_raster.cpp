#include "battle_sub_icon_raster.hpp"
#include <cstring>
namespace ttyd {namespace {
int32_t sw(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
float converted(uint32_t n,double bias,bool sign=true){const uint64_t raw=(uint64_t(0x43300000)<<32)|(sign?n^0x80000000u:n);double v;std::memcpy(&v,&raw,8);return float(v-bias);}
bool same(const BattleSubIconRasterRequest&a,const BattleSubIconRasterRequest&b){return a.function==b.function&&a.callsite==b.callsite&&a.work==b.work&&a.row==b.row&&a.label==b.label&&a.enabled==b.enabled&&a.icon==b.icon&&a.cost==b.cost&&a.bp==b.bp&&a.flag==b.flag&&a.stackFlag==b.stackFlag&&std::memcmp(a.matrix.data(),b.matrix.data(),sizeof(a.matrix))==0;}
bool same(const BattleSubIconRasterServices&a,const BattleSubIconRasterServices&b){const auto&m=a.actors;const auto&n=b.actors;return a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.user==b.user&&a.current==b.current&&a.request==b.request&&a.matrix==b.matrix&&a.truncate==b.truncate&&m.user==n.user&&m.byte==n.byte&&m.audienceLifecycle.user==n.audienceLifecycle.user&&m.audienceLifecycle.event==n.audienceLifecycle.event&&m.memory.user==n.memory.user&&m.memory.word==n.memory.word&&m.memory.half==n.memory.half&&m.memory.storeWord==n.memory.storeWord&&m.memory.storeHalf==n.memory.storeHalf&&m.memory.storeByte==n.memory.storeByte&&m.memory.flushBlock==n.memory.flushBlock&&m.memory.systemCall==n.memory.systemCall;}
struct Running{bool&f;explicit Running(bool&v):f(v){f=true;}~Running(){f=false;}};
}
struct BattleSubIconRasterCursor::State {
 BattleSubIconRasterServices services;const BattleSubIconRasterRequest* borrowed=nullptr;BattleSubIconRasterRequest input;
 uint32_t phase=0,pending=0,battle=0,cursor=0,camera=0,top=0,alpha=255,shade=0,color=0,texture=0,icon=0,width=0,text=0;
 float scroll=0,fraction=0,numeric=0,labelScale=0,cachedX=0,cachedOffset=0;double cachedBias=0;
 std::array<uint8_t,3> rgb{};BattleActMatrix transformed{},translation{},scale{};
 uint32_t group=0,digitCount=0,digitIndex=0;std::array<uint32_t,3> digits{},positions{};int32_t extraFirst=0,extraSecond=0;
 bool invalid=false;std::optional<BattleSubIconRasterCall> held;std::optional<BattleAudienceMatrixRequest> math;std::optional<float> conversion;
 bool valid(){if(invalid)return false;if(!services.current||!services.current(services.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t a)const{return services.actors.memory.word(services.actors.memory.user,a);}
 uint16_t half(uint32_t a)const{return services.actors.memory.half(services.actors.memory.user,a);}
 uint8_t byte(uint32_t a)const{return services.actors.byte(services.actors.user,a);}
 float constant(int32_t offset)const{return scalar(word(0x80427440u+uint32_t(offset)));}
 double bias(uint32_t a=0x802ef780)const{const auto hi=word(a),lo=word(a+4);const uint64_t raw=(uint64_t(hi)<<32)|lo;double d;std::memcpy(&d,&raw,8);return d;}
 template<class Build>std::optional<uint32_t> call(uint32_t fn,uint32_t site,Build build){pending=fn;if(!held){held=BattleSubIconRasterCall{};held->function=fn;held->callsite=site;build(*held);}if(!services.request)return {};const auto v=services.request(services.user,*held);if(!valid()||!v)return {};held.reset();return v;}
 std::optional<uint32_t> call(uint32_t fn,uint32_t site){return call(fn,site,[](auto&){});}
 template<class Build>std::optional<BattleActMatrix> matrix(uint32_t fn,Build build){pending=fn;if(!math){math=BattleAudienceMatrixRequest{};math->function=fn;build(*math);}if(!services.matrix)return {};const bool completed=services.matrix(services.user,*math);if(!valid()||!completed)return {};const auto v=math->output;math.reset();return v;}
 template<class Build>std::optional<int32_t> truncate(uint32_t site,Build build){pending=site;if(!conversion)conversion=build();if(!services.truncate)return {};const auto v=services.truncate(services.user,*conversion);if(!valid()||!v)return {};conversion.reset();return v;}
 std::optional<BattleActMatrix> trans(int32_t x,int32_t y){return matrix(0x8029e230,[&](auto&q){const auto a=constant(x),b=constant(y),z=constant(-20500);q.xyz={a,b,z};});}
 std::optional<BattleActMatrix> uniform(int32_t n){return matrix(0x8029e2b0,[&](auto&q){const auto f=constant(n);q.xyz={f,f,f};});}
 std::optional<BattleActMatrix> concat(bool final){return matrix(0x8029dd64,[&](auto&q){q.left=final?transformed:input.matrix;q.right=final?scale:translation;});}
 void decompose(int32_t n){const auto hundreds=n/100,remainder=n-hundreds*100,tens=remainder/10,units=remainder-tens*10;digits={uint32_t(units),uint32_t(tens),uint32_t(hundreds)};if(hundreds>0){positions={28,14,0};digitCount=3;}else if(tens>0){positions={21,7,0};digitCount=2;}else{positions={14,0,0};digitCount=1;}digitIndex=0;}
 uint32_t digitTranslationSite()const{return group==0?0x80118964:group==1?0x80118b10:group==2?0x80118d4c:0x80118f70;}
 uint32_t digitFontSite()const{return group==0?0x80118990:group==1?0x80118b3c:group==2?0x80118dc0:0x80118fe4;}
};
BattleSubIconRasterCursor::BattleSubIconRasterCursor()=default;BattleSubIconRasterCursor::~BattleSubIconRasterCursor()=default;
bool BattleSubIconRasterCursor::active()const{return bool(state_);}bool BattleSubIconRasterCursor::stale()const{return state_&&state_->invalid;}uint32_t BattleSubIconRasterCursor::pending()const{return state_?state_->pending:0;}bool BattleSubIconRasterCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleSubIconRaster(const BattleSubIconRasterRequest& input,const BattleSubIconRasterServices&s,BattleSubIconRasterCursor&cursor){
 if(cursor.invoking_||!s.lifetime||!s.current||!s.actors.memory.word||!s.actors.memory.half||!s.actors.byte)return false;
 if(cursor.state_&&(cursor.state_->borrowed!=&input||!same(cursor.state_->input,input)||!same(cursor.state_->services,s)))return false;
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<BattleSubIconRasterCursor::State>();auto&c=*cursor.state_;c.services=s;c.input=input;c.borrowed=&input;}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return false;switch(c.phase){
 case 0:c.battle=currentBattleAddress(s.actors);c.cursor=c.word(input.work);c.phase=1;continue;
 case 1:{const auto v=c.call(0x8000f2fc,0x801184e0,[](auto&q){q.integers={8};});if(!v)return false;c.camera=*v;c.phase=2;continue;}
 case 2:{const auto v=c.truncate(0x801184f4,[&]{c.scroll=scalar(c.word(input.work+0x98));return c.scroll;});if(!v)return false;c.top=uint32_t(*v);c.fraction=sub(c.scroll,converted(c.top,c.bias()));c.phase=input.row==0?3:input.row==6?4:5;continue;}
 case 3:{const auto v=c.truncate(0x80118534,[&]{const auto one=c.constant(-20452),maximum=c.constant(-20360);return mul(maximum,sub(one,c.fraction));});if(!v)return false;c.alpha=uint32_t(*v);c.phase=5;continue;}
 case 4:{const auto v=c.truncate(0x80118550,[&]{return mul(c.constant(-20360),c.fraction);});if(!v)return false;c.alpha=uint32_t(*v);c.phase=5;continue;}
 case 5:
  if(c.top+input.row==c.word(c.cursor)){c.rgb={c.byte(0x80416d20),c.byte(0x80416d21),c.byte(0x80416d22)};c.shade=c.alpha;c.phase=7;continue;}
  c.rgb={255,255,255};c.phase=6;continue;
 case 6:{const auto v=c.truncate(0x801185bc,[&]{const auto d=c.bias(0x802ef808);const auto factor=c.constant(-20356),maximum=c.constant(-20360);return div(mul(factor,converted(uint8_t(c.alpha),d,false)),maximum);});if(!v)return false;c.shade=uint32_t(*v);c.phase=7;continue;}
 case 7:if(!c.call(0x80103a14,0x801185c8))return false;c.phase=8;continue;
 case 8:{const auto v=c.matrix(0x8029dd64,[&](auto&q){for(unsigned i=0;i<12;++i)q.left[i]=scalar(c.word(c.camera+0x11c+4*i));q.right=input.matrix;});if(!v)return false;c.transformed=*v;c.phase=9;continue;}
 case 9:if(!c.call(0x802ba2fc,0x801185e4,[&](auto&q){q.integers={0};q.matrix=c.transformed;}))return false;c.phase=10;continue;
 case 10:case 14:{const bool second=c.phase==14;const auto v=c.call(0x80037a80,second?0x80118670:0x80118600,[&](auto&q){const auto owner=c.word(c.battle+0x163f8),palette=c.word(c.word(owner+0xa0));q.integers={palette,second?3u:2u};});if(!v)return false;c.texture=*v;++c.phase;continue;}
 case 11:case 15:if(!c.call(0x802b7880,c.phase==11?0x8011860c:0x8011867c,[&](auto&q){q.integers={c.texture,0};}))return false;++c.phase;continue;
 case 12:case 13:case 16:{const auto phase=c.phase;const auto v=c.call(0x801037c4,phase==12?0x80118630:phase==13?0x80118654:0x801186a0,[&](auto&q){q.integers={c.rgb[0],c.rgb[1],c.rgb[2],c.shade};q.reals={c.constant(phase==12?-20352:phase==13?-20344:-20348),c.constant(-20416),c.constant(phase==12?-20348:-20340),c.constant(-20500)};});if(!v)return false;++c.phase;continue;}
 case 17:{const auto v=c.trans(-20336,-20332);if(!v)return false;c.translation=*v;++c.phase;continue;}
 case 18:{const auto v=c.matrix(0x8029e2b0,[&](auto&q){const auto a=c.constant(-20364),z=c.constant(-20452);q.xyz={a,a,z};});if(!v)return false;c.scale=*v;++c.phase;continue;}
 case 19:case 20:{const auto v=c.concat(c.phase==20);if(!v)return false;c.transformed=*v;++c.phase;continue;}
 case 21:if(!c.call(0x8001a4bc,input.label==1?0x80118724:0x80118750,[&](auto&q){const auto raw=c.word(0x80427440u+uint32_t(input.label==1?-20520:-20516));q.integers={input.label==1?16u:48u,input.icon?input.icon:403u,(raw&0xffffff00)|uint8_t(c.alpha)};q.matrix=c.transformed;}))return false;++c.phase;continue;
 case 22:if(!c.call(0x80077b4c,0x80118754))return false;++c.phase;continue;
 case 23:if(!c.call(0x800777e0,0x80118798,[&](auto&q){const auto raw=c.word(0x80427440u+uint32_t(input.label==1?8536:-20512));q.integers={(raw&0xffffff00)|uint8_t(c.alpha)};}))return false;++c.phase;continue;
 case 24:{const auto v=c.trans(-20328,-20324);if(!v)return false;c.translation=*v;c.phase=input.stackFlag==1?25:27;continue;}
 case 25:{const auto v=c.call(0x80075528,0x801187bc,[&](auto&q){q.integers={input.enabled};});if(!v)return false;c.width=*v&0xffff;c.phase=c.width>150?26:27;continue;}
 case 26:{const auto v=c.call(0x80075528,0x801187d0,[&](auto&q){q.integers={input.enabled};});if(!v)return false;c.width=*v&0xffff;const auto d=c.bias(0x802ef808);const auto numerator=c.constant(-20320);c.labelScale=div(numerator,converted(c.width,d,false));c.phase=28;continue;}
 case 27:c.labelScale=c.constant(-20404);c.phase=28;continue;
 case 28:{const auto v=c.matrix(0x8029e2b0,[&](auto&q){if(input.stackFlag==1){const auto yz=c.constant(-20404);q.xyz={c.labelScale,yz,yz};}else q.xyz={c.labelScale,c.labelScale,c.labelScale};});if(!v)return false;c.scale=*v;++c.phase;continue;}
 case 29:case 30:{const auto v=c.concat(c.phase==30);if(!v)return false;c.transformed=*v;++c.phase;continue;}
 case 31:if(!c.call(0x80076cdc,0x80118850,[&](auto&q){q.integers={input.enabled};q.matrix=c.transformed;}))return false;c.group=0;c.phase=32;continue;
 case 32:
  if(c.group==0&&!input.cost){c.group=1;continue;}
  if(c.group==1&&!input.bp){c.phase=44;continue;}
  c.decompose(sw(c.group==0?input.cost:input.bp));c.phase=33;continue;
 case 33:{const auto v=c.uniform(-20452);if(!v)return false;c.scale=*v;c.cachedX=c.constant(-20400);c.cachedOffset=c.constant(-20316);c.cachedBias=c.bias();c.phase=34;continue;}
 case 34:
  if(c.digitIndex>=c.digitCount){c.phase=39;continue;}
  {const auto v=c.matrix(0x8029e230,[&](auto&q){const auto y=c.constant(-20312),z=c.constant(-20500);q.xyz={add(c.cachedX,add(c.cachedOffset,converted(c.positions[c.digitIndex],c.cachedBias))),y,z};});if(!v)return false;c.translation=*v;c.phase=35;continue;}
 case 35:case 36:{const auto v=c.concat(c.phase==36);if(!v)return false;c.transformed=*v;++c.phase;continue;}
 case 37:if(!c.call(0x80076cdc,c.digitFontSite(),[&](auto&q){q.matrix=c.transformed;q.privateText=std::array<uint8_t,2>{uint8_t(c.digits[c.digitIndex]+48),0};}))return false;++c.digitIndex;c.phase=34;continue;
 case 39:{const auto v=c.trans(-20320,-20324);if(!v)return false;c.translation=*v;++c.phase;continue;}
 case 40:{const auto v=c.uniform(-20404);if(!v)return false;c.scale=*v;++c.phase;continue;}
 case 41:case 42:{const auto v=c.concat(c.phase==42);if(!v)return false;c.transformed=*v;++c.phase;continue;}
 case 43:{const auto v=c.call(0x80081b8c,c.group==0?0x801189f0:0x80118b9c,[&](auto&q){q.integers={c.group==0?0x802ef7c0u:0x802ef7d4u};});if(!v)return false;c.text=*v;c.phase=100;continue;}
 case 100:if(!c.call(0x80076cdc,c.group==0?0x801189fc:0x80118ba8,[&](auto&q){q.integers={c.text};q.matrix=c.transformed;}))return false;if(c.group==0){c.group=1;c.phase=32;}else c.phase=44;continue;
 case 44:if(!input.flag){cursor.state_.reset();return true;}c.phase=45;continue;
 case 45:{const auto v=c.truncate(0x80118bc0,[&]{c.scroll=scalar(c.word(input.work+0x98));c.translation={};const auto x=c.constant(-20308);c.numeric=x;return c.scroll;});if(!v)return false;
  // Original constants follow fctiwz before signed16 row fields are loaded.
  const auto y=c.constant(-20324),z=c.constant(-20500);c.translation[0]=c.numeric;c.translation[1]=y;c.translation[2]=z;const auto row=input.flag+24*(input.row+uint32_t(*v));const auto first=c.half(row+0x14),second=c.half(row+0x16);c.extraFirst=first<0x8000?int32_t(first):int32_t(first)-65536;c.extraSecond=second<0x8000?int32_t(second):int32_t(second)-65536;c.phase=46;continue;}
 case 46:{const auto v=c.matrix(0x8029e230,[&](auto&q){q.xyz={c.translation[0],c.translation[1],c.translation[2]};});if(!v)return false;c.translation=*v;++c.phase;continue;}
 case 47:{const auto v=c.uniform(-20404);if(!v)return false;c.scale=*v;++c.phase;continue;}
 case 48:case 49:{const auto v=c.concat(c.phase==49);if(!v)return false;c.transformed=*v;++c.phase;continue;}
 case 50:{const auto v=c.call(0x80081b8c,0x80118c24,[](auto&q){q.integers={0x802ef7e8};});if(!v)return false;c.text=*v;++c.phase;continue;}
 case 51:if(!c.call(0x80076cdc,0x80118c30,[&](auto&q){q.integers={c.text};q.matrix=c.transformed;}))return false;c.group=2;c.phase=52;continue;
 case 52:c.decompose(c.group==2?c.extraFirst:c.extraSecond);c.phase=53;continue;
 case 53:{const auto v=c.uniform(-20452);if(!v)return false;c.scale=*v;c.phase=54;continue;}
 case 54:
  if(c.digitIndex>=c.digitCount){if(c.group==3){cursor.state_.reset();return true;}c.phase=59;continue;}
  {const auto v=c.matrix(0x8029e230,[&](auto&q){const auto gp=c.word(0x80415fd0),language=c.word(gp+0x16c);const auto d=c.bias();const auto base=c.constant(-20308),extra=c.constant(c.group==2?(language==0?-20304:-20396):(language==0?-20292:-20288)),y=c.constant(-20312),z=c.constant(-20500);q.xyz={add(extra,add(base,converted(c.positions[c.digitIndex],d))),y,z};});if(!v)return false;c.translation=*v;c.phase=55;continue;}
 case 55:case 56:{const auto v=c.concat(c.phase==56);if(!v)return false;c.transformed=*v;++c.phase;continue;}
 case 57:if(!c.call(0x80076cdc,c.digitFontSite(),[&](auto&q){q.matrix=c.transformed;q.privateText=std::array<uint8_t,2>{uint8_t(c.digits[c.digitIndex]+48),0};}))return false;++c.digitIndex;c.phase=54;continue;
 case 59:{const auto v=c.matrix(0x8029e230,[&](auto&q){const auto language=c.word(c.word(0x80415fd0)+0x16c);q.xyz={c.constant(language==0?-20300:-20296),c.constant(-20324),c.constant(-20500)};});if(!v)return false;c.translation=*v;++c.phase;continue;}
 case 60:{const auto v=c.uniform(-20404);if(!v)return false;c.scale=*v;++c.phase;continue;}
 case 61:case 62:{const auto v=c.concat(c.phase==62);if(!v)return false;c.transformed=*v;++c.phase;continue;}
 case 63:{const auto v=c.call(0x80081b8c,0x80118e48,[](auto&q){q.integers={0x802ef7f4};});if(!v)return false;c.text=*v;++c.phase;continue;}
 case 64:if(!c.call(0x80076cdc,0x80118e54,[&](auto&q){q.integers={c.text};q.matrix=c.transformed;}))return false;c.group=3;c.phase=52;continue;
 default:return false;
 }}
}
}
