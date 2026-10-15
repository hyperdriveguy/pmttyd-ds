#!/usr/bin/env python3
"""Stage original window panel from an empty private export before editable assets."""
import argparse
import shutil
import tempfile
from pathlib import Path
from battle_window_panel_assets import build
from iso_assets import DEFAULT_ISO


def stage(iso,output):
    output=Path(output)
    # Source exporter enforces original whole-file and pixel identities. A fresh
    # temporary destination prevents re-importing over any editable source.
    with tempfile.TemporaryDirectory(prefix='pm-window-panel-') as directory:
        build(iso,directory)
        output.mkdir(parents=True,exist_ok=True)
        for suffix in ('t3x','json'):
            name='battle-window-panel.'+suffix
            shutil.copyfile(Path(directory)/name,output/name)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();stage(a.iso,a.output)
