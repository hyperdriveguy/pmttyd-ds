#!/usr/bin/env python3
"""Original full native banks for the harbor encounter and its two sailors.

This catalog supplies resources, not actors or event IDs. The original four
direct Entry calls and tagged-map/setup lifecycle still own actor creation.
"""
import argparse
import json
from pathlib import Path
from field_npc_resource_assets import export_model, source_tribes
from iso_assets import DEFAULT_ISO

# Original gor.rel 0x25290 Entry/SetTribe child, followed by the two tagged
# sailors used by the original before-battle tail. The two subordinates share
# one model bank. Keep first-exact-match source tribe identities.
TRIBES = {
    'クリスチーヌ': 'c_pkr',
    '第三勢力幹部': 'c_kanbu_ibt',
    '第三勢力部下': 'c_zako_n',
    'ガラの悪い水夫': 'c_gara',
    '水夫ボム兵': 'c_suifu_b',
}


def build(iso=DEFAULT_ISO, output='romfs', work='local-assets/harbor-native'):
    output, work = Path(output), Path(work)
    output.mkdir(parents=True, exist_ok=True)
    work.mkdir(parents=True, exist_ok=True)
    source = source_tribes(iso)
    tribes, resources = {}, {}
    for tribe, model in TRIBES.items():
        row = source[tribe]
        if row['model'] != model:
            raise ValueError('Original harbor tribe/model identity changed')
        tribes[tribe] = row
        bank = export_model(iso, model, work, output)
        resources[model] = dict(asset=bank['asset'], texture=bank['texture'],
                                descriptors=bank['source_descriptor_order'],
                                fingerprint=bank['fingerprint'])
    catalog = dict(tribes=tribes, resources=resources,
        policy='Full original MODEL/TPL, descriptor order and CPU clips only. '
               'No actor creation, async readiness, pose entry, Main or GPU acknowledgment.',
        license='Nintendo/Intelligent Systems original resources; local use only, no redistribution rights')
    (output / 'harbor-native-npc-resources.json').write_text(
        json.dumps(catalog, ensure_ascii=False, indent=2) + '\n')
    return catalog


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', default='romfs')
    parser.add_argument('--work', default='local-assets/harbor-native')
    args = parser.parse_args()
    build(args.iso, args.output, args.work)
