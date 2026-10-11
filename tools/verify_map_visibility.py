#!/usr/bin/env python3
"""Check original station road-entry group suppression against full conversion."""
from pathlib import Path
import tempfile
from map_convert import convert
ROOT=Path(__file__).resolve().parents[1]
def main():
    with tempfile.TemporaryDirectory(prefix='ttyd-visibility-') as work:
        folders=[Path(work)/name for name in ['full','road']]
        full=convert(ROOT/'local-assets/gor_04.d',folders[0])
        road=convert(ROOT/'local-assets/gor_04.d',folders[1],hidden=('gondora_2','hako_1'))
        assert full['triangles']==41398;assert road['triangles']==41084
        assert full['textures']==road['textures']
        for ext in ['walls','ground']:
            assert (folders[0]/('map.'+ext)).read_bytes()==(folders[1]/('map.'+ext)).read_bytes()
        # Both suppressed owners have no own mesh parts: the 314-triangle
        # difference therefore checks descendant suppression, not leaf hiding.
        print('Original road-entry station visibility: 314 subtree triangles omitted; collision and texture inventory unchanged')
if __name__=='__main__':main()
