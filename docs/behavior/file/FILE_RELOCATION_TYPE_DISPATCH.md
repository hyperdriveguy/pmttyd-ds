# Complete file relocation dispatch and type-route map

US G8ME01 revision0; ID2,2026-10-12. Complete dispatch/noop paths of
fileGarbageDataAdrSet8018a998/68c and fileGarbageDataAdrClear8018b024/69c.
Containing-body hashes do not imply all nested handler contracts are complete.
Verify `python3 tools/verify_file_relocation_dispatch.py`;read
[complete flat handlers](FILE_FLAT_POINTER_RELOCATION.md) and
[payload movement](FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md).
All local handlers are now linked;format consumers and original runtime remain open.

## Set receives a payload and full type word

Arguments B=payload(r3),T=type(r4). Retain B. Compare UNSIGNED T to10. T>10
returns without payload access or relocation. T0..10 indexes the11-word table
80397618 using T*4,loads target and transfers directly. Negative words are
unsigned-large and rejected. There is no payload-null/signature/length guard
in this dispatch;selected handlers supply their own local operations.

## Clear resolves entry/type/holder before rejecting the type

Argument E=file-entry(r3). Read BYTE E01 FIRST;read WORD E.a0 as H;sign-extend
type byte to word;compare UNSIGNED extended type to10;read WORD H00 as B BEFORE
taking the rejection branch. Invalid type therefore still requires valid E/H
addresses. There is no entry/holder-null guard,state/count/pending admission
or source size check. This distinction matters for type0b on completion failure.

Accepted0..10 indexes table80397644. Types80..ff become negative words and are
rejected after holder access;11..127 are likewise rejected. Set's full-word T
and Clear's signed stored byte are different input domains. Do not unify them
by an unsigned native enum cast performed before entry/holder resolution.

## All11 original table slots

| Type | Set target | Clear target | Local contract delivered |
| --- | --- | --- | --- |
| 0 | 8018b010 | 8018b6ac | Noop/epilogue |
| 1 | 8018a9cc | 8018b064 | Flat25 words14c..1ac |
| 2 | 8018ab04 | 8018b19c | Flat8 words24..40 |
| 3 | 8018ab70 | 8018b208 | Flat1 word48 |
| 4 | 8018ab88 | 8018b220 | [Complete nested/signature handler](FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_SETUP.md) |
| 5 | 8018ac80 | 8018b320 | [Complete nested graph](FILE_TYPE5_NESTED_POINTER_SETUP.md) |
| 6 | 8018ae58 | 8018b4f4 | Flat1 word48 |
| 7 | 8018ae70 | 8018b50c | Flat3 words64..6c |
| 8 | 8018aea0 | 8018b53c | Flat23 wordsc8..120 |
| 9 | 8018afc0 | 8018b65c | Flat4 words58..64 |
| 10 | 8018affc | 8018b698 | Flat1 word48 |

Offsets here are hexadecimal;the flat handlers are supplied in the linked
contract. Types3/6/10 have identical local field rules but distinct table targets;
no shared resource enum/name is inferred from matching instructions. Table4
has a0020af30 signature branch and nested records,which is not a license to
claim a complete TPL loader from the dispatch alone. Type5 nested edges are supplied by the linked setup/clear contracts. This document imposes no guessed model/texture enum.

Type0 performs no payload operations. Clear type0 still performed its earlier
entry/type/holder reads. Unsupported types share that same epilogue after their
specified admission accesses. There are no scalar success/failure results
established by these paths;callers ignore the raw return.

## Caller/backend and acceptance requirements

New archive/DVD acquisition may pass a full T before storing its low8 byte.
Completion and payload movement sign-extend the stored byte for Set. Movement
calls Clear(E) before copy except initial state3/pending,then rereads live
holder/state/type before Set(payload,signedType) or pending destination retarget.
Preserve those separate pointer levels and argument domains.

Check full-word T0..10/11/negative/256,stored bytes00..ff,entry/holder accesses
before Clear rejection,all11 exact table routes,type0 access distinction,flat
versus nested providers and no invented runtime status. Bind the linked type4/type5 graphs explicitly;do not silently return success
or apply a flat scheme to either nested route.

Two containing-body hashes,selected dispatch/flat instruction anchors and both
original tables are checked read-only. No complete source-format schema,
source-domain,successful relocation/rendering or original execution is claimed.
Full EVT-01/JUMP-01/DMG-02 resource requirements remain open.

[Type4 setup](FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_SETUP.md) and
[type4 clear](FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_CLEAR.md) now close that nested
handler,including source US effect TPL witness. Type5 is now [supplied](FILE_TYPE5_NESTED_POINTER_SETUP.md);complete TPL
consumers/platform/source/runtime still require their own contracts.

[Type5 setup](FILE_TYPE5_NESTED_POINTER_SETUP.md) and
[type5 clear](FILE_TYPE5_NESTED_POINTER_CLEAR.md) complete the local Set/Clear
graph across all11 routes. verify_file_type5_relocation.py pins all842 body
instructions plus tables/diagnostics. Source formats/consumers/platform/original
runtime remain separate requirements;this does not complete the resource engine.
