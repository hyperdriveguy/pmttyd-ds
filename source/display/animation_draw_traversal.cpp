#include "animation_draw_traversal.hpp"
namespace ttyd {
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
            if(f.captured.visibility>=pose->shape.visibility.size()){c.pending=0x8003c4bc;return false;}
            f.phase=pose->shape.visibility[f.captured.visibility]?1:9;break;
        case 1:{
            if(f.captured.node>pose->shape.nodes.size() || pose->shape.nodes.size()-f.captured.node<24){c.pending=0x8003c4bc;return false;}
            std::array<float,24> node;for(unsigned i=0;i<24;++i)node[i]=pose->shape.nodes[f.captured.node+i];
            std::array<float,3> parent{1,1,1};
            if(f.captured.joint && f.parent!=-1){const auto p=resource.group(uint32_t(f.parent));if(p.joint){if(p.node>pose->shape.nodes.size() || pose->shape.nodes.size()-p.node<6){c.pending=0x8003c4bc;return false;}for(unsigned i=0;i<3;++i)parent[i]=pose->shape.nodes[p.node+3+i];}}
            if(!resumeAnimationNodeDraw(node,f.captured.joint!=0,parent,c.stack,s.user,s.matrix,f.node)){c.pending=f.node.pending;return false;}
            f.phase=2;break;}
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
        case 4:
            // The special matched-group pointer/effect path owns its own
            // material/Vivian graph. No default unmatched-group assumption.
            if(pose->word(0)&0x20){c.pending=0x8003c4bc;return false;}
            f.callback=pose->word(0x154);f.callbackData=pose->word(0x158);f.callbackLevel=uint32_t(c.stack.size()-1);f.phase=5;break;
        case 5:
            if(f.callback){c.pending=f.callback;if(!s.callback || !s.callback(s.user,f.callback,f.callbackData,uint32_t(f.group),c.stack[f.callbackLevel]))return false;}
            f.render={f.captured.shape,c.context.current,c.context.pass,c.stack.back()};f.phase=6;break;
        case 6:{c.pending=0x8003c884;uint32_t pending=0;if(!s.render || !s.render(s.user,f.render,pending)){if(pending)c.pending=pending;return false;}f.phase=7;break;}
        case 7:{const auto child=f.captured.child,parent=f.group;f.phase=8;if(child!=-1){AnimationDrawFrame next;next.group=child;next.parent=parent;c.frames.push_back(std::move(next));}break;}
        case 8:if(c.stack.size()<2){c.pending=0x8003c4bc;return false;}c.stack.pop_back();f.phase=9;break;
        case 9:{const auto sibling=f.captured.sibling,parent=f.group;if(sibling==-1)c.frames.pop_back();else {f=AnimationDrawFrame{};f.group=sibling;f.parent=parent;}break;}
        default:c.pending=0x8003c4bc;return false;
        }
    }
    c.active=false;c.pending=0;return true;
}
}
