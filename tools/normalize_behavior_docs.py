#!/usr/bin/env python3
"""Normalize behavior documentation into per-subsystem folders.

Moves flat Markdown files in ``docs/behavior/`` into the subsystem folder that
owns them and rewrites every relative Markdown link in the repository so the
documentation graph keeps resolving. Only ``README.md`` (the collection index)
stays at the ``docs/behavior`` root.

The script is idempotent: with no flat files to move it changes nothing. Run it
with no arguments for a dry-run plan, or ``--apply`` to move files and rewrite
links. New subsystem folders should be added to ``FOLDERS`` and ``PREFIX``.

This tool edits documentation only. It never executes original code.
"""
import argparse
import os
import re
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BEHAVIOR = os.path.join(REPO, 'docs', 'behavior')

# Files that are renamed as part of normalization (old basename -> new basename).
RENAME = {
    'BATTLE_ACTION_COMMAND_MANAGER.md': 'BATTLE_ACTION_COMMAND_MANAGER_REFERENCE.md',
    'BATTLE_MAIN.md': 'BATTLE_MAIN_REFERENCE.md',
    'BATTLE_AC_COMMAND_MD.md': 'BATTLE_AC_MAIN_TIMING_A_REFERENCE.md',
    'BATTLE_RESULT_TIMING_A_MD.md': 'BATTLE_AC_RESULT_TIMING_A_REFERENCE.md',
    'BATTLE_STICK_KEEP_LEFT_DELETE_MD.md': 'BATTLE_AC_DELETE_STICK_KEEP_LEFT_REFERENCE.md',
    'BATTLE_STICK_KEEP_LEFT_MD.md': 'BATTLE_AC_MAIN_STICK_KEEP_LEFT_REFERENCE.md',
    'BATTLE_STICK_KEEP_LEFT_RESULT_MD.md': 'BATTLE_AC_RESULT_STICK_KEEP_LEFT_REFERENCE.md',
    'BATTLE_TIMINGA_GET_SUCCESS_FRAME_MD.md': 'BATTLE_AC_TIMING_A_GET_SUCCESS_FRAME_REFERENCE.md',
    'binaries.md': 'BINARY_ARTIFACTS.md',
}

# Explicit folder assignments that override the prefix table.
EXPLICIT = {
    'US_COMPLETION_AND_PRIORITIES.md': 'meta',
    'US_MESSAGE_INVENTORY_EVIDENCE_CORRECTIONS.md': 'meta',
    'RANDOMNESS.md': 'meta',
    'UNADDRESSED_BINARY_FUNCTIONS.md': 'binary',
    'REFERENCE_FILES_INVENTORY.md': 'binary',
    'BINARY_ARTIFACTS.md': 'binary',
    'GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md': 'platform',
    'EVENT_DISC_THREAD_READINESS_GATE.md': 'platform',
    'DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md': 'platform',
    'EVENT_RETRACE_TIME_PUBLICATION.md': 'platform',
    'BTL_CAMERA_SET_ZOOM.md': 'camera',
    'CAM_GET_CUR_PTR.md': 'camera',
    'FIELD_SIMPLE_CAMERA_DIRECTION.md': 'camera',
    'STATE_AND_INVENTORY.md': 'unit',
    'INVENTORY_REMOVAL_AND_RESERVATIONS.md': 'unit',
    'AFFINE_MATRIX_SERVICES.md': 'display',
    'HSV_COLOR_CONVERSION.md': 'display',
    'ANIM_GET_PTR.md': 'display',
    'FLOOR_NUMERIC_SERVICE.md': 'format',
    'MULTIBYTE_CONVERSION.md': 'format',
    'ICON_DIMENSIONS.md': 'message',
    'AUDIO_DISC_COVER_STATE_REQUESTS.md': 'audio',
    'PHYSICAL_INPUT_PRODUCER.md': 'input',
    'CHRISTINE_HELPER_ROLES.md': 'story',
    'STORY_HARBOR_OPENING_BINDINGS.md': 'story',
}

