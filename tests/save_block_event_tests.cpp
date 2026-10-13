#include "save_block_event.hpp"
#include "map_object_lifecycle.hpp"
#include "message_assets.hpp"
#include <cassert>
#include <algorithm>
#include <vector>
using namespace ttyd;
struct Backend {
    MessageAssets messages;MapObjectKeyControl keys;
    std::vector<std::string> order;std::vector<int32_t> choices,codes;int32_t choice=0,code=0,guard=0;unsigned writes=0,headers=0,busy=0,selections=0,codeReads=0,formats=0,creates=0;
    SaveEventServices services() {
        SaveEventServices service;service.user=this;service.call=[](void* p,SaveEventRequest& r) {
            auto& b=*static_cast<Backend*>(p);
            if(r.kind==SaveEventCall::Continue)assert(r.argumentCount==0);
            if(r.kind==SaveEventCall::Print)assert(r.argumentCount==4 && r.values[2]==0 && r.values[3]==0);
            if(r.first) {
                if(r.kind==SaveEventCall::Print || r.kind==SaveEventCall::Append || r.kind==SaveEventCall::Select) {
                    if(!(r.flags&1))assert(b.messages.find(r.text.c_str()));
                    b.order.push_back(r.text);if(r.kind==SaveEventCall::Select)++b.selections;
                }
                if(r.kind==SaveEventCall::Continue)b.order.push_back("continue");
                if(r.kind==SaveEventCall::Code)++b.codeReads;
                if(r.kind==SaveEventCall::Format)++b.formats;
                if(r.kind==SaveEventCall::Create)++b.creates;
                if(r.kind==SaveEventCall::Key){if(r.flags)b.keys.on();else b.keys.off();}
                if(r.kind==SaveEventCall::Header){++b.headers;b.order.push_back("header");}
                if(r.kind==SaveEventCall::Write){++b.writes;b.order.push_back("write");}
            }
            if(r.kind==SaveEventCall::Select)r.result=b.choices.empty()?(b.selections==1?b.choice:1):b.choices[std::min(size_t(b.selections-1),b.choices.size()-1)];
            if(r.kind==SaveEventCall::Code)r.result=b.codes.empty()?b.code:b.codes[std::min(size_t(b.codeReads-1),b.codes.size()-1)];
            if(r.kind==SaveEventCall::Guard)r.result=b.guard;
            if(r.kind==SaveEventCall::Header || r.kind==SaveEventCall::Write) {
                if(r.first){b.busy=2;return 0;}
                if(b.busy--){return 0;}
            }
            return 2;
        };return service;
    }
};
SaveBlockEvent run(Backend& b,SaveBlockSwitches switches) {
    assert(b.messages.loadGlobal("romfs/messages/global.txt"));
    SaveBlockEvent event;assert(event.begin(0,switches,17));
    auto services=b.services();
    for(unsigned tick=0;tick<2000 && event.active();++tick)event.tick(uint64_t(tick)*1000/60,services);
    assert(!event.failed && event.finished && !b.keys.depth && !(b.keys.marioFlags&8));return event;
}
int main() {
    assert(loadSaveBlockEventAssets("romfs"));
    Backend no;no.choice=1;auto cancelled=run(no,{8,false,false,9});
    assert(cancelled.switches.firstHit && cancelled.switches.postStoryHit && cancelled.switches.story==8 && cancelled.switches.mode==9);
    assert(!no.writes && !no.headers);
    assert((no.order==std::vector<std::string>{"save_first_hit","<scrl_auto><once_stop>","msg_mcard_save_select","msg_mcard_yesno","continue"}));
    Backend yes;auto saved=run(yes,{7,false,false,9});
    assert(saved.switches.firstHit && saved.switches.postStoryHit && saved.switches.mode==0 && saved.switches.story==7);
    assert(yes.headers==1 && yes.writes==1 && yes.order.back()=="msg_mcard_save_finish");
    assert(std::find(yes.order.begin(),yes.order.end(),"continue")==yes.order.end());
    auto header=std::find(yes.order.begin(),yes.order.end(),"header"),write=std::find(yes.order.begin(),yes.order.end(),"write");assert(header<write);
    Backend repeat;repeat.choice=1;auto again=run(repeat,{6,true,false,0});
    assert(!again.switches.postStoryHit && repeat.order.front()=="msg_mcard_save_select" && !repeat.writes);
    for(int32_t error:{-3,-2,-13,-6,-4,-10,-8,-9,-1}) {
        Backend failed;failed.code=error;auto event=run(failed,{8,true,true,0});
        assert(event.switches.story==8 && failed.writes==1 && failed.headers==1 && failed.order.back()=="continue");
        assert(std::find(failed.order.begin(),failed.order.end(),"msg_mcard_save_finish")==failed.order.end());
    }
    Backend format;format.choices={0,0,0,0};format.codes={-13,0,0,0};run(format,{8,true,true,0});
    assert(format.formats==1 && format.creates==1 && format.writes==2 && format.headers==1 && format.order.back()=="msg_mcard_save_finish");
    Backend create;create.choices={0,0};create.codes={-4,0,0};run(create,{8,true,true,0});
    assert(!create.formats && create.creates==1 && create.writes==2 && create.headers==1 && create.order.back()=="msg_mcard_save_finish");
    Backend formatFail;formatFail.choices={0,0,0};formatFail.codes={-13,-1,-3};run(formatFail,{8,true,true,0});
    assert(formatFail.formats==1 && !formatFail.creates && formatFail.writes==1 && formatFail.order.back()=="continue");
    Backend createFail;createFail.choices={0,0};createFail.codes={-4,-1,-3};run(createFail,{8,true,true,0});
    assert(createFail.creates==1 && createFail.writes==1 && createFail.order.back()=="continue");
    Backend writeFail;writeFail.choices={0,0};writeFail.codes={-4,0,-1,-3};run(writeFail,{8,true,true,0});
    assert(writeFail.creates==1 && writeFail.writes==2 && writeFail.order.back()=="continue");
    Backend guard;guard.choices={0,0,0};guard.codes={-13};guard.guard=1;run(guard,{8,true,true,0});
    assert(!guard.formats && !guard.creates && guard.writes==1 && std::find(guard.order.begin(),guard.order.end(),"msg_out_mcard_warnning")!=guard.order.end());
    SaveBlockEvent moved;Backend b;assert(b.messages.loadGlobal("romfs/messages/global.txt"));
    assert(moved.begin(0,{8,true,true,0},17));auto copy=std::move(moved);
    assert(!moved.active());moved.tick(0,b.services());
    for(unsigned tick=0;tick<2000 && copy.active();++tick)copy.tick(uint64_t(tick)*1000/60,b.services());
    assert(copy.finished && !copy.failed);
}
