#pragma once
#include "event_manager.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
enum class SourceMarioDirectionNumberKind{Add,Subtract,Multiply,Divide,FusedMultiplyAdd,SignedWordMinusDouble};
struct SourceMarioDirectionNumber{SourceMarioDirectionNumberKind kind=SourceMarioDirectionNumberKind::Add;uint32_t callsite=0;std::array<float,3>values{};uint32_t word=0;uint64_t doubleBits=0;};
struct SourceMarioDirectionServices {
 std::shared_ptr<const void>lifetime;void*user=nullptr;bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 std::optional<uint64_t>(*doubleWord)(void*,uint32_t)=nullptr;
 bool(*storeWord)(void*,uint32_t,uint32_t)=nullptr;
 std::optional<float>(*eventFloat)(void*,EventEntry&,uint32_t)=nullptr;
 std::optional<int32_t>(*eventValue)(void*,EventEntry&,uint32_t)=nullptr;
 std::optional<float>(*number)(void*,const SourceMarioDirectionNumber&)=nullptr;
 std::optional<int32_t>(*truncate)(void*,float)=nullptr;
};
// Whole revise36080096564/34 or toMovedirSimple800961f8/cc, including source
// loops/loads/rounding boundaries. Numeric operations remain actual providers.
class SourceMarioDirectionHelper {
public:
 SourceMarioDirectionHelper();~SourceMarioDirectionHelper();
 SourceMarioDirectionHelper(const SourceMarioDirectionHelper&)=delete;
 SourceMarioDirectionHelper&operator=(const SourceMarioDirectionHelper&)=delete;
 std::optional<float>resume(uint32_t function,float,const SourceMarioDirectionServices&);
 bool complete()const;bool reset();uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
// Whole evt_mario_set_dir800e6de8/1e0, owning retained helper children and exact
// sysMsec2Frame800059cc/28. Engaged0 is a complete original polling invocation.
class SourceMarioSetDirection {
public:
 SourceMarioSetDirection();~SourceMarioSetDirection();
 SourceMarioSetDirection(const SourceMarioSetDirection&)=delete;
 SourceMarioSetDirection&operator=(const SourceMarioSetDirection&)=delete;
 std::optional<int32_t>resume(EventEntry&,uint32_t actualEventAddress,bool first,const SourceMarioDirectionServices&);
 bool complete()const;bool reset();uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
}
