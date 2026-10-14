# Native RAM admission interval prefilter

`EventAddressSpace::registerRam` and transactional script import are native
storage adapters, not recovered original VM bounds checks. Before this change,
admission tested every requested byte through all live semantic providers and
existing RAM regions. Native trio startup stalled during its initial RAM
registrations in an isolated Azahar fixture.

The adapter now rejects overlap with existing RAM by half-open interval
intersection, using 64-bit endpoints. RAM contains every byte, so this rejection
is equivalent to the previous readable-byte test. Each live provider exposes a
conservative `mayMapRange` probe: entry registrations use packed stride 0x1b0,
contexts use 0xa0, GP uses its original extent plus the pointer cell, and runtime
uses its two schedule arrays and six mapped scalar words. These probes are used
only after the address space validates a nonempty range within the 32-bit ceiling.

If a live provider's extent intersects, admission retains the original per-byte
`readByte` check. This preserves sparse/unreadable aliases, including a context
entry-array pointer whose host target has no original address mapping. A
conservative overlap alone does not reject registration. If no live provider can
map the range and no RAM overlaps, admission succeeds without byte enumeration.
Read/write precedence, packed byte aliases, retirement, allocation contents and
transactional script publication are unchanged.

Tests compare admission against the previous readable-byte rule around unaligned
RAM/live-region boundaries, runtime holes and the 32-bit ceiling. They explicitly
exercise an unreadable context pointer lane and adjacent readable context bytes.
The isolated original eight-member battle fixture now reaches all initialization
markers and displays the actual original audience sprites. This demonstrates the
native startup improvement; it does not prove normal-story battle admission,
original camera framing, complete battles or lower-screen scene/menu fidelity.
No original asset, story, control or simulation-timing changes are involved.
