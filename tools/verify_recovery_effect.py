#!/usr/bin/env python3
"""Pin read-only original recovery/Maya bodies and embedded resource spans.

These identities support static recovery; they do not prove runtime semantics,
original raster fidelity, service admission or hardware behavior.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

BODIES = {'effRecoveryDisp4': (2148800552, 1048, 'a1385b9caae3a283b3f403ae61ffeaadd3a62212af373cd4efaa2fe1f3d3ea7b'), 'effRecoveryDisp3': (2148801600, 1212, 'd48e855c47e397bd1e2619849d8c9e03cdcc3c609c6f0cd07fb2e0c01c370053'), 'effRecoveryDisp2': (2148802812, 504, '92ee0ff2c7b9763d7060422c96edf7ec54efdeb505b6529320551ac2c89f64fc'), 'effRecoveryDisp': (2148803316, 1668, '6ffcafbc6c94e19ceec8a766f2f36276d5855d4eba7e11bf6b85431e7e2cad4a'), 'effRecoveryMain2': (2148804984, 588, 'e5bb7b0f2804b08fa238913a7035a55b3d69181f0a44ddc8854d96f8c75e9429'), 'effRecoveryMain': (2148805572, 1944, 'd266425c58c41bb7365b6fde65d0bf9146dd7441346e0cd48c5c6b46a9b166a7'), 'effRecoveryEntry': (2148807516, 1024, 'ebd2023da8d46a1e3761aef4a6fe0298117757b7fc85a10b8d0e4570d8da74c1'), 'effMayaAnimAlloc': (2147869840, 100, '98e271986906846946ba73bc202222560cae5e1943303f27d4cbec25c14dfb6a'), 'effPlayMayaAnim': (2147869660, 140, '8ce41174e9112188685f3cbc8020489dea26076c8f4104ab1c1ecc3053fb2f98'), 'effCalcMayaAnim': (2147869320, 340, '3f7ac7c05f7f7547cd0821d0d01ac1f7cf7471dbf0242d67b46e24bfde993dc1'), 'effDeleteMayaAnim': (2147869800, 40, '34e4b5c05d995f1594e8f0388e2a818ebc475ab15da19f473d0b399c9f3cfd42'), 'effCalcMayaAnimMatrix': (2147869940, 940, '63687aab6932eaa72843975812e0b28086b629fc65e2c55956a7ecfec83b1ca2')}
DATA = {'signed_conversion_bias': (2150375192, 8, '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'), 'recovery_vectors_and_sfx_names': (2150578976, 368, '370990239e7acdcd71cde15b50dee653d805b8a03a82f7db3697a689ed7b26fd'), 'recovery_maya_keys': (2151096232, 1780, 'd542e5932a5bcc4495cef13e2003fbea722357f5f0b586bdff72650b098ee169'), 'recovery_dispatch_tables': (2151098012, 136, 'be04a22a505ce1b05a140e95334382bdec09c1c47fff8ffb022dcbf856aa8869'), 'recovery_colors': (2151774680, 36, '5ad6edf2f2b9ef6b5f6261092bb09e45511f3bc74727dc33db6bbd7288415133'), 'recovery_maya_face': (2151774716, 42, '17ffd5fd5f9289c45a9d147eb1b6452e21109cfe5c533f684544eccc680224b3'), 'recovery_maya_descriptor_graph': (2151774760, 40, '10f38b335f4f96cdac774d6dc0961905ed1340bc01192fd3c3c629f646a4fd7f'), 'maya_lifecycle_constants': (2151809436, 28, 'ec69344fc5bc1c3aa659cac8ee80a26592bf7546badcc1767ecabc35aebcb639'), 'recovery_constants': (2151820720, 156, '95aafb2cfffba8518de8f605d8854ec97b4ef9ccffd3c6e159c62a8d61f2bc2d')}


def verify(iso):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US G8ME01 disc0 revision0 required')
        source.seek(0x420)
        dol, = struct.unpack('>I', source.read(4))
        source.seek(dol)
        header = source.read(256)
        if len(header) != 256:
            raise ValueError('Truncated DOL')
        for name, (address, size, digest) in {**BODIES, **DATA}.items():
            sections = [tuple(struct.unpack_from('>I', header, p+4*i)[0]
                              for p in (0, 0x48, 0x90)) for i in range(18)]
            matches = [o+address-a for o, a, n in sections
                       if o and a <= address and address+size <= a+n]
            if len(matches) != 1:
                raise ValueError('Unmapped/ambiguous original span: '+name)
            source.seek(dol+matches[0])
            raw = source.read(size)
            if len(raw) != size or hashlib.sha256(raw).hexdigest() != digest:
                raise ValueError('Original source identity differs: '+name)
    return dict(schema=1, bodies=len(BODIES), data_spans=len(DATA),
                scope='Whole recovery and Maya lifecycle bodies/resources; static evidence only; graphics and native admission remain separate')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output')
    args = parser.parse_args()
    result = json.dumps(verify(args.iso), indent=2)+'\n'
    if args.output:
        Path(args.output).write_text(result)
    else:
        print(result, end='')


if __name__ == '__main__':
    main()
