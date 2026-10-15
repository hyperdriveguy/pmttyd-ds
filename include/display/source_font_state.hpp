#pragma once
#include <memory>
#include "battle_audience_matrix.hpp"
namespace ttyd {
struct SourceFontStateInput {
  uint32_t function=0,argument=0,scaleBits=0;
  // Original caller local, retained at this exact address through a wait.
  // Null selects the genuine guest pointer in argument for FontDrawColor.
  uint32_t* privateColor=nullptr;
};
struct SourceFontStateCall {
  uint32_t function=0,callsite=0;
  std::vector<uint32_t> integers;
  std::optional<uint32_t> color;
  std::optional<BattleActMatrix> matrix;
};
struct SourceFontStateServices {
  BattleActorServices actors;
  void* user=nullptr;std::shared_ptr<const void> lifetime;
  bool (*current)(void*)=nullptr;
  // Whole original GX/JUT child, including actual ordered native effects.
  // Capturing a request or geometry is insufficient acknowledgement.
  bool (*request)(void*,const SourceFontStateCall&)=nullptr;
  bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
};
// Pins helper/input/private-cell address and actual resource/service owners.
// Void completion0 consumes the cursor; missing child keeps partial source/GX
// effects. Reset is not a style pop or implicit color/global restoration.
class SourceFontStateCursor {
public:
  SourceFontStateCursor();~SourceFontStateCursor();
  SourceFontStateCursor(const SourceFontStateCursor&)=delete;
  SourceFontStateCursor& operator=(const SourceFontStateCursor&)=delete;
  bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
  struct State;std::unique_ptr<State> state_;bool invoking_=false;
  friend std::optional<uint32_t> resumeSourceFontState(const SourceFontStateInput&,const SourceFontStateServices&,SourceFontStateCursor&);
};
// Fourteen whole source bodies8007773c..80077bdc. GetDrawColor returns its raw
// word; void helpers return native completion0, not an original ABI register.
// Source memory readers are total over actual admitted mapped storage.
std::optional<uint32_t> resumeSourceFontState(const SourceFontStateInput&,
    const SourceFontStateServices&,SourceFontStateCursor&);
}
