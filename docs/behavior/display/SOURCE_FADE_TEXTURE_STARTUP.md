# Original fade startup texture owner

Fresh read-only US revision0 DOL and original symbols establish distinct
fadeInit80013b90/0x114, fadeTexSetup80013aa4/0xec and callback80013ca4/0x48.
Whole-body SHA256 respectively:
4d9bf07a55b5e2a997ae95ba738d0773587aac5744d4b6ed49976f14a0b56837,
c6bb7058a6a65cb509dd8fad39fbda6e2943c9d2ff76cd782d2355b31f62ad0c,
2e610a930a107427f6da57d729d7126dba310fc846d7a40270258e866f8ad3f7.

## Actual manager initialization

Original fadeInit allocates heap0 size0x360, publishes the actual result through
8041e620 and clears that allocation using original memset. It loads binary32
zero8041f7a8 and one8041f7ac once. Five rows at manager offsets0,0xa8,0x150,
0x1f8 and0x2a0 each use a fresh manager pointer and write word-1 at row+74/+78,
float0 at+24/+28/+80/+84, float1 at+98 and word0 at+ac. These hexadecimal field
offsets and row order are authoritative, including stores extending into the
following stride; do not replace them with an invented struct initializer.
The retained zero/one and -1 values are not refreshed per row. After the rows,
separate fresh manager-pointer reads precede+350=-1,+358=0 and+35c=0.

The manager address must come from the actual source heap; a guessed pre-mapped
manager cannot satisfy this initializer. New global8041e620/4 is original BSS.
Constants8041f7a8/8 are original initialized zero/one. These mappings are not
provided by the environment/effect texture profiles.

## Original texture admission and callback

Texture setup captures the manager and clears only+358 palette, preserving+35c
readiness history. It calls arcOpen800782e4 with802bf79c (`fade.tpl`),0,0,
reloads the manager for publication, then reloads manager and palette for the
branch. A hit calls whole UnpackTexPalette with that fresh palette and reloads
the manager before readiness1.

A miss obtains the actual DVD root, formats802bf7a8 (`%s/fade.tpl`) privately,
opens type2 with zero final operand, captures handle and gets length. It rounds
wrapping(length+31)&~31 and retains that size for BOTH heap0 allocation and
ReadAsync. This differs from the environment driver's logical read size. It
reloads manager before publishing the allocation, stores handle at handle+6c,
reloads manager and palette for ReadAsync(handle,palette,rounded,0,80013ca4),
and returns after job admission. There is no subordinate setup after either
branch and no source readiness wait or null recovery.

The independent callback ignores its first argument. It captures manager FIRST,
then handle from secondArgument+2c, then palette from the captured manager+358.
After whole unpack, it closes the captured handle, reloads the manager and writes
+35c=1. Native submission or file copying alone cannot acknowledge this callback.

## Assets and production frontier

Root fade.tpl is22720 bytes with4 TPL descriptors, ISO offset948116944,
SHA256ce2c99dbe41f28d5c06b777b9b0b19ed5a9fe3829f63952cae9352195fb99435.
New literals802bf79c/0xc and802bf7a8/0x10 preserve original padding.
No fade bank is imported by this contract. Original rights are unchanged.
Do not confuse this bank with
[fade10's embedded white texel](BATTLE_END_FADE10_TEXTURE_CONSUMER.md).

Original marioStInit calls fadeInit at8006fdd4, before shadow/effect/Mario/env
initialization (other intervening children still matter). Original seq_logoExit
calls fadeTexSetup at80087480, after font/window texture setup and before
icon/environment/effect texture setup and marioPoseInit. The native UI prefix
and explicit environment/effect APIs do not implement that whole sequence.

Implement the genuine fade manager and these whole bodies over shared original
VM/heap/SDK/root/archive/unpack ownership before completing normal activation.
The [common native texture transfer backend](../platform/NATIVE_SOURCE_TEXTURE_STARTUP.md)
can supply retained DVD mechanics while preserving fade-specific cursor order.
All bounded direct children are already identified; no duplicate unknown-function
request is needed. Broader fade updates/raster/completion remain with their
existing contracts. Fresh binaries, exact source order, source offsets and hashes
are retained under artifacts/environment-startup/source-evidence.json and
owner-order-and-fade-frontier.txt. No native rendering or hardware performance
claim is made here.
