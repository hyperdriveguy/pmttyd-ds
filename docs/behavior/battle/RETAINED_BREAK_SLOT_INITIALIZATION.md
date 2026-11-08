# Retained original break-slot initialization

Whole US revision0 `BattleBreakSlot_Init` at `8020507c`, size `194` hexadecimal,
ends immediately before `BattleBreakSlotReelGetPtr` at `80205210`. Earlier
inspection of only `174` hexadecimal bytes omitted the third table copy and
return. The source verifier pins all404 bytes, both complete pointer helpers,
and the21 category-table words directly from the read-only ISO.

`resumePackedBattleBreakSlotInit` owns one invocation. Each child request records
its original function and callsite. `nullopt` retains that same request object;
engaged zero means a completed child returning zero, including void children.
Successful clears, allocations, stores and copies are not repeated on resumption.
Reentry and reset during a callback are rejected. Binding identity and the strong
allocation/process owner remain fixed; a false current callback permanently
retires this invocation, even if the callback later becomes true again.

The exact original order is:

1. Call `8020524c` at `80205094` and capture the controller pointer.
2. Read8/8/5 source words at `802fe040/60/80`. Original paired loads precede
   their corresponding private stores. The final third-table word is separate.
3. Call memset `800050b4` at `8020511c`, with captured controller, zero and
   `134` hexadecimal bytes. Store controller+4=0 after completion.
4. Call reel getter `80205210(0)` at `80205130`; initialize three contiguous
   reel indices/states with stride `5c` hexadecimal, from this returned pointer.
5. Separately call the reel getter for0,1,2 at `80205158/190/1c8`. For each,
   store count8/8/5, reload it for allocation size, call `800f8934` at
   `80205170/1a8/1e0`, and store the raw return at the captured reel+14.
6. Reload count and destination, then call memcpy `8000519c` at
   `80205188/1c0/1f8`. Return only after the third copy completes.

These source getters are separate invocations. `8020524c` reads actual global
`8041eb90` and adds `1616c` hexadecimal. `80205210` calls that helper and adds
`18 + index*5c`. A changed global can therefore select a different later reel;
an allocation cannot change the already captured reel used by its following
pointer store and copy. Neither parent nor native binding may replace the
lookups with a fixed BattleWork address or a cached initial controller.

Private stack arrays retain their original offsets: row0 at3c, row1 at1c, row2
at8. Copy requests carry a typed private source plus byte length, not a fabricated
guest stack pointer. Unknown words, including saved-register/gap words, remain
unknown. A native copy provider must admit every requested private source byte
and destination before copying. Unsupported private overreads genuinely hold;
unknown bytes must not become zero. A null allocator return is stored exactly
as original; unavailable memcpy through that pointer holds without allocating
again. No rollback or additional success check is attributed to the source.

All total memory callbacks require prior admission of their reachable spans.
The actual Harbor binding must pin the real BattleWork/map allocations and VM,
validate their current identities before each resume, use the source pool's real
allocator, and retain the full cursor across cold-start retries. Actual zero
memset and typed private-copy implementations can return engaged0 after effects.
The cold-start service may return engaged0 for `8020507c` only when this whole
parent returns true. Existing immediate `initializePackedBattleBreakSlot` remains
an explicitly nonretained bridge and is unsafe to retry after a held allocation.

Focused tests cover all12 child points held twice without replay, stable request
address, paired private captures surviving table mutation, real global changes
between allocations/getters, count reload after allocation, null return followed
by held copy, binding replacement, retirement and recursive invocation rejection.
The tests use an explicit host memory fixture and provider, not a native battle
playthrough. This does not implement active slot-game states, source Main/Draw,
normal Game startup, audience interaction or the complete Crump encounter.
