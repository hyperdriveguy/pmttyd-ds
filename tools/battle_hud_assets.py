#!/usr/bin/env python3
"""Verify original US status-window art and numeral tables, read-only."""
import hashlib
import struct
from pathlib import Path

STATUS_TABLE = 0x8036edd0
DIGIT_TABLE = 0x80415084
STATUS_HASH = 'a44bcbdc5e0be34e351fd345b6b66a914ca77e16a6436c5d2a910d919ac10fbc'
DIGIT_HASH = '54b8acd42d57db411d64d29bb6eb6f2741bd2ecadea16b20bd0752c36e693137'

def source_art(iso):
    with Path(iso).open('rb') as disc:
        header = disc.read(0x440)
        if header[:6] != b'G8ME01' or header[7] != 0:
            raise ValueError('US G8ME01 revision0 required for battle HUD')
        dol = struct.unpack_from('>I', header, 0x420)[0]
        disc.seek(dol)
        sections = disc.read(256)
        def read(address, size):
            for i in range(18):
                offset, base, length = (struct.unpack_from('>I', sections, k+4*i)[0] for k in (0, 0x48, 0x90))
                if base <= address and address+size <= base+length:
                    disc.seek(dol+offset+address-base)
                    data = disc.read(size)
                    if len(data) != size:
                        raise ValueError('Truncated battle HUD source')
                    return data
            raise ValueError('Unmapped battle HUD source')
        status, digits = read(STATUS_TABLE, 156), read(DIGIT_TABLE, 40)
    if hashlib.sha256(status).hexdigest() != STATUS_HASH or hashlib.sha256(digits).hexdigest() != DIGIT_HASH:
        raise ValueError('Unexpected original battle HUD tables')
    rows = [dict(x=x, y=y, icon=icon, scale=scale) for x,y,icon,scale in
            (struct.unpack_from('>hhH2xf', status, 12*i) for i in range(13))]
    numbers = list(struct.unpack('>20H', digits))
    return dict(status_table=STATUS_TABLE, status_sha256=STATUS_HASH, rows=rows,
                digit_table=DIGIT_TABLE, digit_sha256=DIGIT_HASH,
                large_digits=numbers[:10], small_digits=numbers[10:])
