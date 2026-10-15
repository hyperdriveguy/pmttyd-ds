#pragma once
#include "source_font_setup.hpp"
#include "source_window_texture_setup.hpp"
namespace ttyd {
enum class NativeResourcePathKind {Font,Window};
struct NativeResourcePathFormatterServices {
 void*user=nullptr;std::shared_ptr<const void>lifetime;
 bool(*current)(void*)=nullptr;
 std::optional<uint8_t>(*byte)(void*,uint32_t)=nullptr;
 // Pure actual ownership proofs, not inferred from readable bytes. Inputs
 // stay immutable through held admission; output is an isolated typed private
 // destination, alive and disjoint from source inputs through atomic commit.
 bool(*immutableInput)(void*,uint32_t address,uint32_t bytes)=nullptr;
 bool(*privateDestination)(void*,NativeResourcePathKind,const void*,uint32_t nativeCapacity)=nullptr;
 uint32_t stringBudget=0,outputCapacity=0; // explicit native policy, includes NUL
};
// Finite native C-runtime adapter, NOT the whole original printf/varargs/writer.
// Only three original Font/Window plain%s path callsites are admitted.
class NativeSourceResourcePathFormatter {
public:
 NativeSourceResourcePathFormatter();~NativeSourceResourcePathFormatter();
 NativeSourceResourcePathFormatter(const NativeSourceResourcePathFormatter&)=delete;
 NativeSourceResourcePathFormatter&operator=(const NativeSourceResourcePathFormatter&)=delete;
 bool bind(NativeResourcePathFormatterServices);
 std::optional<uint32_t>request(const SourceFontSetupCall&);
 std::optional<uint32_t>request(const SourceWindowTextureSetupCall&);
 bool active()const;bool stale()const;std::optional<uint32_t>pendingAddress()const;
 bool reset();void retire();
private:
 struct State;std::unique_ptr<State>state_;NativeResourcePathFormatterServices services_;
 bool bound_=false,invalid_=false,invoking_=false;
 bool current();
 std::optional<uint32_t>format(NativeResourcePathKind,uint32_t site,const std::array<uint32_t,3>&,uint8_t,const void*,std::map<uint32_t,uint8_t>*);
};
}
