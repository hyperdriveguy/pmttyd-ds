#include "battle_dive.hpp"
#include <cstdlib>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t value){return value<=0x7fffffff?int32_t(value):int32_t(int64_t(value)-0x100000000LL);}
float square(int32_t value){return float(signedWord(uint32_t(value)*uint32_t(value)));}
}
float battleDiveInterpolate(int32_t curve,int32_t t,int32_t count,float start,float target) {
    const float delta=target-start;
    if(curve==0){const float product=float(t)*delta;const float quotient=product/float(count);return start+quotient;}
    if(curve==1){const float product=delta*square(t);const float quotient=product/square(count);return start+quotient;}
    if(curve==4){const int32_t q=signedWord(uint32_t(count)-uint32_t(t));const float end=start+delta;const float product=delta*square(q);const float quotient=product/square(count);return end-quotient;}
    std::abort();
}
}
