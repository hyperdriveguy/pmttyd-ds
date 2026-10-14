#include "camera_letterbox.hpp"
#include <cstring>
namespace ttyd {
namespace {
// Decision/update continuation stages, not original game sequence IDs. Captured
// party token survives holds while each source query is reissued only at its stage.
enum Stage {Party,Flags,Bero,Sequence,Key,Ctrl,PartyFlags,Disable,ClearSound,Enable,SetSound,Animate,Rate,Register};
int16_t half(uint32_t value){const uint16_t bits=uint16_t(value);int16_t result;std::memcpy(&result,&bits,2);return result;}
}
// Source camera flags:0x200 bypass automatic decisions,0x100 enables bars,
//0x400 requests immediate final scalar state. Bero mask1 also bypasses decisions.
// Sequence2 admits key/control/party checks; sequence3 preserves disabling state.
// Sound mask0x20 belongs to the sound flag service, not the camera record.
bool updateCameraLetterbox(CameraState& c,DisplayQueue& queue,const CameraLetterboxServices& s,CameraLetterboxCursor& cursor){
    if(!cursor.active)cursor={true,Party,0,0};cursor.pending=0;
    auto query=[&](LetterboxQuery kind,uint32_t identity){cursor.pending=identity;return s.query?s.query(s.user,kind,cursor.party):std::nullopt;};
    for(;;)switch(cursor.stage){
    case Party:{
        cursor.pending=0x800bf1b4;const auto party=s.partySlot0?s.partySlot0(s.user):std::nullopt;
        if(!party)return false;cursor.party=*party;cursor.stage=Flags;break;
    }
    case Flags:cursor.stage=c.publication.flags&0x200?Animate:Bero;break;
    case Bero:{const auto value=query(LetterboxQuery::BeroExec,0x8007ac40);if(!value)return false;cursor.stage=uint32_t(*value)&1?Animate:Sequence;break;}
    case Sequence:{const auto value=query(LetterboxQuery::Sequence,0x8002e210);if(!value)return false;cursor.stage=*value==2?Key:Disable;break;}
    case Key:{const auto value=query(LetterboxQuery::MarioKey,0x8005c47c);if(!value)return false;cursor.stage=*value?Ctrl:PartyFlags;break;}
    case Ctrl:{const auto value=query(LetterboxQuery::MarioCtrl,0x8005c464);if(!value)return false;cursor.stage=*value?Disable:PartyFlags;break;}
    case PartyFlags:{
        if(!cursor.party){cursor.stage=Enable;break;}
        const auto value=query(LetterboxQuery::PartyFlags,0x8000e388);if(!value)return false;
        cursor.stage=uint32_t(*value)&0x100?Disable:Enable;break;
    }
    case Disable:{
        const auto value=query(LetterboxQuery::Sequence,0x8002e210);if(!value)return false;
        if(*value==3)cursor.stage=Animate;
        else {c.publication.flags=uint16_t(c.publication.flags&~0x100u);cursor.stage=ClearSound;}
        break;
    }
    case ClearSound:
        cursor.pending=0x800dc0f8;if(!s.soundFlag || !s.soundFlag(s.user,false,0x20))return false;cursor.stage=Animate;break;
    case Enable:
        if(c.publication.flags&0x100)cursor.stage=Animate;
        else {c.publication.flags|=0x100;cursor.stage=SetSound;}
        break;
    case SetSound:
        cursor.pending=0x800dc110;if(!s.soundFlag || !s.soundFlag(s.user,true,0x20))return false;
        c.letterboxAlpha=255;c.letterboxTop=c.letterboxBottom=0;c.drawCamera=8;cursor.stage=Animate;break;
    // Signed16 bar positions grow3 per invocation, cap top49/bottom44;
    // enabled alpha255. Source drawCamera8 selects UI projection. Narrow before
    // signed comparisons; these scalar widths are not rendered-frame timing.
    case Animate:
        if(c.publication.flags&0x100){
            c.letterboxAlpha=255;
            c.letterboxTop=half(uint32_t(c.letterboxTop)+3);if(c.letterboxTop>49)c.letterboxTop=49;
            c.letterboxBottom=half(uint32_t(c.letterboxBottom)+3);if(c.letterboxBottom>44)c.letterboxBottom=44;
            if(c.publication.flags&0x400){c.publication.flags=uint16_t(c.publication.flags&~0x400u);c.letterboxTop=49;c.letterboxBottom=44;}
            cursor.stage=Register;
        }else cursor.stage=Rate;
        break;
    case Rate:{
        const auto rate=query(LetterboxQuery::Rate,0x8000e388);if(!rate || *rate==0)return false;
        // Disabled fade step is2*trunc(255/rate), NOT trunc(510/rate).
        // Wrap/narrow to signed16 before flooring alpha0. Rate0 holds.
        const uint32_t step=uint32_t(255 / *rate)*2u;
        c.letterboxAlpha=half(uint32_t(c.letterboxAlpha)-step);if(c.letterboxAlpha<0)c.letterboxAlpha=0;
        if(c.publication.flags&0x400){c.publication.flags=uint16_t(c.publication.flags&~0x400u);c.letterboxAlpha=0;}
        cursor.stage=Register;break;
    }
    // Register every completed visit, including invisible alpha0: draw class3,
    // callback token0x8000e564 via Entry0x80010bc4. Deferred GPU work is separate.
    case Register:
        cursor.pending=0x80010bc4;
        if(queue.enter(c.drawCamera,3,0x8000e564,0,0)!=DisplayAdmission::Registered)return false;
        cursor.active=false;cursor.pending=0;return true;
    default:return false;
    }
}
}
