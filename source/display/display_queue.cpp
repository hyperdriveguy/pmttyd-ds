#include "display_queue.hpp"
#include <cmath>
#include <utility>
namespace ttyd {
DisplayAdmission DisplayQueue::enter(uint32_t camera,int32_t drawClass,uint32_t callback,uint32_t userdata,float priority){
    if(std::isnan(priority))return DisplayAdmission::RejectedNaN;
    if(count_>=records_.size() || drawClass<0 || drawClass>10)return DisplayAdmission::Unavailable;
    constexpr float offsets[]{400000,400000,600000,100000,100000,300000,900000,1100000,1000000,500000,700000};
    auto& record=records_[count_];record.camera=uint8_t(camera);record.drawClass=uint8_t(drawClass);record.callback=callback;record.userdata=userdata;
    record.key=(drawClass==2 || drawClass==5 || drawClass==9 || drawClass==10)?offsets[drawClass]+priority:offsets[drawClass]-priority;
    order_[count_]=uint16_t(count_);++count_;return DisplayAdmission::Registered;
}
void DisplayQueue::sort(){
    if(count_<=1)return;
    // Source fsort permutes references to original sort-pointer slots. Apply the
    // resulting permutation with a distinct native copy, preserving tie order.
    std::array<uint16_t,1024> slots{},before=order_;for(uint32_t i=0;i<count_;++i)slots[i]=uint16_t(i);
    auto compare=[&](int32_t a,int32_t b){const float x=records_[before[slots[size_t(a)]]].key,y=records_[before[slots[size_t(b)]]].key;return x>y?1:x<y?-1:0;};
    auto swap=[&](int32_t a,int32_t b){std::swap(slots[size_t(a)],slots[size_t(b)]);};
    auto partition=[&](int32_t left,int32_t right,int32_t bound,int32_t pivot,int32_t step){
        for(;;){
            while(right>=left){const int c=compare(right,pivot);if(c<0)break;if(c>0)bound=right;right-=step;}
            if(right<left)break;bool exchanged=false;
            while(left<right){const int c=compare(left,pivot);if(c<0){left+=step;continue;}
                swap(left,right);if(c>0)bound=right;else pivot=right;left+=step;right-=step;exchanged=true;break;}
            if(!exchanged)break;
        }
        if(bound<pivot){swap(bound,pivot);bound+=step;}
        return std::pair<int32_t,int32_t>{right,bound};
    };
    auto sortRange=[&](auto&& self,int32_t base,int32_t count)->void {
        while(count>1){
            const int32_t end=base+count-1;
            if(count==2){if(compare(base,end)>0)swap(base,end);return;}
            int32_t left=base,right=end-1,bound=end,pivot=end;
            if(count<40 && count!=4){
                const int32_t mid=base+count/2;const auto a=slots[size_t(base)],m=slots[size_t(mid)],z=slots[size_t(end)];
                if(compare(base,end)>0){
                    if(compare(base,mid)<0){slots[size_t(base)]=z;slots[size_t(mid)]=a;slots[size_t(end)]=m;}
                    else if(compare(mid,end)<0){slots[size_t(base)]=m;slots[size_t(mid)]=z;slots[size_t(end)]=a;}
                    else {slots[size_t(base)]=z;slots[size_t(mid)]=m;slots[size_t(end)]=a;}
                }else if(compare(base,mid)>0)swap(base,mid);else if(compare(mid,end)>0)swap(mid,end);
                if(count==3)return;left=base+1;pivot=mid;
            }else if(count>=40){
                uint32_t bits=0;for(uint32_t n=uint32_t(count);n;n>>=1)++bits;
                const int32_t sample=(bits&1)?int32_t(1u<<(bits/2)):count>>int32_t(bits/2),stride=count/sample;
                const int32_t target=base+(sample/2)*stride;int32_t start=base,tail=base+(sample-1)*stride;
                while(start<tail){const auto cut=partition(start,tail,tail,start+((tail-start)/stride/2)*stride,stride);
                    if(cut.second<=target)start=cut.second;else if(cut.first>=target)tail=cut.first;else break;}
                right=end;pivot=target;
            }
            const auto cut=partition(left,right,bound,pivot,1);right=cut.first;bound=cut.second;
            if(right-base<end-bound){const int32_t nextBase=bound,nextCount=end-bound+1;if(right>base)self(self,base,right-base+1);base=nextBase;count=nextCount;}
            else {const int32_t nextCount=right-base+1;if(end>bound)self(self,bound,end-bound+1);count=nextCount;}
        }
    };
    sortRange(sortRange,0,int32_t(count_));for(uint32_t i=0;i<count_;++i)order_[i]=before[slots[i]];
}
bool DisplayQueue::dispatch(uint32_t camera,const DisplayDispatchServices& services,DisplayDispatchCursor& cursor){
    if(!cursor.active){cursor={0,camera,0,true};}else if(cursor.camera!=camera)return false;
    while(cursor.invoking || cursor.index<count_){
        if(cursor.invoking){
            if(!services.callback || !services.callback(services.user,camera,cursor.callback,cursor.userdata))return false;
            cursor.invoking=false;++cursor.index;cursor.stage=0;continue;
        }
        const auto reference=order_[cursor.index];const auto& record=records_[reference];
        if(record.camera!=camera){++cursor.index;cursor.stage=0;continue;}
        std::array<DisplayStateRequest,7> states{};size_t count=3;
        states[0]={DisplayGXState::ColorUpdate,{1}};states[1]={DisplayGXState::AlphaUpdate,{camera==4 || camera==5 || camera==7?0:1}};
        states[2]={DisplayGXState::ZScaleOffset,{},{{1,0}}};const auto c=record.drawClass;
        if(c<=5 || c==9 || c==10){
            const bool blend=c==2 || c==5 || c==9 || c==10,alpha=c==1 || c==4,depth=c!=3 && c!=4;
            states[3]={DisplayGXState::BlendMode,blend?std::array<int32_t,5>{1,4,5,0,0}:std::array<int32_t,5>{0,1,0,c==0?7:0,0}};
            states[4]={DisplayGXState::ZCompareLocation,{alpha?0:1}};
            states[5]={DisplayGXState::AlphaCompare,alpha?std::array<int32_t,5>{6,128,1,0,0}:std::array<int32_t,5>{7,0,0,7,0}};
            states[6]={DisplayGXState::ZMode,{depth?1:0,3,blend?0:(depth?1:0)}};count=7;
        }
        while(cursor.stage<count){if(!services.state || !services.state(services.user,states[cursor.stage]))return false;++cursor.stage;}
        current_=reference;cursor.callback=record.callback;cursor.userdata=record.userdata;cursor.invoking=true;
    }
    cursor.active=false;return true;
}
}
