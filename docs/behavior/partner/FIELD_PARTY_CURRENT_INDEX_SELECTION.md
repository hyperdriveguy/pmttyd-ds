# Field party current-index selection and entry gate

US G8ME01 revision0, read-only complete local bodies recovered2026-10-13.
This refines the actual lookup required by
[trio field restoration](../battle/UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md),
[constructors](FIELD_PARTY_POSITION_CONSTRUCTORS.md) and
[destruction](FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md). It does not implement
a native registry or prove an original execution trace. Ignored listings/ELFs:
artifacts/recovered-temp-evidence/native-field-party-id,
native-field-party-ptr and native-field-mario-party-entry.

## Pointer and current-index selection

partyGetPtr800bf1b4 takes a signed index. Negative returns null. Nonnegative
reads pointer8041ead8+4*index, without an upper-bound check, allocation,
kind lookup or null repair. Two registered slots are the supported native
constructor domain; do not infer original bounds checking from that domain.

marioGetPartyId80141050 first gets the persistent field Mario pointer, then
reads/sign-extends its byte245. If nonnegative, it calls partyGetPtr. A
nonnull pointer with actor word0 mask8 CLEAR returns that primary index.
Otherwise it reads/sign-extends captured Mario byte246 and repeats the
nonnull/mask8-clear test. Success returns the auxiliary index; failure
returns-1. It does not read the global current-index word8041ead0, select
by party kind, test word8's pending-mode bit or assume Goombella is slot1.
The flag is exactly actor word0 bit8; its producers remain separate owners.

## Constructor wrapper admission and sequencing

marioPartyEntry801412fc captures Mario before its signed kind<8 branch.
For used kind1, it reads the canonical pouch halfword at14*kind and tests
low bit1. Clear returns-1 before destroying the current primary party.
This is a real source failure return, not a successful actor construction
or permission to grant an unlock. Negative kinds are outside this used
caller domain; the source has no local lower-bound check.

After admission, a nonnegative captured primary index requests Mario again,
rereads that owner's index, and if still nonnegative calls partyKill then
stores byteff there. Then partyEntry(kind) is requested. A nonnegative
returned index is stored in the originally captured Mario byte245, passed
to partyGetPtr, and followed by partyHello. A negative result skips those
writes/requests and is returned. Kind>=8 uses byte246 and partyEntry2;
that branch does not use the ordinary pouch unlock gate.

The ending's captured pre-export battle-party kind selects Entry versus Kill.
The later current-index query uses the live field registry above. These are
distinct decisions. A native fixed partner token cannot stand in for both.
Actual registry, owner-slot initialization, animation/appearance/Hello and
joining event history must be connected before declaring native restoration.

## Evidence and next binding

| Complete source body | Address / sizehex | SHA256 |
| --- | --- | --- |
| marioGetPartyId | 80141050 /98 | 2339c1d1b7c127d2570ff8125d1c901db3676fca636de93d725f3f923b02923b |
| partyGetPtr | 800bf1b4 /20 | b338694df4542858c65e2ac8e13a77b66393a928b732dd679665bcfdbfac572a |
| marioPartyEntry | 801412fc /110 | d3c9137c93eab1c7d3acfda0d3abacebd556d2118f7c9150c2bfd1378c58a641 |

Native Game currently retains a fixed partner service token. Next replace it
with two actual registered field actors and persistent owner indices, keeping
constructor failure and missing providers explicit. Preserve the canonical
pouch's authored unlock history and source mask8 exclusion. This local audit
adds no original assets, native behavior, tests or runtime equivalence claim.
