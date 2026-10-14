#include "game.hpp"
namespace ttyd {
// New-file harbor room1/story3 handoff retains the original departing ship
// scene at current field tick. Other checkpoints must not invent a departure.
bool Game::retainOpeningDeparture(const OpeningHarborScene& source) {
 return progress.room==1 && progress.originalStory==3 && openingDeparture.begin(source,ticks);
}
}
