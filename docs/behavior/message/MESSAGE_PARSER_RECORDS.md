# Message parser records, layout and control state

US G8ME01 revision 0, inspected 2026-10-12. B evidence means original DOL
instructions and constants, not executed gameplay. `msgAnalize` at 0x80081cbc,
size 0x16a4, is only partially reconstructed in the pinned Japanese source;
the following contracts come from the US binary. See [resource lookup](MESSAGE_RESOURCES_AND_LOOKUP.md)
and [announcements](../battle/BATTLE_ANNOUNCEMENT_MESSAGES.md). Original dialogue remains
reused data; no story text is invented or rewritten here.

## Input, persistent state and record ownership (B)

Inputs are a smart allocation handle and a text pointer. Dereference the
handle's first word as message data D; no local null guard. Window W is the
pointer at D+0xf24c. Initial output pointer is D+0x3c+24*D.word[0x38]. The
routine appends to this existing record count rather than clearing it. It has
no local output-capacity bound. Negative W.word[8] is changed to zero first.

| Data offset | Observed parser role |
| --- | --- |
| 4 | message flags; bit 0x10 changes line spacing, 0x20 selection mode, 0x100 keyxon |
| 38 | emitted record count |
| 3c onward | 24-byte records |
| f230 | word T, record schedule coordinate |
| f234/f236 | signed halfwords X/Y, current text coordinates |
| f238/f23a | signed halfwords K/S, key ordinal and scroll displacement |
| f23c | word V, schedule increment/default speed |
| f240 | selection line byte |
| f241/f242 | selection configuration bytes |
| f244/f246 | signed halfwords for dimensions/selection parameters |
| f24c | window pointer |

T, X, Y, K, S and V start from these fields. Local style flags F start at zero;
scale floats A/B/C start at (1,1,1), maximum line scale L starts at 0, scroll
baseline H starts at 0, and maximum width M starts at 70. F and scale changes
are local to this invocation; they are not restored from previous glyph records.
Terminology such as schedule coordinate does not establish a unit of frames
or simulation ticks: the consumer must still be recovered.

| Record offset | Established storage |
| --- | --- |
| 0 | glyph flags or opcode-specific payload (color, audio ID, pose ID, pointer) |
| 4 | unsigned halfword glyph/opcode |
| 6/8 | halfwords for position or opcode-specific parameters |
| a | extra halfword used by icon records |
| c | schedule word or opcode-specific argument |
| 10/14 | float style/scale or opcode-specific values |

Writing a record does not clear untouched fields. Do not treat all records as a
single fully initialized glyph structure. Except where specified below, emitting
a record advances the output pointer by 24 and increments D.word[0x38].

If window type is 3 or 8 on entry, first append opcode 0xfff7 with payload
0xdcc8dcff or 0x00d914ff respectively and schedule T. This does not advance T.

## Scanner and command boundary (B)

The loop classifies Japanese lead bytes only when language configuration is
zero, using the same ranges as [_ismbblead](MESSAGE_RESOURCES_AND_LOOKUP.md).
Such a lead byte consumes two bytes and forms a big-endian code unit. Otherwise
dispatch signed input byte: NUL terminates, LF emits a line record, '<' enters
command parsing, and all other bytes follow single-byte glyph conversion.
US language 1 does not take the two-byte lead path.

The command splitter is inlined here: start after '<', scan at most 128 bytes,
copy name and arguments into separate 128-byte stack areas, split on the first
ASCII space, end on '>'. It has the same NUL/delimiter limitations as
msgGetCommand. Successful end pointer replaces the text pointer. Failure assigns
null and is not followed by a local safe-failure branch; malformed input is not
a supported presentation contract.

Names are compared by exact strcmp in binary order. An unknown command emits
nothing. Most handled/unknown commands share a continuation that consumes one
immediately following LF without creating a line record. Thus control-only
lines cannot be preserved by blindly rendering every source newline. The special
annotation path has separate continuations and remains to be documented.

## Glyph layout and termination (B)

For a single-byte glyph, pass the zero-extended byte to hankakuSearch (the signed
classification uses a separate register). In
Japanese language 0 only, ASCII digits instead use kanjiSearch on code unit
`0x824f+(byte-'0')`. For a two-byte glyph use kanjiSearch(codeUnit). The returned
glyph is stored as a halfword; kanjiGetWidth receives its low 16 bits.
Font lookup tables and missing-glyph behavior remain a separate dependency.

Append a glyph record with F at +0, glyph at +4, X at +6,
`Y-trunc(10*B)` narrowed to a halfword at +8, T at +c and A at +10.
If F bit 1 is set, write the dynamic float at +14; otherwise leave +14 alone.
For single-byte input, set record flag 8 unless the original byte is ASCII space.
For two-byte input, code 0x8145 sets flag 0x10, codes 0x8140/0x8151 add neither
flag, and other codes set flag 8. Preserve F's existing bits in each case.

Update L to B if B is greater than L. Advance signed-halfword X by
`trunc(A*(width+2))` for single-byte or `trunc(A*(width+4))` for two-byte input.
Increment T by V with word arithmetic. Advance input by one or two bytes.
The vertical multiplication uses B while horizontal advance uses A, including
the scaleX path below; do not infer their meaning from the command name alone.

LF appends opcode 0xfffb with schedule T. If L equals zero, use L=1.
With D.flags bit 0x10 set, use delta=`0.7*(32*L)`; otherwise delta=`32*L`.
Set Y=trunc(float(Y)-delta) and S=trunc(float(S)-delta), using single-precision
arithmetic, truncation toward zero and halfword storage. Truncation occurs after
subtraction; rounding delta first can differ for negative coordinates.
Advance T by V; reset L=0.
In selection mode (D.flags bit 0x20), increment the line byte, clear F's
0xff00 region and OR `0x100 << signed(oldLineByte)` into F. If dimension f244
is still zero, update M from the old X using the original unsigned word compare.
Finally set X=0. Shift edge behavior and dimension overflow are original word/
halfword behavior, not arbitrary-precision arithmetic.

