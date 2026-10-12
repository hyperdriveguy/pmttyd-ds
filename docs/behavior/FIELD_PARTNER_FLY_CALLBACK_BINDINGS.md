# Cloud and Vivian callbacks: pending init,writer and post-motion coordinates

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
cloud_move0x80187c54(size0xbc),vivian_move0x801862a0(size0x118).
Kind5 record0x803144b0 has +8 callback80187c54;kind6 record0x803144d8 has
+8 callback801862a0. Both exact40-byte records are pinned. Thus
[driveParty](FIELD_PARTY_DRIVER_DISPATCH.md)'s unguarded kind+8 calls bind these
ordinary callbacks for those kinds,subject to its own run-mode/flag admission.
Names/resource annotations are not original execution observations.

## Shared pending marker

Each first tests P+8 mask8. If set,clear ONLY that marker before requesting
[partyMoveFlyInit](FIELD_PARTNER_FLY_HISTORY_INITIALIZATION.md)(P,1). With or
without pending setup,Vivian requests the
[complete flying writer](FIELD_PARTNER_FLY_HISTORY_WRITER.md)(P) unconditionally.
Cloud has the extra gate below. Initialization does not imply an independent
animation-engine reset or a measured tick frequency.

## Cloud kind5 phase and offset

After pending initialization,Cloud additionally stores float0 to P+17c and
float4.25 to P+180. These writes occur only on the pending branch. Then load
word P+184:NONZERO returns immediately,skipping the writer,phase/offset update
and final flag clear. Pending initialization/phase setup already happened.
This body does not decrement or otherwise change P+184.

For word184==0:

1. Request partyFlyMain(P),which may update selector/root/phase fields through
   its executor and motion services before the following live reads.
2. Compute P+17c=revise360(R(current P+17c-current P+180)).
3. Read that phase again,compute single R(R(pi*phase)/180),pi word0x40490fdb
   at0x80424350. Request sin0x80272410,frsp its returned double to single.
4. Single FMA3*sine+3,constant0x40400000 at0x80424354,store P+80.
5. If current unsigned selector P+3b!=2,clear P+8 mask0x00040000.
   Selector2 preserves that mask.

This writes offset field+80,not rootY+5c. No renderer/pose consumer of that
offset is closed here. Step4.25 is a recovered float,not measured oscillation
period or proof of a fixed simulation/render rate. libm/FPSCR/wrap-domain
behavior remains separate from this complete local arithmetic/request order.

## Vivian kind6 trailing coordinates

Vivian requests partyFlyMain before its post-motion work. Only AFTER that call
it checks P nonnull and unsigned kind P+31==6. That late null check is not an
entry guard:the prefix already dereferenced P and passed it into the writer.
Other kinds return after the writer with no local trailing update.

For kind6,capture old trailing X=P+b8 and root X/Y/Z with corresponding trailing
Y/Z. Compute deltas root-old trailing,then THREE separately rounded products
with0x3dcccccd(approximately0.1) at0x804242cc. Write:

- X:single oldX+productX to P+b8.
- Y:reread CURRENT P+bc,add the previously computed productY,store P+bc.
- Z:reread CURRENT P+c0,add the previously computed productZ,store P+c0.

These are separate multiply/add operations,not FMA. X retains its earlier base;
Y/Z are fresh loads after preceding stores. Preserve that alias/read order.
Request distABf(current rootXZ,current trailingXZ). If not ordered>=30,return.
If ordered>=30,copy RAW current root XYZ into trailing+b8/+bc/+c0,THEN request
angleABf(current rootXZ,current trailingXZ),then movePos(&trailingX,&trailingZ,
30,returned angle). TrailingY remains the copied rootY.

The angle request therefore sees coincident planar coordinates in the ordinary
stable-source domain after the copy. Do not substitute the earlier pre-copy
angle or a conventional30-unit distance clamp without proving equivalence to
angleABf's actual zero-vector behavior. The
[complete system-angle leaf](FIELD_SYSTEM_ANGLE_TABLE.md) now defines ordinary
finite coincident input as positive0,so that domain requests movePos(30,0).
This does not move rootXYZ itself.
Full trailing-coordinate renderer/pose consumption remains open.

## Direct bindings and remaining composition

Seven-text-section direct opcode18 census finds exactly four linked sites:
Vivian init801862cc/writer801862d4,Cloud init80187c80/writer80187ca4.
It checks static direct references,not indirect/REL reachability or frequency.
The callback records add concrete kind+8 bindings. Full constructor/kind/lifetime/
source invariants,pose/offset/trailing consumers and original trajectories remain
open after all local body/hash/anchor checks pass.
