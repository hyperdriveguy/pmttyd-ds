#include "message_selection.hpp"
namespace ttyd {
bool MessageSelection::open(const char* id,const char* text,const MessageFont& font,uint64_t now) {
    MessageSelection next;
    if(!next.message.open(id,text,font,now) || next.message.windowType!=5 || next.message.choiceCount<1)return false;
    next.current=next.message.initialChoice;next.cancel=next.message.cancelChoice;next.count=next.message.choiceCount;
    next.state=5;next.message.cursor=next.message.records.size()-1;
    next.cursorY=next.message.geometry.y-float(31*next.current+58);
    *this=std::move(next);return true;
}
void MessageSelection::tick(const MessageSelectionInput& input) {
    if(state==5){opacity+=25;if(opacity>255){opacity=255;state=1;}}
    else if(state==7) {
        if(!opacity){state=4;message.state=0;message.windowFlags=0;return;}
        opacity=opacity<=25?0:opacity-25;
    } else if(state==1) {
        const auto previous=current;
        auto byte=[](int v){const auto u=uint8_t(v);return u<128?int8_t(u):int8_t(int(u)-256);};
        if((input.buttons&8)||(input.directions&0x1000))current=byte(current-1);
        else if(((input.buttonRepeat&8)||(input.directionRepeat&0x1000)) && current!=0)current=byte(current-1);
        if((input.buttons&4)||(input.directions&0x2000))current=byte(current+1);
        else if(((input.buttonRepeat&4)||(input.directionRepeat&0x2000)) && current<count-1)current=byte(current+1);
        if(current<0)current=byte(count-1);
        if(current>=count)current=0;
        if(current!=previous)sounds.push_back(0x20005);
        if(input.buttons&0x200) {
            if(cancel!=-1){current=cancel==-2?int8_t(-1):cancel;state=7;sounds.push_back(0x20013);}
        } else if(input.buttons&0x100){state=7;sounds.push_back(0x20012);}
    }
    message.alpha=uint8_t(opacity);message.state=state;
}
}
