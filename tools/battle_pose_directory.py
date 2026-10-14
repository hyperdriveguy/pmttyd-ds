"""Retain original model descriptor order beside locally converted subsets."""
from pathlib import Path
import struct


def source_directory(raw):
    if len(raw) < 0x1b0:
        raise ValueError('Truncated original model header')

    def name(offset, size):
        field = raw[offset:offset + size]
        if len(field) != size or b'\0' not in field:
            raise ValueError('Truncated or unterminated original name')
        value = field.split(b'\0', 1)[0].decode('ascii')
        if not value:
            raise ValueError('Empty original resource or pose name')
        return value

    count = struct.unpack_from('>I', raw, 0x148)[0]
    offset = struct.unpack_from('>I', raw, 0x1ac)[0]
    if not count or offset < 0x1b0 or offset + count * 64 > len(raw):
        raise ValueError('Invalid original descriptor directory extent')
    return (name(4, 64), name(0x44, 64),
            [name(offset + i * 64, 60) for i in range(count)])


def write_directory(model, output):
    model_name, texture_name, names = source_directory(Path(model).read_bytes())
    def field(value):
        return value.encode('ascii').ljust(64, b'\0')
    Path(output).write_bytes(b'PMPOSE01' + field(model_name) + field(texture_name)
                            + struct.pack('<I', len(names))
                            + b''.join(field(value) for value in names))
    return model_name, texture_name, names
