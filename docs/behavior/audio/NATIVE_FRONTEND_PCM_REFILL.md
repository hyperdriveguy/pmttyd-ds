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
