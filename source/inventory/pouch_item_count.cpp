#include "pouch_item_count.hpp"
#include <cassert>
namespace ttyd {
int32_t packedPouchItemCount(int32_t item,const EventMemoryServices&m){
 constexpr uint32_t PouchGlobal=0x8041eb00,HaveOffset=0x192,KeyOffset=0xa0,BadgeOffset=0x1fa;
 constexpr int32_t FirstHave=121,EndHave=236,FirstKey=1,EndKey=121,FirstBadge=240,EndBadge=339;
 constexpr uint32_t HaveCount=20,KeyCount=121,BadgeCount=200;
 uint32_t offset=0,count=0;
 if(item>=FirstHave&&item<EndHave){offset=HaveOffset;count=HaveCount;}
 else if(item>=FirstKey&&item<EndKey){offset=KeyOffset;count=KeyCount;}
 else if(item>=FirstBadge&&item<EndBadge){offset=BadgeOffset;count=BadgeCount;}
 else return 0;
 assert(m.word&&m.half);const auto pouch=m.word(m.user,PouchGlobal);int32_t found=0;
 for(uint32_t i=0;i<count;++i){const auto raw=m.half(m.user,pouch+offset+2*i);const auto value=raw<0x8000?int32_t(raw):int32_t(raw)-65536;if(value==item)++found;}
 return found;
}
}
