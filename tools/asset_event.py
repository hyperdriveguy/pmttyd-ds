"""Editable original-address event/data JSON -> native PMEVT001 container."""
import struct

def encode(doc):
    if doc.get('schema') != 1 or not isinstance(doc.get('segments'), list) or not doc['segments']:
        raise ValueError('Expected event schema1 and nonempty segments')
    spans = []
    for row in doc['segments']:
        address = row.get('address')
        if type(address) is not int or address < 0 or address > 0xffffffff or address & 3:
            raise ValueError('Expected aligned original address')
        try:
            data = bytes.fromhex(row['bytes'])
        except (KeyError, TypeError, ValueError) as error:
            raise ValueError('Expected hexadecimal source bytes') from error
        if not data or len(data) & 3 or address + len(data) > 0x100000000:
            raise ValueError('Invalid aligned source extent')
        spans.append((address, data))
    spans.sort()
    previous = 0
    for address, data in spans:
        if address < previous:
            raise ValueError('Overlapping event segments')
        previous = address + len(data)
    offset = 12 + 12 * len(spans)
    table = bytearray(); payload = bytearray()
    for address, data in spans:
        if offset + len(data) > 0xffffffff:
            raise ValueError('Event output exceeds format extent')
        table += struct.pack('>III', address, len(data), offset)
        payload += data; offset += len(data)
    return b'PMEVT001' + struct.pack('>I', len(spans)) + table + payload
