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


def write_directory(model, output, texture=None):
    raw = Path(model).read_bytes()
    model_name, texture_name, names = source_directory(raw)
    def field(value):
        return value.encode('ascii').ljust(64, b'\0')
    metadata = bytearray(struct.pack('<I', struct.unpack_from('>I', raw, 0xc4)[0]))
    directory = struct.unpack_from('>I', raw, 0x1ac)[0]
    for i in range(len(names)):
        offset = struct.unpack_from('>I', raw, directory + i * 64 + 60)[0]
        if offset < 0x1b0 or offset + 0x44 > len(raw):
            raise ValueError('Invalid original animation header extent')
        count = struct.unpack_from('>I', raw, offset + 0x20)[0]
        timeline = offset + struct.unpack_from('>I', raw, offset + 0x40)[0]
        if count > 65536 or timeline < offset or timeline + count * 8 > len(raw):
            raise ValueError('Invalid original discrete timeline extent')
        metadata += struct.pack('<I', count)
        for j in range(count):
            # Reverse each original word independently; preserve float bits,
            # order and values, including empty/duplicate/unordered timelines.
            record = raw[timeline + j * 8:timeline + (j + 1) * 8]
            metadata += record[:4][::-1] + record[4:][::-1]
    Path(output).write_bytes(b'PMPOSE02' + field(model_name) + field(texture_name)
                            + struct.pack('<I', len(names))
                            + b''.join(field(value) for value in names) + metadata)
    # Original data-only MODEL companion preserves group shape IDs and fan
    # streams omitted by the evaluated animation subset. Never source-controlled.
    suffix = '.pose-directory.bin'
    if str(output).endswith(suffix):
        Path(str(output)[:-len(suffix)] + '.draw-model.bin').write_bytes(raw)
        if texture is not None:
            Path(str(output)[:-len(suffix)] + '.draw-textures.bin').write_bytes(Path(texture).read_bytes())
    return model_name, texture_name, names
