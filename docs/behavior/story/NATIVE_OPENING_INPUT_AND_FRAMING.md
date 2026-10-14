# New Game dialogue input and house framing

The interactive New Game presentation retains unchanged original US messages,
IDs, control tokens, source order and existing speaker ownership. The explicit
user preference requires character dialogue to wait for user input. This is a
presentation adaptation, not a claim that every original payload contains a
terminal key token.

`OpeningSequence::print` opts into `MessagePlayback::requireTerminalInput`.
Playback reaches an additional waiting state only when the terminal record is
preceded by a glyph after the most recent source key record. A source final
`<k>` already gates that page and does not receive an extra confirmation.
Existing internal source keys, scrolling, animation and accepted A/B trigger
masks remain. The terminal page stays opaque until a subsequent trigger starts
its ordinary closing animation; an acceleration trigger cannot also close it.
`open` resets the opt-in flag. Original book/demo playback and all other source
message owners retain their existing timing. Host checks cover unchanged raw
bytes, indefinitely held unkeyed terminal text, a separately triggered close,
one press for an existing final key and ordinary default auto completion.

The New Game renderer submits `Renderer::originalMessage` on the top screen,
using the existing original message panel/window geometry, masks, tail and
font. House dialogue supplies the current original speaker's rendered source
pose bounds and Mario's actual root. Both project through the world matrix used
for that frame. The new optional anchor arguments leave ordinary field owners
on their existing actor selection path. Messages whose source speaker services
remain unbound use ordinary unanchored window placement; no actor attribution
or replacement dialogue is invented. The lower screen keeps native input and
failure notices, without duplicate character text/name labels.

`frontWorld` uses the recovered camera4 lens25 from
[initialization and projection](../camera/OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md),
instead of the previous hardcoded42. Authored eye/aim coordinates, scale0.01,
curve11 and source house5000ms/2000ms requests are unchanged; the
[read-only house verifier](../../../tools/verify_opening_house_requests.py)
pins those exact requests. The original608x480 world maps to a centered304x240
native viewport. Cropping the centered400x240 projection to304 pixels gives
its source19/15 effective aspect while retaining the actual matrix for speaker
projection. The scissor resets before window/UI drawing. Near/far clipping,
SDK float operations and exact original raster parity remain native adapters.

This closes the concrete wide house framing/input presentation defects; it
does not recover the remaining letter/map/voyage/wake/arrival event services,
original window audio, full original display lifecycle or hardware parity.
