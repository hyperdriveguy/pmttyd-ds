#pragma once
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
struct SourceMarioMotionCall {uint32_t function=0,callsite=0,argument=0;};
struct SourceMarioMotionServices {
 std::shared_ptr<const void> lifetime;void*user=nullptr;bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 std::optional<uint16_t>(*half)(void*,uint32_t)=nullptr;
 bool(*storeWord)(void*,uint32_t,uint32_t)=nullptr;
 bool(*storeHalf)(void*,uint32_t,uint16_t)=nullptr;
 // Actual table-selected original post child; false retains that child's work.
 // mot_stay_post may delegate to SourceMarioStayPost below, whose reached Off
 // requests use the same service with function800d95f8 and actual handle bits.
 bool(*call)(void*,const SourceMarioMotionCall&)=nullptr;
};
class SourceMarioChangeMotion {
public:
 SourceMarioChangeMotion();~SourceMarioChangeMotion();
 SourceMarioChangeMotion(const SourceMarioChangeMotion&)=delete;
 SourceMarioChangeMotion&operator=(const SourceMarioChangeMotion&)=delete;
 bool resume(uint32_t requestedMotion,const SourceMarioMotionServices&);
 bool complete()const;bool reset();uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
class SourceMarioStayPost {
public:
 SourceMarioStayPost();~SourceMarioStayPost();
 SourceMarioStayPost(const SourceMarioStayPost&)=delete;
 SourceMarioStayPost&operator=(const SourceMarioStayPost&)=delete;
 bool resume(const SourceMarioMotionServices&);
 bool complete()const;bool reset();uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
}