NUL writes terminal opcode 0xffff and schedule T into the current record but
does not increment the count. If D.s16[f244] is zero, unsigned-compare M with
sign-extended X, then write low halfword of M+80 to f244. Persist T/X/Y/K/S back
to f230..f23a and return. V is not generally persisted here; some controls write
its persistent default explicitly. Untouched record fields remain untouched.

## Wait, position, scale and effects (B)

Numeric parsing uses original sscanf formats; conversion counts are not checked.
The following describes valid supplied arguments. Failed/missing conversions
can leave previous or uninitialized stack values and must not be replaced with
an assumed universal zero default.

| Exact name(s) | Local effect |
| --- | --- |
| k, key | Append 0xfffe, +6=K, +c=T; T+=100; K increments as halfword |
| keyxon | OR D.flags with 0x100; no record |
| p, scrl_auto | If S=0, no record. Else append 0xfffd, +8=S-3, +c=T; H=halfword(Y-6), Y=halfword(Y-3), T+=50, S=0 |
| o, once_stop | Append 0xfffc, +c=T; T+=50 |
| pos | Parse `%d%d`; X=halfword(first). If second is not 999, Y=halfword(H-second), S=halfword(-second) |
| scale | Parse `%f` into A; B=A; clear F bit 0x10000 |
| scaleX | Parse `%f` into A only; set F bit 0x10000 |
| /scale | Reset A/B/C=(1,1,1); clear F bit 0x10000 |
| wait | Parse `%d`; add to T; no record |
| speed | Parse `%d` into local V |
| /speed | Restore V from D.word[f23c] |
| dynamic | Parse `%f` into dynamic local; set F bit 1 |
| /dynamic | Clear F bit 1 |
| shake, /shake | Set/clear F bit 2 |
| wave, /wave | Set/clear F bit 4 |
| col | Parse `%x` directly into record +0; append 0xfff7, +c=T; no T advance |
| /col | Append 0xfff7 with payload 0x000000ff and +c=T; no T advance |
| noise | Append 0xfff6, payload 1, +c=T |
| /noise | Append 0xfff6, payload 0; +c is not assigned in this branch |
| dkey, /dkey | Append 0xfff5, payload 1/0, +c=T |
| se | Parse `%d` directly into record +0; append 0xfffa; no +c assignment |
| vol | Parse `%d` directly into record +0; append 0xfff9; no +c assignment |
| nowinse | OR window halfword flags[2] with 8; no record |

These are parser-side records and schedule changes. Actual key input admission,
scroll animation, sound playback, noise and dynamic rendering are consumer work;
the numeric increments above are not measured delays.

## Window type, positioning and selection (B)

| Exact name | Type written to W.word[8] | Additional effect |
| --- | --- | --- |
| plate | 2 | V=0 and D.word[f23c]=0 |
| system | 3 | Append color opcode 0xfff7, payload 0xdcc8dcff, +c=T |
| tec | 8 | Append color opcode 0xfff7, payload 0x00d914ff, +c=T |
| majo | 9 | None locally |
| diary | 7 | None locally |
| housou | 10 | None locally |
| boss | 6 | None locally |
| kanban | 4 | V=0 and D.word[f23c]=0 |
| small | 1 | OR D.flags with 0x10 |
| clear | 11 | None locally |

Window-type changes do not otherwise reset local style flags or coordinates.
`wpos` parses four `%hd` values, ORs W.halfword[2] with 4, and converts the signed
halfwords to floats at W+0xc/0x10/0x14/0x18 respectively. Window geometry consumers
still determine how these fields position and size the rendered window.

`select` sets window type 5 and D.flags bit 0x20. It parses configuration,
stores the low bytes of the first two signed-halfword outputs to f241/f242,
sets f240=1, sets F bit 0x100 and makes local V=0. It does not write D.f23c
on this path. Two more established outputs target D.f244/f246 directly.
Important ABI uncertainty: the literal format is five `%hd` conversions while
the call site explicitly prepares only four destination pointers (r5..r8).
Do not silently assume a fifth valid destination or claim a safe five-argument
grammar until original call data and library/ABI behavior have been traced.

## Remaining record paths and validation obligations (P)

The remaining icon, anim, paper, /paper, r and R-prefixed annotation branches
are specified in [embedded objects](MESSAGE_EMBEDDED_OBJECTS.md), including
literal unreachable closing comparison and the US selection argument census.
Together these files cover local msgAnalize branches; library behavior,
record consumers and full dialogue execution remain separate work.
Local clock/schedule advancement and opcode consumers are now documented in
[message playback](MESSAGE_PLAYBACK.md). Render prefix/controls/history and
embedded objects are covered in [draw admission](MESSAGE_DRAW_RECORD_ADMISSION.md);
normal effects in [glyph effects](MESSAGE_GLYPH_EFFECTS.md). Inner services and
caller contracts remain open.

Differential cases: append with nonzero initial count; startup type 3/8 color;
terminal opcode outside counted records; US high bytes and classification/lookup signedness;
single/two-byte spacing; scale versus scaleX; blank LF and automatic post-command
LF suppression; repeated nonstacking styles; /noise untouched schedule; position
999 sentinel; retained V versus explicit persistent reset; multiple selection
lines and signed byte/halfword boundaries. These are derived test obligations,
not original-execution results. Full rendering, dialogue cadence, story attribution
and event conditions remain unfinished.
