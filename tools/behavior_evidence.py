#!/usr/bin/env python3
"""Read-only US binary identity and behavior research index; emits metadata only.

External symbols are annotations, not proof of behavior. Direct branch records
are instruction-site inventory, not an execution trace or complete call graph.
"""
import argparse
import csv
import hashlib
import json
import struct
from pathlib import Path

from iso_assets import DEFAULT_ISO, inventory


FUNCTIONS = (
    'marioStMain', 'seqMain', 'evtGetWork', 'evtmgrMain', 'evtmgrCmd',
    'evt_wait_msec', 'evtChildEntry', 'evtDelete', 'evtGetValue', 'evtSetValue',
    'make_pri_table', 'evtBrotherEntry', 'evtStopAll', 'evtStartAll',
    'evtStop', 'evtStart', 'evtRestart',
    'BattleActionCommandCheckDefence', 'BattleActionCommandSetDifficulty',
    'BattleActionCommandResetDefenceResult', 'BattleACPadCheckRecordTrigger',
    'BtlPad_WorkInit', 'BtlPad_WorkUpdate', 'BattleActionCommandManager',
    'BattleActionCommandSetup', 'BattleCheckDamage', 'BattleDamageDirect',
    'BSE_TurnFirstProcessEffectMain', 'BSE_TurnFirstProcessEffectEntry',
    'btlevtcmd_CheckDataOfDataTable', 'btlevtcmd_RunDataEventChild',
    'btlevtcmd_StopWaitEvent', 'btlevtcmd_StartWaitEvent',
    'btlevtcmd_CheckStatus', 'btlevtcmd_CheckDefinePosetype',
    'btlevtcmd_AnimeChangePoseFromTable', 'btlevtcmd_SetFallAccel',
    'BattleWaitAllActiveEvtEnd_NoBgSetEndWait', 'BattleCheckConcluded',
    'BattleAfterReactionMain', 'BattleAfterReactionRelease',
    'pouchRemoveItem', 'pouchRemoveItemIndex', 'btlevtcmd_ConsumeItemReserve',
    'msgSearch', 'msgLoad', 'msg_compare', 'msgGetCommand', '_ismbblead',
    'msgAnalize', 'msgInit', 'msgIconStr2ID',
    'msgMain', 'keyGetButtonTrg',
    'windowEntry', 'windowGetPointer', 'windowCheckID',
    'msgWindow_Entry', 'msgWindow_Delete', 'msgWindow_Add',
    'msgWindow_Continue', 'msgWindow_Repeat', 'msgWindow_ForceClose',
    'evt_msg_close', 'evt_msg_repeat', 'evt_msg_continue',
    'selectWindow_Main', 'evt_msg_select', 'keyGetDirTrg',
    'keyGetButtonRep', 'keyGetDirRep', 'windowDeleteID', 'windowDelete',
    'msgWindow_Main', 'msgWindow_Clear_Main',
    'btlevtcmd_AnnounceMessage', 'btlevtcmd_AnnounceSetParam',
    'BattlePadCheckTrigger', 'BattlePadMultiCheckTrigger', '_disp@battle_message.o',
    'evt_btl_camera_set_mode', 'evt_btl_camera_set_homing_unit',
    'evt_btl_camera_set_moveSpeedLv', 'evt_btl_camera_set_zoom',
    'btl_camera_set_mode', 'btl_camera_set_moveSpeedLv', 'btl_camera_set_zoom',
    'btl_camera_nomove_x_onoff', 'btl_camera_nomove_y_onoff',
    'btl_camera_nomove_z_onoff',
    'evt_sub_intpl_init', 'evt_sub_intpl_get_value', 'intplGetValue',
    'btlevtcmd_SetDispOffset', 'BtlUnit_SetDispOffset',
    'BattleCommandInit', 'battleCameraGetPosMoveSpeed',
    'BtlUnit_SetTotalHitDamage',
    'btlseqPhase', 'btlseqMove', 'btlseqAct', 'btlseqPhaseFirstProcess',
    'battleSortPhaseMoveTable', 'battleMakePhaseEvtTable',
    'BattlePhaseEndCheck', 'BtlUnit_GetHomePos',
    'btlseqTurnFirstProcess',
    'BtlUnit_ResetMoveStatus', 'BtlUnit_CanActStatus', 'BtlUnit_CanGuardStatus',
    'btlevtcmd_SetMaxMoveCount', 'btlevtcmd_CheckActStatus',
    'BattleConsumeReserveItem', 'BattleAfterReactionEntry',
    'BattleAfterReactionQueueInit', 'btlevtcmd_AfterReactionEntry',
    'BattleTransID', 'btlevtcmd_GetBodyId', 'btlevtcmd_AnimeChangePoseType',
    'BtlUnit_SetFallAccel', 'BtlUnit_GetData',
    'BtlUnit_CheckShadowGuard', 'BtlUnit_RecoverHp',
    'btlseqTurn', 'BattleWaitAllActiveEvtEnd',
    'BattleIncSeq', 'BattleGetSeq', 'BattleSetSeq',
    'BattleCheckPikkyoro', 'BtlUnit_GetACPossibility', 'BtlUnit_CheckStatusFlag',
    'BattlePreCheckDamage', 'BattleCalculateDamage', 'BattleCalculateFpDamage',
    'btlevtcmd_ResultACDefence',
    'BattleCheckCounter', 'BattleInitCounterPreCheckWork',
    '_checkDamageCode_EmergencyRevival',
    'BtlUnit_GetFp', 'BtlUnit_SetFp', 'BtlUnit_GetMaxFp', 'BtlUnit_RecoverFp',
    'BattleGetMarioPtr',
    'BattleRunHitEvent', 'BattleRunHitEventDirect', 'BtlUnit_CheckData',
    'BattleRunWaitEvent', 'BattleRunPhaseEvent', 'evtDeleteID', 'evtEntry',
    'btlevtcmd_SetEventDamage', 'btlevtcmd_RunHitEventDirect',
    'btlevtcmd_CheckDamageCode', 'btlevtcmd_GetDamageCode',
    'BtlUnit_CheckStatus', 'BtlUnit_GetStatus', 'BtlUnit_SetStatus',
    'BtlUnit_CheckRecoveryStatus', '_CheckMoveCount',
    'BattleSetStatusDamageFromWeapon', 'BattleSetStatusDamage',
    '_getSickStatusParam', '_getSickStatusRate', '_getRegistStatus',
    'swByteGet', 'swByteSet', '_swByteGet', '_swByteSet', 'swGet', 'swSet',
    'swClear', 'swReInit', 'swInit', 'pouchSetHP', 'pouchAddHP', 'pouchSetFP',
    'pouchSetMaxHP', 'pouchSetMaxFP', 'pouchSetPartyHP', 'pouchSetAP',
    'pouchAddAP', 'pouchSetCoin', 'pouchAddCoin', 'pouchGetEmptyHaveItemCnt',
    'pouchGetItem', 'pouchGetEmptyKeepItemCnt', 'rand', 'irand',
)