# Prefix rules, evaluated in order. First match wins.
PREFIX = (
    ('REFERENCE_', 'binary'),
    ('BATTLE_INPUT_', 'input'),
    ('BATTLE_', 'battle'), ('BTL_', 'battle'), ('BTLEVTCMD_', 'battle'),
    ('WEAPON_', 'battle'), ('NORMAL_JUMP_', 'battle'), ('JUMP_MAJINAI_', 'battle'),
    ('EVENT_', 'event'),
    ('FIELD_PARTNER_', 'partner'), ('FIELD_PARTY_', 'partner'), ('FIELD_NPC_', 'partner'),
    ('FIELD_', 'field'),
    ('OVERWORLD_CAMERA_', 'camera'), ('OVERWORLD_', 'overworld'),
    ('MESSAGE_', 'message'), ('FONT_', 'message'), ('WINDOW_', 'message'), ('ICON_', 'message'),
    ('EFFECT_', 'effect'), ('MAJINAI_', 'effect'),
    ('DISPLAY_', 'display'), ('DISP_', 'display'), ('ANIMATION_', 'display'), ('ANIM_', 'display'),
    ('UNIT_', 'unit'), ('POUCH_', 'unit'),
    ('HARBOR_', 'story'), ('LECTURE_', 'story'),
    ('FORMAT_', 'format'), ('SCAN_', 'format'), ('DECIMAL_', 'format'),
    ('POWER_', 'format'), ('SQUARE_ROOT_', 'format'),
    ('FILE_', 'file'),
    ('SMART_', 'memory'), ('SDK_HEAP_', 'memory'), ('MEMORY_POOL_', 'memory'), ('MAP_BUFFER_', 'memory'),
)

FOLDERS = (
    ('meta', 'Collection-level audits, completion register and RNG baseline'),
    ('platform', 'Boot, retrace/main loop, disc-thread and clock publication'),
    ('event', 'Event VM, opcodes, constructors, scheduling and dispatcher'),
    ('battle', 'Battle/turn engine, action commands, weapons and tutorial encounters'),
    ('input', 'Physical input producer and battle input cadence/snapshots'),
    ('unit', 'Unit/part/pouch/inventory records and stats'),
    ('partner', 'Party, partner and NPC motion, forms and presentation'),
    ('field', 'Field entry, intersections and non-partner field services'),
    ('overworld', 'Overworld Mario movement, roads and motion arbitration'),
    ('camera', 'Camera containers, paths, follow and collision targets'),
    ('effect', 'Effect drivers, pools, Majinai and effect lifetime'),
    ('display', 'Display queue/sort, animation and matrix/colour services'),
    ('message', 'Message resources, windows, fonts, icons and print events'),
    ('audio', 'Audio request and disc-cover state services'),
    ('story', 'Story, lecture and harbor event bindings'),
    ('file', 'DVD/file requests, cache and handle ownership'),
    ('memory', 'SDK/smart heaps, pools and buffer release'),
    ('format', 'printf/scanf, decimal, power and numeric library services'),
    ('binary', 'Binary-artifact inventories and unaddressed-function register'),
)

LINK = re.compile(r'\]\(([^)]+)\)')


def classify(old_name):
    new_name = RENAME.get(old_name, old_name)
    if old_name in EXPLICIT:
        return EXPLICIT[old_name], new_name
    for prefix, folder in PREFIX:
        if new_name.startswith(prefix):
            return folder, new_name
    return 'meta', new_name


def flat_files():
    return sorted(
        f for f in os.listdir(BEHAVIOR)
        if f.endswith('.md') and f != 'README.md' and os.path.isfile(os.path.join(BEHAVIOR, f))
    )


