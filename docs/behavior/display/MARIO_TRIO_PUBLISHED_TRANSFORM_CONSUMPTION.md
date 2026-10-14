# Mario consumes the published trio transform

The independent native trio world previously passed the published battle actor
record to the Goombella/enemy model paths but omitted it for Mario. Mario therefore
ignored unit scale, rotation, pivot and display offset. His facing also used the
ordinary field reflection instead of the existing battle flag/camera policy.

Mario now supplies that same record to `characterModel`, using `battleMirrorZ`
with his unit Y rotation, battle flags and the actual adapter camera angle. Field
story rendering retains the ordinary path. The existing shared point adapter
applies unit scale, the recovered rotation/pivot order and source display offset;
its declared native-.05 Z separation policy remains a native adaptation. Lower
scene replay consumes the same prepared actor vertices and introduces no second
transform/animation visit. Input, source unit fields and story scripts are unchanged.

A host diagnostic of original compiled idle packs found Mario local bounds
(-10.7977,-1.6582,-1)..(7.34854,21.8018,1), and Goombella
(-5.32982,-.197732,-.3)..(9.60646,14.5269,.5). The ordinary root adapter already
uses.02: documented scalar2 times native unit conversion.01. These facts do not
justify an arbitrary larger scale or establish original battle camera/pose parity.
No model pixels, mesh coordinates, default unit scales or pose data were changed.

Existing recovered rotation/facing and packed publication tests remain applicable;
a separate isolated graphics probe supplies nonidentity packed Mario scale,
rotation and display-offset fields to verify their visible native consumption.
A development probe is not evidence of normal-story battle admission or complete
attack/turn/tutorial flow. Original pose-root/part policy and camera lens/framing
still require their actual source ownership and validation.
