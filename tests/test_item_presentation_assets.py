import json
from pathlib import Path
import struct
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from item_presentation_assets import COUNT, pack, source_rows
from iso_assets import DEFAULT_ISO

class ItemPresentationAssetsTests(unittest.TestCase):
    def test_null_and_empty_key_boundaries(self):
        rows=[(0,0,0,b'',b'')]*COUNT
        rows[0]=(0x80300000,0x80300010,307,b'key\0',b'\0')
        data=pack(rows)
        self.assertEqual(struct.unpack_from('<8s3I',data),(b'PMITEM01',340,0x803108a8,40))
        self.assertEqual(struct.unpack_from('<2I4H',data,20),(0x80300000,0x80300010,307,4,1,0))
        self.assertEqual(data[36:41],b'key\0\0')
        for name in (b'',b'key',b'k\0ey\0',b'x'*256+b'\0'):
            rows[0]=(1,0,0,name,b'')
            with self.assertRaises(ValueError):pack(rows)
        rows[0]=(0,0,0,b'\0',b'')
        with self.assertRaises(ValueError):pack(rows)

    @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'original local ISO unavailable')
    def test_original_table_extent_and_known_quest_bindings(self):
        rows=source_rows(DEFAULT_ISO)
        self.assertEqual(len(rows),340)
        self.assertEqual(rows[95][2:],(307,b'in_irai_butsu\0',b'msg_irai_butsu\0'))
        self.assertEqual(rows[107][2:],(308,b'in_fighters_pants\0',b'msg_fighters_pants\0'))
        self.assertEqual(rows[339][0:2],(0,0))
        self.assertTrue(all(n and d for n,d,_,_,_ in rows[:339]))

    @unittest.skipUnless(Path('romfs/message-icons.json').is_file(),'local icon pack unavailable')
    def test_original_selector_art_survives_atlas_packing(self):
        from PIL import Image
        meta=json.loads(Path('romfs/message-icons.json').read_text())
        icons={icon['id']:icon for icon in meta['icons']}
        atlas=Image.open('local-assets/message-icons/atlas.png').convert('RGBA')
        for identifier,tpl in ((307,312),(308,313),(445,465),(446,466),(447,467)):
            icon=icons[identifier];self.assertEqual(icon['loop'],0);self.assertEqual(len(icon['frames']),1)
            frame=icon['frames'][0];self.assertEqual(frame['tpl'],tpl)
            x=round(frame['uv'][0]*atlas.width);y=round((1-frame['uv'][1])*atlas.height)
            original=Image.open(f'local-assets/message-icons/{tpl}.png').convert('RGBA')
            packed=atlas.crop((x,y,x+frame['width'],y+frame['height']))
            self.assertEqual(packed.size,original.size);self.assertEqual(packed.tobytes(),original.tobytes())

if __name__=='__main__':unittest.main()
