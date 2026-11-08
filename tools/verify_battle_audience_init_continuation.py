#!/usr/bin/env python3
"""Read-only retained audience Init/Setting whole-body and dependency proof."""
import argparse
import hashlib
import struct
import re
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS = [('BattleAudienceSettingAudience', '0x801a60c4', '0xa4c', '21467d6044dd1773f586a7721fd0064ab1bfca57de5f8ba9495831c045cf2e8e'), ('BattleAudienceBaseGetPtr', '0x801a86a0', '0xc', '17096f3ef21a7a0336e1d70a08dc681fa1d734a641d0eef0da749a779d749f79'), ('BattleAudienceGetPtr', '0x801a8664', '0x3c', 'b9357ba56748bd7cca833c66858c0d7c8552b2f84196298ab4e054eefcc73d56'), ('BattleAudience_GetExist', '0x801a29c0', '0x64', '21d4c42ea7469e96002e8a917d4e5ed2736c391d5dcbed4200a0e2ebc445e95d'), ('BattleAudienceWinGetPtr', '0x801a85fc', '0x28', '83072008ebed0da92f94dd4f7c2d89972dcd5873406a460fd21ad99915d4356d'), ('BattleAudience_Init', '0x801a83f8', '0x1c4', 'a926eae5ff02962f79d66e063dc5a345edfceb4e58130f38a13e65db79d25d99'), ('memcpy', '0x8000519c', '0x50', '2ae30d938e412adf2364d7d2c9768f1b36c4b0bcefaf934345ec62b428b2a046'), ('memset', '0x800050b4', '0x30', '4176c17fce5f28ccea737946be853ea45a125b52e9acab356ba54454aa352b5c'), ('pouchGetPtr', '0x800d5eb0', '0x8', 'dbcd2f415efe4aa4e5684ee546cf73d97cc641b5a0290f1b46b14ab606786537'), ('pouchGetAudienceNum', '0x800d3bac', '0xc', '2295cb31b0022a43c846eeabe45ab19049fdfafcd9f0535fed9a05759f362c93'), ('BattleAudienceSetThrowItemMax', '0x801a5a0c', '0xa0', 'dfebbb471c2cb5e209ba0a37dcf78381ceea2264b16a70c279e060106d0a48dc'), ('BattleAudienceGuestTPLRead', '0x801a6050', '0x74', 'b34d46c4ca66a2c36c02eec6b7c57c5ca0c03f18b0528c728616353894b710c2'), ('BattleAudienceSoundNoiseAlways', '0x801a0b88', '0x164', 'e74827a007ccb0d6896b7655bc507f7e14710bdedeb87ab01ae914c5a1125afa'), ('BattleAudienceSoundSetVol', '0x8019fe28', '0x4c', '5ed4ebd79198572ac18db0fe0d2a74dfcc97fb8af766a849b1a3871c3638a464'), ('BattleAudienceNumToTarget', '0x801a1c60', '0x7c', '30e68a9f7d1c3c49a6acbe8d03aa2cb19ad25523b7aaad25accb4509ff6b7bd3'), ('irand', '0x80005da0', '0x44', '0523fae52d8e314f0e46dafe5c3f04f5c4c90ced7628011fdb0cf4efa4ba0daf'), ('LevelTable', '0x802f9eac', '0x104', 'd9c8727be8860337f45b67ad87641ba086e34bb08d911065a4ef995847c92e1d'), ('WeightThresholds', '0x802f9fb0', '0x2c', '9b140ea6c4e7562bce284f741cf74dad6ff5a31f8a0f9d421668f9b625eee73f'), ('SignedDoubleBias', '0x802fa328', '0x8', '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'), ('PairedSpeciesFlags', '0x803b4de4', '0x180', '204d1af06719d3b9a9679aca92ce71fa14a62545af122092cc529566b21438be'), ('OverrideJumpTable', '0x8039cd88', '0x34', 'bf9f58344d809243fea2122ade338d985bc4e5749dfb4250ad77674c8623b5f7'), ('FloatScalars', '0x80424988', '0xf4', '177427ad24035605c75d096c1a58020e4142e15886dd821c814102ea518125fb'), ('ResourceNames', '0x802fa710', '0x38', '1a5d94781be8cee3eb0c097d54d1f63289ea97243755d3baa22c78ecc104b00d'), ('StartupR2R13', '0x80005418', '0x10', '76a62b3dba4a365727ca04077b196ca71058a5e7c9fd484abfe1ad0eeb44bc32')]

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as f:
        if f.read(8)!=b'G8ME01\0\0':
            raise ValueError('US revision0 disc required')
        f.seek(0x420)
        dol,=struct.unpack('>I',f.read(4))
        f.seek(dol)
        h=f.read(256)
        def read(address,size):
            for i in range(18):
                off,base,n=(struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90))
                if base<=address and address+size<=base+n:
                    f.seek(dol+off+address-base)
                    data=f.read(size)
                    if len(data)!=size:
                        raise ValueError('Truncated original source')
                    return data
            raise ValueError('Unmapped original source')
        for name,address,size,digest in SPANS:
            if hashlib.sha256(read(int(address,16),int(size,16))).hexdigest()!=digest:
                raise ValueError('Original source changed: '+name)
    cpp = (Path(__file__).resolve().parents[1] / 'source/battle/battle_audience_init_continuation.cpp').read_text()
    translated = cpp[cpp.index('bool Program::run() {'):]
    actual = [int(a,16) for a in re.findall(r'case (0x[0-9a-f]+):', translated)]
    expected = set(range(0x801a60c4,0x801a60c4+0xa4c,4)) | set(range(0x801a83f8,0x801a83f8+0x1c4,4))
    if len(actual)!=len(expected) or set(actual)!=expected:
        raise ValueError('Finite source translation has a missing or duplicate original instruction case')
    return dict(source='US G8ME01 revision0',spans=len(SPANS),translated_instructions=len(actual),
        scope='Whole audience Init/Setting and used leaves/data; typed child ownership and finite host numeric policy remain explicit',
        original_executed=False)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(p.parse_args().iso))
