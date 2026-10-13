#include "item_presentation.hpp"
#include "message_assets.hpp"
#include "message_icons.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace ttyd;
namespace {
void half(std::vector<uint8_t>& b,uint16_t x){b.push_back(x&255);b.push_back(x>>8);}
void word(std::vector<uint8_t>& b,uint32_t x){half(b,x&65535);half(b,x>>16);}
std::vector<uint8_t> fixture() {
    std::vector<uint8_t> b{'P','M','I','T','E','M','0','1'};
    word(b,340);word(b,0x803108a8);word(b,40);
    for(unsigned i=0;i<340;++i) {
        word(b,i==339?0:0x80300000+i);word(b,i==339?0:0x80310000+i);
        half(b,i);half(b,i==339?0:2);half(b,i==339?0:1);half(b,0);
        if(i!=339){b.push_back('N');b.push_back(0);b.push_back(0);}
    }
    return b;
}
}
int main(int argc,char**) {
    ItemPresentationCatalog catalog;
    assert(!catalog.find(95));auto b=fixture();assert(catalog.assign(b));assert(catalog.size()==340);
    const auto* resident=catalog.find(95);assert(resident && resident->icon==95 && !std::strcmp(resident->name(),"N"));
    assert(resident->description()!=nullptr && *resident->description()==0); // Empty key is distinct from NULL.
    assert(catalog.find(339) && !catalog.find(339)->name() && !catalog.find(339)->description());
    assert(!catalog.find(340) && !catalog.find(0xffffffff));
    auto bad=b;bad.pop_back();assert(!catalog.assign(bad));assert(catalog.find(95)==resident);
    bad=b;bad.push_back(0);assert(!catalog.assign(bad));
    bad=b;bad[8]=0;assert(!catalog.assign(bad));
    bad=b;bad[34]=1;assert(!catalog.assign(bad)); // Reserved pack word.
    bad=b;bad[36]='X';bad[37]='X';assert(!catalog.assign(bad)); // Unterminated key.
    bad=b;bad[20]=bad[21]=bad[22]=bad[23]=0;assert(!catalog.assign(bad)); // NULL/bytes mismatch.
    assert(!catalog.load(nullptr));assert(!catalog.load("missing-item-pack.bin"));assert(catalog.find(95)==resident);
    if(argc>1) {
        assert(catalog.load("romfs/item-presentations.bin"));
        MessageAssets messages;assert(messages.loadGlobal("romfs/messages/global.txt"));
        for(unsigned i=0;i<339;++i) {
            const auto* item=catalog.find(i);assert(item && item->name() && item->description());
            assert(messages.find(item->name()) && messages.find(item->description()));
        }
        assert(!catalog.find(339)->name());
        const auto* request=catalog.find(95);const auto* pants=catalog.find(107);
        assert(request->icon==307 && !std::strcmp(request->name(),"in_irai_butsu") && !std::strcmp(request->description(),"msg_irai_butsu"));
        assert(pants->icon==308 && !std::strcmp(pants->name(),"in_fighters_pants") && !std::strcmp(pants->description(),"msg_fighters_pants"));
        assert(loadMessageIcons("romfs/message-icons.bin"));
        for(unsigned id:{307u,308u,445u,446u,447u}) {
            const auto* icon=messageIcon(id);assert(icon && icon->frames.size()==1 && icon->loop==0);
        }
        for(unsigned id:{307u,308u})assert(messageIcon(id)->frames[0].width==40 && messageIcon(id)->frames[0].height==40);
        assert(loadItemPresentations("romfs/item-presentations.bin"));assert(itemPresentation(107)->icon==308);
    }
    std::cout<<"Item presentation IDs, key/null boundaries, transactional loading and original resource lookup passed\n";
}
