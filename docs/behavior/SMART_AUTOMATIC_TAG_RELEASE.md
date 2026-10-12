# Complete automatic smart-header tag release

US G8ME01 revision0; ID2,2026-10-12. Complete smartAutoFree8002f9cc/a4.
Verify `python3 tools/verify_smart_release.py`;read
[header release/accounting](SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md) and
[file collection](FILE_CACHE_COLLECTION_AND_PRELOAD_AGING.md).
No original caller cadence,tag-domain,scene or heap execution trace is claimed.

## First pass compares unsigned byte tag to low16 argument

Retain full word input K and its LOW16 value T. Reload current manager wp,
read used head W.e008. Traverse retained header H. At each node read BYTE H0e,
then capture NEXT=WORD H14 BEFORE comparison or calls. Compare the byte value
to T as signed words (both are nonnegative after zero extension). Exact equality
calls smartFree(H);then traverse retained NEXT regardless of any link changes.
No state/count/owner/file/payload/null tag guard or callback occurs locally.

T values256..65535 cannot match any byte. High16 bits of K do not affect this
first pass. It does not reread global head after every release;the traversal
starts with one captured head and retains each next link before the release.
The called smartFree retains its own live manager/link/accounting semantics.

## Only full word K==3 admits a second pass

After first traversal,compare FULL signed K to3. Exact equality reloads CURRENT
manager and used head,then traverses again. Each iteration reads BYTE H0e,
captures NEXT=WORD H14 and calls smartFree(H) only for exact byte4. Traverse
retained NEXT;no callback or additional third pass is supplied.

smartFree on tag4 changes it to3 and immediately returns without unlinking.
Consequently the second pass marks tag4 headers as3;it does not normally remove
them during that same invocation. First-pass tag3 headers were already removed
before this second traversal. Do not merge both passes into a predicate that
immediately recycles tag3 OR tag4.

| Input K | First-pass match | Second pass | Local consequence |
| --- | --- | --- | --- |
| 3 | tag3 | tag4 | Recycle initial tag3;mark remaining tag4→3 |
| 4 | tag4 | None | Mark matched tag4→3;preserve used headers |
| 00010003 hex | tag3 | None | Recycle tag3;do not visit tag4 by second-pass policy |
| 00010004 hex | tag4 | None | Mark tag4→3 |
| 00000100 hex | No byte can match256 | None | Traverse first chain without release calls |
| 0 | tag0 | None | Raw tag0 headers can reach normal smartFree |

Alias/service mutation may change the traversed set or tags;these examples
assume ordinary stable provider memory and do not define all valid source tags.
The distinction between full K==3 and low16 matching is mandatory even if native
public APIs currently offer a narrower enum.

## Lifetime and implementation requirements

Keep raw32-bit argument,low16 comparator,unsigned-byte header tags,initial
used-head capture and per-node NEXT before calls. Reload manager for second-pass
head only when full K==3. Preserve used headers marked4→3 until a later recovered
release/collection owner handles them;do not schedule an invented same-call
cleanup or force payload deallocation.

Acceptance covers every matching/nonmatching byte,T256/high16 arguments,
K3 versus00010003,tag3/tag4 ordering,retained next after unlinking,manager swaps
inside smartFree and between passes,new nodes/tag changes and stale payload
preservation. A host address provider must preserve each original header's
links/control bytes separately from the native backing storage lifetime.

Tag producers/scene auto-release callers,smart allocation/compaction/arena
ownership,file-holder consumers/relocation/source/platform and original traces
remain open. File/reference/automatic release are separate local operations;
none alone establishes full-game memory fidelity or total RAM requirements.
