#include "game.hpp"
#include <cassert>
using namespace ttyd;
int main() {
 Game game;game.itemCamera8Flags=0x101;game.fieldCameraFlags=0x40;
 game.acquisitionEffect.beginCoin({0,62,0});assert(game.acquisitionEffect.camera()==4);
 assert(game.applyFieldItemStateRequest(ItemGetRequest::EffectPayload5));
 assert(game.acquisitionEffect.camera()==5 && game.acquisitionEffect.kind()==7);
 assert(game.applyFieldItemStateRequest(ItemGetRequest::Camera8FlagOn));
 assert(game.itemCamera8Flags==0x301 && game.fieldCameraFlags==0x40);
 assert(game.applyFieldItemStateRequest(ItemGetRequest::Camera8FlagOff));assert(game.itemCamera8Flags==0x101);
 game.itemSystemMask=0xa0;assert(game.applyFieldItemStateRequest(ItemGetRequest::SystemLevel2));
 assert(game.itemSystemRequest==2 && game.itemSystemMask==0xa3);
 assert(game.applyFieldItemStateRequest(ItemGetRequest::SystemLevel0));assert(game.itemSystemRequest==0 && game.itemSystemMask==0xa0);
 assert(game.applyFieldItemStateRequest(ItemGetRequest::KeyOff));assert(game.applyFieldItemStateRequest(ItemGetRequest::KeyOff));
 assert(game.fieldMarioBattleState().keyDepth==2 && (game.fieldMarioBattleState().flags&8) && game.fieldKeysOff);
 assert(game.applyFieldItemStateRequest(ItemGetRequest::KeyOn));assert(game.fieldMarioBattleState().keyDepth==1 && game.fieldKeysOff);
 assert(game.applyFieldItemStateRequest(ItemGetRequest::KeyOn));assert(!(game.fieldMarioBattleState().flags&8) && !game.fieldKeysOff);
 assert(game.applyFieldItemStateRequest(ItemGetRequest::SoundFlag80On));assert(game.itemSoundFlags()&0x80);
 assert(game.applyFieldItemStateRequest(ItemGetRequest::SoundFlag80Off));assert(!(game.itemSoundFlags()&0x80));
}
