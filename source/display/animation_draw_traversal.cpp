#include "animation_draw_traversal.hpp"
#include "animation_scratch_memory.hpp"
namespace ttyd {
// Iterative source group recursion keeps both matrix and call-frame stacks
// across holds. Start at last group; an invisible group skips its node/child
// work but still follows its sibling. Parent token is retained explicitly.
// Typed/copy-based traversal,distinct from actual-VM resumeAnimationGroup.
// Context is captured once;only handle is compared on resume. Caller keeps
// resource,pass,providers,pool generations and copied matrix lifetime stable.
bool resumeAnimationDrawTraversal(BattlePosePool& pool,const BattlePoseInnerContext& input,const AnimationDrawTraversalServices& s,AnimationDrawTraversalCursor& c){
    if(!c.active){
        if(!input.resource || !input.resource->draw)return false;
        c.active=true;c.pending=0;c.context=input;c.stack.clear();c.frames.clear();
        c.stack.reserve(32);c.frames.reserve(32);c.stack.push_back(input.model);
        AnimationDrawFrame root;root.group=int32_t(input.resource->draw->groups())-1;c.frames.push_back(std::move(root));
    }
    if(input.handle!=c.context.handle)return false;
    const auto& resource=*c.context.resource->draw;
    while(!c.frames.empty()){
        auto& f=c.frames.back();auto* pose=pool.pose(c.context.handle);if(!pose){c.pending=0x8003c4bc;return false;}
        switch(f.phase){
        case 0:
            if(f.group==-1){c.frames.pop_back();break;}
            if(f.group<0 || uint32_t(f.group)>=resource.groups()){c.pending=0x8003c4bc;return false;}
            f.captured=resource.group(uint32_t(f.group));
            {const auto visible=animationOutputByte(*pose,false,f.captured.visibility);if(!visible){c.pending=0x8003c4bc;return false;}
            f.phase=*visible?1:9;}break;
        case 1:{
            std::array<float,24> node{};std::array<float,3> parent{1,1,1};
            // Typed adapter snapshots 24 floats before node SDK planning. The
            // raw source node controller instead retains interleaved 12/21 reads;
            // do not infer original VM read order from this convenience path.
            if(!f.node.active){
            for(unsigned i=0;i<24;++i){const auto value=animationOutputNode(*pose,f.captured.node+i);if(!value){c.pending=0x8003c4bc;return false;}node[i]=*value;}
            if(f.captured.joint && f.parent!=-1){const auto p=resource.group(uint32_t(f.parent));if(p.joint){for(unsigned i=0;i<3;++i){const auto value=animationOutputNode(*pose,p.node+3+i);if(!value){c.pending=0x8003c4bc;return false;}parent[i]=*value;}}}
            }
            if(!resumeAnimationNodeDraw(node,f.captured.joint!=0,parent,c.stack,s.user,s.matrix,f.node)){c.pending=f.node.pending;return false;}
            f.phase=2;break;}
        // Pose flag 40/80(hex) publish different destinations(+15c/+164).
        // Each publication/callback acknowledges independently before shape draw.
        case 2:
            if(pose->word(0)&0x40){f.publication={0x40,uint32_t(f.group),pose->word(0x15c),c.stack.back()};f.phase=20;}
            else f.phase=3;
            break;
        case 20:c.pending=0x8003c4bc;if(!s.publish || !s.publish(s.user,f.publication))return false;f.phase=3;break;
        case 3:
            if(pose->word(0)&0x80){f.publication={0x80,uint32_t(f.group),pose->word(0x164),c.stack.back()};f.phase=30;}
            else f.phase=4;
            break;
        case 30:c.pending=0x8029dd30;if(!s.publish || !s.publish(s.user,f.publication))return false;f.phase=4;break;
        // Pose flag 20(hex) is a separate matched-group/material graph. Missing
        // provider cannot be treated as an ordinary unmatched group.
        case 4:
            // The special matched-group pointer/effect path owns its own
            // material/Vivian graph. No default unmatched-group assumption.
            // Any flag20 currently holds this typed route,even if a raw source
            // matched-group test could be false. Only actual-VM group owns the
            // complete material/Vivian branch;absence cannot imply unmatched.
            if(pose->word(0)&0x20){c.pending=0x8003c4bc;return false;}
            f.callback=pose->word(0x154);f.callbackData=pose->word(0x158);f.callbackLevel=uint32_t(c.stack.size()-1);f.phase=5;break;
        case 5:
            if(f.callback){c.pending=f.callback;if(!s.callback || !s.callback(s.user,f.callback,f.callbackData,uint32_t(f.group),c.stack[f.callbackLevel]))return false;}
            f.render={f.captured.shape,c.context.current,c.context.pass,c.stack.back()};f.phase=6;break;
        case 6:{c.pending=0x8003c884;uint32_t pending=0;if(!s.render || !s.render(s.user,f.render,pending)){if(pending)c.pending=pending;return false;}f.phase=7;break;}
        // Child traversal uses the current composed matrix; pop only after the
        // child returns. Sibling continuation then runs with the restored stack.
        case 7:{const auto child=f.captured.child,parent=f.group;f.phase=8;if(child!=-1){AnimationDrawFrame next;next.group=child;next.parent=parent;c.frames.push_back(std::move(next));}break;}
        case 8:if(c.stack.size()<2){c.pending=0x8003c4bc;return false;}c.stack.pop_back();f.phase=9;break;
        // Sibling receives CURRENT group as parent,even for invisible nodes.
        // Reuse frame but restore composed stack before following the sibling.
        case 9:{const auto sibling=f.captured.sibling,parent=f.group;if(sibling==-1)c.frames.pop_back();else {f=AnimationDrawFrame{};f.group=sibling;f.parent=parent;}break;}
        default:c.pending=0x8003c4bc;return false;
        }
    }
    c.active=false;c.pending=0;return true;
}
}