def report(iso_path, symbols_path):
    info = inventory(iso_path)
    if info['disc_id'] != 'G8ME01':
        raise ValueError('US G8ME01 required; addresses below are version-specific')
    symbol_bytes = Path(symbols_path).read_bytes()
    rows = list(csv.DictReader(symbol_bytes.decode('utf-8').splitlines()))
    names = {int(r['ram_addr'], 16): r['name'] for r in rows
             if r['area'] == '_main' and r['ram_addr']}
    with Path(iso_path).open('rb') as disc:
        def read_at(offset, size):
            if offset < 0 or size < 0 or offset + size > info['disc_size']:
                raise ValueError('Read outside disc')
            disc.seek(offset)
            data = disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated disc')
            return data

        header = read_at(0, 0x440)
        dol_offset = struct.unpack_from('>I', header, 0x420)[0]
        dol_header = read_at(dol_offset, 0x100)
        sections = []
        for i in range(18):
            offset = struct.unpack_from('>I', dol_header, i * 4)[0]
            address = struct.unpack_from('>I', dol_header, 0x48 + i * 4)[0]
            size = struct.unpack_from('>I', dol_header, 0x90 + i * 4)[0]
            if size:
                if offset < 0x100:
                    raise ValueError('Section overlaps DOL header')
                sections.append(dict(index=i, kind='text' if i < 7 else 'data',
                                     file_offset=offset, address=address, size=size))

        def at_va(address, size):
            matches = [s for s in sections if s['address'] <= address
                       and address + size <= s['address'] + s['size']]
            if len(matches) != 1:
                raise ValueError(f'Ambiguous/unmapped DOL address {address:#x}')
            s = matches[0]
            return read_at(dol_offset + s['file_offset'] + address - s['address'], size)

        functions = []
        for name in FUNCTIONS:
            symbol_name, separator, symbol_namespace = name.partition('@')
            matches = [r for r in rows if r['area'] == '_main'
                       and r['sec_type'] == 'text' and r['name'] == symbol_name
                       and (not separator or r['namespace'] == symbol_namespace)]
            if len(matches) != 1:
                raise ValueError(f'Expected unique text symbol: {name}')
            row = matches[0]
            address, size = int(row['ram_addr'], 16), int(row['size'], 16)
            if size <= 0 or size % 4 or address % 4:
                raise ValueError('Invalid instruction range')
            data = at_va(address, size)
            if data != read_at(dol_offset + int(row['file_addr'], 16), size):
                raise ValueError('Symbol file/RAM mapping disagrees with DOL')
            calls = []
            for offset in range(0, size, 4):
                word = struct.unpack_from('>I', data, offset)[0]
                # PPC opcode 18, LK=1. Conditional/indirect calls are excluded.
                if word >> 26 == 18 and word & 1:
                    displacement = word & 0x03fffffc
                    if displacement & 0x02000000:
                        displacement -= 0x04000000
                    target = (displacement if word & 2 else
                              address + offset + displacement) & 0xffffffff
                    calls.append(dict(site=address + offset, target=target,
                                      annotation=names.get(target)))
            functions.append(dict(name=name, address=address, size=size,
                                  sha256=hashlib.sha256(data).hexdigest(),
                                  direct_branch_link_sites=calls))

        recovery_branches = []
        move_reset_branches = []
        for section in sections:
            if section['kind'] != 'text':
                continue
            data = at_va(section['address'], section['size'])
            for offset in range(0, len(data) - 3, 4):
                word = struct.unpack_from('>I', data, offset)[0]
                if word >> 26 != 18:
                    continue
                displacement = word & 0x03fffffc
                if displacement & 0x02000000:
                    displacement -= 0x04000000
                site = section['address'] + offset
                target = (displacement if word & 2 else site + displacement) & 0xffffffff
                if target == 0x80127890:
                    recovery_branches.append(dict(site=site, link=bool(word & 1)))
                if target == 0x801266d4:
                    move_reset_branches.append(dict(site=site, link=bool(word & 1)))

        # US dispatch address recovered from evtmgrCmd's table load.
        dispatch = [struct.unpack('>I', at_va(0x80304fd0 + i * 4, 4))[0]
                    for i in range(119)]
        if any(not 0x800316b8 <= target < 0x80032a70 for target in dispatch):
            raise ValueError('Unexpected US event dispatch table')
        dol_size = max(s['file_offset'] + s['size'] for s in sections)
        assets = []
        for item in info['files']:
            if item['path'] == 'rel/gor.rel' or item['path'].startswith('msg/US/'):
                assets.append(dict(item, sha256=hashlib.sha256(
                    read_at(item['offset'], item['size'])).hexdigest()))
        return dict(
            schema=1, evidence_kind='static inventory; not observed execution',
            disc=dict(id=info['disc_id'], disc_number=header[6], revision=header[7],
                      size=info['disc_size'], file_count=len(info['files']),
                      header_sha256=hashlib.sha256(header).hexdigest()),
            dol=dict(iso_offset=dol_offset, span_size=dol_size,
                     sha256=hashlib.sha256(read_at(dol_offset, dol_size)).hexdigest(),
                     entry_point=struct.unpack_from('>I', dol_header, 0xe0)[0],
                     bss_address=struct.unpack_from('>I', dol_header, 0xd8)[0],
                     bss_size=struct.unpack_from('>I', dol_header, 0xdc)[0],
                     sections=sections),
            recovery_direct_branches=recovery_branches,
            move_reset_direct_branches=move_reset_branches,
            symbols_sha256=hashlib.sha256(symbol_bytes).hexdigest(),
            functions=functions, event_dispatch_address=0x80304fd0,
            event_dispatch_targets=dispatch, assets=assets,
            defence_window_tables={hex(a): list(at_va(a, 7))
                                   for a in (0x802ee018, 0x802ee020)},
            rel_paths=[x['path'] for x in info['files'] if x['path'].endswith('.rel')])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--symbols', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args()
    output = Path(args.output)
    if output.resolve() in (Path(args.iso).resolve(), Path(args.symbols).resolve()):
        raise ValueError('Output must not overwrite an input')
    data = report(args.iso, args.symbols)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(data, indent=2) + '\n')
    print(f"{data['disc']['id']} revision {data['disc']['revision']}: "
          f"{data['disc']['file_count']} files, {len(data['rel_paths'])} RELs, "
          f"{len(data['functions'])} function fingerprints")
    print('DOL SHA256: ' + data['dol']['sha256'])


if __name__ == '__main__':
    main()
