# Original sscanf entry and string-reader protocol

US G8ME01 revision0, recovered2026-10-12. Evidence B: static original DOL
sscanf0x8026c344,size0xc4 and __StringRead0x8026c408,size0x90. These services are
dependencies of [message width icons](../message/FONT_MESSAGE_WIDTH.md) and
[embedded objects](../message/MESSAGE_EMBEDDED_OBJECTS.md). No original scan execution or
host-libc equivalence was established. See [formatter consumers](SCAN_FORMAT_CONSUMERS.md)
for conversion/output behavior and [varargs service](FORMAT_OUTPUT_RUNTIME.md)
for the shared __va_arg implementation.

## Entry and ABI (B)

sscanf receives input pointer r3, format pointer r4 and destination-pointer
varargs in subsequent argument slots. Save r3..r10 to the stack register-save
area. Save f1..f8 as doubles only when condition-register bit6 is set. Build the
12-byte varargs record with word0=0x02000000,+4=caller overflow area,+8=save base:
general index2 skips the input/format arguments, floating index starts0.
Icon callers clear bit6 and pass destination pointers, including the float
output pointer, as general arguments; they do not pass a floating value in f1.

If input is null or its first byte is NUL, return-1 immediately. This occurs
before the formatter consumes the format: even an empty format or a `%n` cannot
bypass the entry's empty-input rule. A nonnull input with a nonzero first byte
creates reader context {cursor=input,EOFflag=0} and calls
__sformatter(__StringRead,&context,format,&varargs). Return its word unchanged.
No input buffer write, destination initialization, capacity parameter, errno
write or post-scan termination happens in the wrapper. Null format is unchecked
on the delegated route. Conversion results are established only by destination
writes in the formatter, not by the initial contents of caller stack fields.

## Reader interface and actions (B)

__StringRead receives(context,value,action) in r3/r4/r5. The context has cursor
word+0 and EOF flag word+4. Its action behavior is:

| Action | Operation and return |
| --- | --- |
| 0 | Read unsigned byte at cursor. If nonzero, advance cursor one byte and return0..255. If zero, keep cursor, set EOFflag=1 and return-1. |
| 1 | If EOFflag==0, decrement cursor one byte. Otherwise clear EOFflag without changing cursor. Return incoming value unchanged. |
| 2 | Return EOFflag unchanged. |
| Other signed action values | Return0 without changing context. |

Read action0 ignores value. Unread action1 does not verify that value matches
the preceding byte, write that value into input, or check the beginning of the
buffer. Multiple unreads can move before input. An EOF unread clears the flag
rather than moving before the terminal NUL. Reading a nonzero byte does not
explicitly clear a previously set flag; ordinary formatter calls normally clear
it through the unread protocol. An action2 query does not inspect the cursor.
The context/input pointers are unchecked locally.

The raw reader distinguishes byte0xff (return255 and advance) from EOF(-1 and
no advance). Several formatter paths subsequently sign-extend a stored byte,
losing that distinction; [consumer branch details](SCAN_FORMAT_CONSUMERS.md)
must therefore be retained. Do not implement all callback sites with one generic
signed-char or Unicode abstraction.

## Observable scope and remaining work

These local bodies define the stack ABI, empty-input gate and callback state
mutation order. Whole-function fingerprints and independent anchors pin those
sites; the ignored disassemblies are inspection artifacts, not code linked into
the native game. No source message text was rewritten or exported by this step.

Validate empty/null input, high bytes, NUL, unread after EOF versus after a real
byte, repeated unread and a caller-provided callback/context through original
execution. Compose with the recovered [directive parser](SCAN_DIRECTIVE_PARSER.md) and
recover numeric/multibyte converters;
compare destination writes and return values for the actual original argument
corpus. The runtime contracts do not establish complete icon/dialogue or game
fidelity, and do not assert Switch scanner behavior.
