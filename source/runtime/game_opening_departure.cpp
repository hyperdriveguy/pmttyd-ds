#include "game.hpp"
namespace ttyd {
bool Game::retainOpeningDeparture(const OpeningHarborScene& source) {
 return progress.room==1 && progress.originalStory==3 && openingDeparture.begin(source,ticks);
}
}