def is_tracked(rel):
    return subprocess.run(
        ['git', 'ls-files', '--error-unmatch', rel],
        cwd=REPO, capture_output=True).returncode == 0


def move_flat_files(apply):
    moves = {}
    for old in flat_files():
        folder, new = classify(old)
        old_rel = os.path.join('docs', 'behavior', old)
        new_rel = os.path.join('docs', 'behavior', folder, new)
        moves[old] = (folder, new, old_rel, new_rel)
    if not apply:
        return moves
    for old, (folder, new, old_rel, new_rel) in moves.items():
        os.makedirs(os.path.join(BEHAVIOR, folder), exist_ok=True)
        src = os.path.join(REPO, old_rel)
        dst = os.path.join(REPO, new_rel)
        if is_tracked(old_rel) and not os.path.exists(dst):
            subprocess.run(['git', 'mv', old_rel, new_rel], cwd=REPO, check=True)
        else:
            os.replace(src, dst)
    return moves


def rewrite_links(moved, apply):
    """Re-relativize links pointing at moved files from anywhere in the repo."""
    oldabs_to_new = {
        os.path.join(REPO, old_rel): os.path.join(REPO, new_rel)
        for _, _, old_rel, new_rel in moved.values()
    }
    newabs_to_old = {v: k for k, v in oldabs_to_new.items()}
    changed = 0
    for base, dirs, files in os.walk(REPO):
        if os.sep + '.git' in base + os.sep:
            continue
        if any(part in ('build', 'build-host', 'artifacts', 'local-assets',
                        'third_party', 'romfs') for part in base.split(os.sep)):
            continue
        for fn in files:
            if not fn.endswith('.md'):
                continue
            path = os.path.join(base, fn)
            cur_abs = os.path.abspath(path)
            old_abs = newabs_to_old.get(cur_abs, cur_abs)
            old_dir = os.path.dirname(old_abs)
            new_dir = os.path.dirname(cur_abs)
            try:
                text = open(path, encoding='utf-8').read()
            except (OSError, UnicodeDecodeError):
                continue

            def repl(m):
                nonlocal changed
                raw = m.group(1)
                if raw.startswith(('http://', 'https://', 'mailto:', '#')):
                    return m.group(0)
                target, sep, anchor = raw.partition('#')
                if not target:
                    return m.group(0)
                tgt_old = os.path.normpath(os.path.join(old_dir, target.split('?')[0]))
                if tgt_old in oldabs_to_new:
                    tgt_new = oldabs_to_new[tgt_old]
                elif old_dir != new_dir and os.path.exists(tgt_old):
                    tgt_new = tgt_old
                else:
                    return m.group(0)
                rel = os.path.relpath(tgt_new, new_dir)
                new_link = rel + (sep + anchor if sep else '')
                if new_link == raw:
                    return m.group(0)
                changed += 1
                return '](' + new_link + ')'

            new_text = LINK.sub(repl, text)
            if new_text != text and apply:
                open(path, 'w', encoding='utf-8').write(new_text)
    return changed


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--apply', action='store_true',
                        help='move files and rewrite links (default: dry run)')
    parser.add_argument('--list-folders', action='store_true',
                        help='print the subsystem folder taxonomy and exit')
    args = parser.parse_args()

    if args.list_folders:
        for name, desc in FOLDERS:
            print(f'{name:10s} {desc}')
        return

    moves = move_flat_files(args.apply)
    if not moves:
        print('docs/behavior: no flat files to normalize')
        return
    for old, (folder, new, old_rel, new_rel) in sorted(moves.items()):
        tag = '' if old == new else f'  (renamed to {new})'
        print(f'  {old} -> {folder}/{tag}')
    print(f'{"moved" if args.apply else "would move"} {len(moves)} file(s)')
    changed = rewrite_links(moves, args.apply)
    print(f'{"rewrote" if args.apply else "would rewrite"} {changed} link(s)')


if __name__ == '__main__':
    sys.exit(main())
