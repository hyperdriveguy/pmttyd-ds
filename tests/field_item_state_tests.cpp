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
 Game locked(false);locked.progress.hp=3;
 assert(locked.applyFieldItemStateRequest(ItemGetRequest::KeyOff));
 assert(locked.fieldMarioBattleState().keyDepth==1 && locked.fieldMarioBattleState().controlDepth==0);
 locked.tick({});assert(!(locked.fieldIdle.displayFlags&4)); // Key byte39 suppresses low-HP key-dependent idle.
 auto bero=locked.beroExec;assert(bero->read()==0);bero->onOff(0,8);
 Progress checkpoint=locked.progress;locked.restore(checkpoint);
 assert(locked.beroExec==bero && !bero->read(1)); // Neither restore nor unknown checkpoint history is get_info.
}
