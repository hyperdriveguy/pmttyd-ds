# Bero previous-party admission and retained first-call boundary

US G8ME01 disc0 revision0, static read-only recovery for
[EVT-01](../../requests/EVT-01.md). Complements
[Bero key/next frontier](../field/BERO_KEY_LEAVES_FRESH_ACTOR_OWNERS_AND_NEXT_ROAD_FRONTIER.md),
[constructor graph](FIELD_PARTY_POSITION_CONSTRUCTORS.md),
[entry animation admission](FIELD_PARTY_ENTRY_ANIMATION_ADMISSION.md)
and [current-index distinction](FIELD_PARTY_CURRENT_INDEX_SELECTION.md).
Addresses/offsets/masks hexadecimal; slot/selectors/counts decimal.
This supplies a safe finite native binding plan, not invented actor allocation
or a claim that complete Entry/Entry2 services already exist.

## Complete callback's used raw0/1 branches

evt_mario_set_prev_party800e3058/c8 captures argument base,gets Mario once
at800e3078,then loads raw operand0 at3080. It does not use evtGetValue.
Raw selector is an actual byte offset added to the captured Mario pointer,
not the zero/current versus nonzero/slot1 rule of other party callbacks.
The original uses bytes245+selector and247+selector. Native supported input
may be exactly0/1;reject other raw words before memory access rather than
fold every nonzero into1.

FirstCall!=0:

1. Read signed8 current index from captured Mario245+selector at3090.
   If nonnegative,return2 at309c without party lookup,kind read or allocation.
2. Ifnegative,read signed8 savedkind247+selector at30a4. Exactlyzero returns2
   at30b0. A negative signed kind is not zero and reaches the constructor in
   original unchecked source;native may refuse unsupported kind values.
3. Rawselector0 requests partyEntry800bea68(savedkind) at30c0;nonzero requests
   partyEntry2800bddd0(savedkind) at30c8. If returned signed index<0,return2
   at30d4 with no index-byte store and no following pointer lookup.
4. Otherwise write returned lowbyte to captured Mario245+selector at30dc.
   Continue to the common tail.

FirstCall==0 skips steps1..4 entirely. Common tail freshly reads/sign-extends
captured Mario245+selector at30e0..30e8,then partyGetPtr800bf1b4 at30ec.
Null and nonnull both return2;no additional actor writes occur in this body.
It never writes event locals or current-index global8041ead0. Constructor
side effects are actual world work,not canceled merely because this wrapper
returns the same scalar2 in every completed source branch.

## Existing native bytes support real completed branches

FieldMarioBattleState in include/field/field_battle.hpp already owns
partyIndices[0/1] for source245/246,with explicit255 initial values, and
savedPartyKinds[0/1] for source247/248,with constructor-memset zeros.
These defaults have actual source MarioInit provenance. They are distinct
from current-party global8041ead0 and party kind/pouch ownership.
FieldPartyRegistry owns the actual two pointer slots;pointer(index) returns
null for negative and asserts for index>=2 as a native supported-domain check.

Thus for an actual fresh native owner,firstcallraw0 and1 each read signed-1,
then savedkind0,and complete2 without needing party resources or a synthetic
actor. Use these real bytes rather than assuming no partner from screen art.
If firstcallindex>=0,the source likewise completes without pointer lookup:
no native registry probe is required even for a known nonnegative byte outside
native0/1,provided this exact early branch is retained. By contrast the common
tail does dereference the registry and must admit a negative index or actual
native0/1;do not call pointer(index>=2) and trigger its assertion.

Source callback captures Mario once. A constructor/provider may change the
global Mario pointer, but its later index store and fresh slot-byte reread still
belong to the original captured Mario. Constructor result is full signed word
for the negative check,lowbyte only for index storage;do not normalize it to a
bool or conflate it with a party kind. A subsequent partyGetPtr uses the fresh
signed byte,not necessarily the original full return word.

## Missing constructor cannot consume the original first phase

USER_FUNC caches its function and marks initialized before polling. Returning0
because an Entry provider is absent consumes VM firstCall. Simply invoking
this original callback with first=false on the next visit skips all creation
and can falsely complete through a null pointer. Native first-call preservation
must be explicit if this service becomes asynchronously admissible.

A suitable owner-scoped policy is retain per-event-generation callback state:
first invocation captures its source argument/Mario/selector/current/savedkind
reads;when construction is genuinely required,admit its complete resources
before making constructor effects and retain this pending first phase across
polls. Resume that pending phase and perform the actual constructor/index/tail
once when ready. Clear it at completion and event retirement. Do not infer the
phase merely from the next VM firstCall=false,or use an unrelated event.waitCount
slot as invented original state. A simpler initial implementation can admit
only the proven early-return branches and retain an explicit pending-first
marker for unsupported nonzero-kind domains.

Preflighting constructor dependencies before the callback's first source read
is another explicit native admission policy,but it must preserve the same
branch selection/captured-owner semantics if externally mutable providers are
allowed. Never satisfy the missing service by fabricating a negative allocation
result;that would suppress real required resource/actor effects. Source Entry
itself is synchronous to this callback;native pending is an adaptation.

## Reusable actual code and remaining Entry closure

FieldPartyRegistry already supplies useful real mechanics:allocateFirst,
initializeBeforePose,enterPose,material/light owners,requestFreshGoombellaPose,
requestGoombellaPose andkuriboInit. They are not a complete Entry service.
The currently bound ordinary kind1 construction composition resides inside
source/battle/trio_attack.cpp's field-return state0700001f. It uses genuine
pouch HP,original c_pkr animation resource,current Mario pose material/light
owners,registry allocation,kuribo_init and original appearance provider. Factor
those owners/ordered operations for reuse rather than returning success from
allocateFirst alone or copying a rendered follower harness into a registry slot.

partyEntry800bea68 wraps actual partyEntryMain800be144 then vector70=(1,1,1),
modebyte38=10,angles110/10c from owner1ac,partyGetAppearPos2800c6718 current
root placement,previous-position copy,pose request and modebyte34=1. Its wrapper
has no local safe repair after Main fails;constructor resource/free-slot domain
must be admitted before entering unsafe dereferences. True appearance requires
live map/camera/ray/fallback services,not a fixed offset from Mario.

Entry2 is not interchangeable with Main's first-free policy:secondary source
constructors target pointer slot1,own different initial flags/record writes and
saved-kind behavior. Registry.initializeBeforePose models the used Main prefix
and writes savedPartyKinds[0] conditionally;it cannot substitute for secondary
initialization. General kinds,secondary resources/init callbacks and complete
Entry2 placement remain explicit missing providers. Reusing marioPartyEntry
as a shortcut is also wrong:that higher wrapper adds pouch-unlock,current-party
destruction and Hello behavior absent from this Bero callback.

Native Game restore currently rebuilds registry/Mario transient state through
its existing adapter. Source marioReInit retains245/246 and247/248;therefore
future full area integration must carry actual source bytes/registry lifetime
consistently across the chosen restore path. Constructor-proven fresh bytes
are sufficient for the current fresh-owner branch but do not prove arbitrary
checkpoint/room history should erase saved nonzero kinds.

## Evidence and scope

Complete callback listing artifacts/outer-bank-evt_mario_set_prev_party.txt;
original info_run inventory artifacts/house05-preceding-dol-events.json.
Existing constructor contracts above pin complete local source bodies and
current native FieldPartyRegistry methods were inspected read-only. No source,
shared docs,index,git or build changes. Ordinary parent can advance through
actual savedkind0 branches and remain pending at genuine length/position
providers;nonzero party construction must remain honestly unbound until its
world/resource closure is ready.
