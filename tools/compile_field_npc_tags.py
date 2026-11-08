#!/usr/bin/env python3
"""Compile editable original NPC tag text into the existing native parsed view."""
import math
import struct
from field_map_npc_assets import parsed_records


def encode_field_npc_tags(raw):
    rows = parsed_records(raw)
    result = bytearray(struct.pack('<8sII', b'PMFMNPC1', len(rows), 392))
    for ordinal, strings, numbers in rows:
        if not all(math.isfinite(value) for value in numbers):
            raise ValueError('NPC tag coordinates/ranges must be finite')
        try:
            result += struct.pack('<I17f', ordinal, *numbers)
        except (OverflowError, struct.error) as error:
            raise ValueError('NPC tag scalar does not fit binary32') from error
        result += b''.join(value.ljust(64, b'\0') for value in strings)
    return bytes(result)
