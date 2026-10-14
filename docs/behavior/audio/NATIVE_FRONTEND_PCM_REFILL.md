# Native frontend PCM refill ownership

This is a 3DS port service, not measured GameCube buffer scheduling. Original
frontend BGM envelope controllers retain their existing source requests and
independent primary/tail lifetimes.

Native music uses three fixed owners on channels 1, 0 and 18. Each owner has
three 4096-frame PCM buffers. New owners receive their requested envelope volume
before their first buffer is queued; no temporary full-volume start is published.

A dedicated 16 KiB, priority 0x28 worker refills completed buffers. The NDSP
callback only signals a one-shot event. It never reads files or waits for the
main-thread music lock. The worker and track admission/retirement serialize
file access and buffer ownership through that lock. Texture loading and slow
render frames therefore do not postpone the refill until the next game frame.
These buffer sizes and thread settings are port choices.

Shutdown disables the callback, wakes and joins the worker before closing PCM
files or freeing buffers. Joining never holds the music lock. The callback
runs on libctru's sound thread before DSP publication, as shown in the
[libctru NDSP implementation](https://github.com/devkitPro/libctru/blob/master/libctru/source/ndsp/ndsp.c).

Chest effect textures are admitted with the marketplace before the first
market frame, rather than performing their first file import at chest opening.
This reduces first-use work; it does not establish complete hardware performance
or eliminate every synchronous scene load.

Title images and curtain owners are admitted before the frontend simulation
clock and its first music request start. They reside in GPU VRAM, leaving the linear arena available for the overlapping
book, market and Peach atlas. They survive Demo/Title/Files phase
changes and the final book adjustment, and retire on entering New Game or
Playing. The white chest exit therefore performs no first-use Title import.
This shifts that bounded resource work into initial boot admission.

The original US book scene retains a looping BGM owner until its explicit Off
request. The packaged Switch book arrangement is a shorter nonlooped cue and
exhausts before that request (a 2.262-second digital silence was recorded in
the isolated VRAM run). Book track 5 therefore uses original ISO `sys_demo1_32k`,
`romfs:/demo-book.wav` and its extracted loop sidecar, including when the optional
Switch soundtrack is selected. No fabricated loop points are assigned to the
Switch cue. Other soundtrack selection remains governed by the existing policy.
