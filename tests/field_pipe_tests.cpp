#include "field_pipe.hpp"
#include "game.hpp"
#include "save.hpp"
#include "professor_story.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    FieldPipe p;p.node=123;p.caseType=6;RoadCaseAdmission gate;
    assert(!fieldPipeAdmitted(p,123,0,30,gate));
    assert(fieldPipeAdmitted(p,123,0,31,gate));
    assert(!fieldPipeAdmitted(p,124,123,70,gate));
    gate.window=true;assert(!fieldPipeAdmitted(p,123,123,70,gate));gate.window=false;
    p.caseType=10;assert(fieldPipeAdmitted(p,0,123,0,gate));
    assert(!fieldPipeAdmitted(p,123,0,70,gate));
    assert(loadFieldPipes("romfs/field-pipes.bin") && loadProfessorStoryAssets("romfs"));
    CollisionTransition outer;assert(outer.load("romfs/gor_02"));outer.commit();
    Progress start;start.room=2;start.originalStory=11;start.position={-1.9f,3.291645f};start.elevation=.7f;
    Game game;game.restore(start);
    assert(game.groundContact.sourceNode==1216016);
    assert(!game.tryFieldPipe(30));assert(game.tryFieldPipe(31));
    assert(game.pipeTarget()==5 && game.progress.originalFlag1207);
    {CollisionTransition inner;assert(inner.load("romfs/tik_01"));assert(game.finishPipeWarp());inner.commit();}
    assert(game.progress.room==5 && game.progress.originalStory==11 && game.progress.originalFlag1207 && game.pipeRequest<0);
    assert(game.groundContact.sourceNode && !game.professorStory.triggered);
    const auto checkpoint=game.progress;Progress roundtrip;
    assert(decodeSave(encodeSave(checkpoint),roundtrip) && roundtrip.room==5 && roundtrip.originalFlag1207);
    // Source front-contact owner, rather than a proximity/A shortcut.
    game.wallContact.sourceNode=227880;assert(game.tryFieldPipe(0));assert(game.pipeTarget()==2);
    {CollisionTransition back;assert(back.load("romfs/gor_02"));assert(game.finishPipeWarp());back.commit();}
    assert(game.progress.room==2 && game.professorStory.actors[2].hidden && game.progress.originalStory==11);
    std::cout<<"Original pipe contacts, strict axis gate, callback, reciprocal map staging and save passed\n";
}
