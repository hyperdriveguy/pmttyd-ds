#include "message_selection.hpp"
namespace ttyd {
// Admit only parsed type5 with positive choices, then atomically replace.
// Cursor begins at terminal so choice text is available without ordinary reveal.
// Constructor Y = windowY - (31*choice+58); later drawing owns interpolation.
bool MessageSelection::open(const char* id,const char* text,const MessageFont& font,uint64_t now) {
    MessageSelection next;
    if(!next.message.open(id,text,font,now) || next.message.windowType!=5 || next.message.choiceCount<1)return false;
    next.current=next.message.initialChoice;next.cancel=next.message.cancelChoice;next.count=next.message.choiceCount;
    next.state=5;next.message.cursor=next.message.records.size()-1;
    next.cursorY=next.message.geometry.y-float(31*next.current+58);
    *this=std::move(next);return true;
}
void MessageSelection::tick(const MessageSelectionInput& input) {
    // Opacity step25 is per invocation, not renderer frames. Opening requires
    // >255; closing at0 completes on the next call. Keep the extra zero-alpha
    // lifetime before clearing playback/window poll flags.
    if(state==5){opacity+=25;if(opacity>255){opacity=255;state=1;}}
    else if(state==7) {
        if(!opacity){state=4;message.state=0;message.windowFlags=0;return;}
        opacity=opacity<=25?0:opacity-25;
    } else if(state==1) {
        // Decrement then increment independently: opposite inputs may cancel.
        // New presses wrap, repeats guard endpoints. Byte narrowing precedes
        // signed normalization; sound0x20005 reports only net selection change.
        const auto previous=current;
        auto byte=[](int v){const auto u=uint8_t(v);return u<128?int8_t(u):int8_t(int(u)-256);};
        if((input.buttons&8)||(input.directions&0x1000))current=byte(current-1);
        else if(((input.buttonRepeat&8)||(input.directionRepeat&0x1000)) && current!=0)current=byte(current-1);
        if((input.buttons&4)||(input.directions&0x2000))current=byte(current+1);
        else if(((input.buttonRepeat&4)||(input.directionRepeat&0x2000)) && current<count-1)current=byte(current+1);
        if(current<0)current=byte(count-1);
        if(current>=count)current=0;
        if(current!=previous)sounds.push_back(0x20005);
        // Cancel takes precedence even when disabled(-1), suppressing confirm
        // on that invocation. Cancel-2 returns signed result-1; other values copy
        // unchanged. Cancel/confirm sounds0x20013/0x20012 begin closing only;
        // the selected result stays resident until the caller consumes it.
        if(input.buttons&0x200) {
            if(cancel!=-1){current=cancel==-2?int8_t(-1):cancel;state=7;sounds.push_back(0x20013);}
        } else if(input.buttons&0x100){state=7;sounds.push_back(0x20012);}
    }
    message.alpha=uint8_t(opacity);message.state=state;
}
}
