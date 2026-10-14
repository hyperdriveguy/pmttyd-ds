import collections
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from iso_assets import DEFAULT_ISO, inventory
from map_convert import convert, batch_flags
from map_visibility import EVENT_GROUPS


class MapVisibilityTests(unittest.TestCase):
    def test_original_story_boundaries(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / 'visibility.cpp'
            source.write_text('''#include "map_visibility.hpp"
#include <cassert>
using ttyd::mapEventGroupVisible;
int main() {
 for(int s=2;s<=6;++s) {
  for(unsigned g:{1u,2u,3u,5u,6u,7u})assert(!mapEventGroupVisible(g,s));
  for(unsigned g:{0u,4u,8u})assert(mapEventGroupVisible(g,s));
 }
 for(int s=0;s<=405;++s) {
  assert(mapEventGroupVisible(2,s)==(s>=268 && s!=402 && s!=404));
  assert(mapEventGroupVisible(3,s)==(s>=172 && s<=229 && s!=217));
  assert(unsigned(mapEventGroupVisible(4,s))+mapEventGroupVisible(5,s)+mapEventGroupVisible(6,s)==1);
 }
 assert(!mapEventGroupVisible(7,23));assert(!mapEventGroupVisible(7,22,4));
 assert(mapEventGroupVisible(7,23,4));assert(!mapEventGroupVisible(7,23,2));
 assert(mapEventGroupVisible(13,23));assert(!mapEventGroupVisible(13,23,4));
 assert(mapEventGroupVisible(8,9));assert(!mapEventGroupVisible(8,10));
 assert(mapEventGroupVisible(4,163));assert(mapEventGroupVisible(5,164));
 assert(mapEventGroupVisible(5,338));assert(mapEventGroupVisible(6,339));
 assert(!mapEventGroupVisible(63,3));
 assert(!mapEventGroupVisible(9,3));assert(!mapEventGroupVisible(10,8));
 assert(mapEventGroupVisible(9,3,1));assert(!mapEventGroupVisible(10,3,1));
 assert(mapEventGroupVisible(10,8,2));assert(!mapEventGroupVisible(9,8,2));
 assert(mapEventGroupVisible(11,3));assert(!mapEventGroupVisible(11,3,1));
 assert(mapEventGroupVisible(12,8));assert(!mapEventGroupVisible(12,8,2));
}
'''.replace('#include <cassert>', '#include <cassert>\n#include <initializer_list>'))
            binary = Path(directory) / 'visibility'
            subprocess.run([shutil.which('c++') or 'g++', '-std=c++17', '-I', str(ROOT/'include'/'display'), str(source), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)

    def test_source_subtrees_partition_without_losing_geometry(self):
        if not Path(DEFAULT_ISO).is_file():
            self.skipTest('Local US ISO required')
        files = {entry['path']: entry for entry in inventory(DEFAULT_ISO)['files']}
        hidden = {'gor_01': {'S_ie_03_naka'},
                  'gor_00': set(EVENT_GROUPS['gor_00']),
                  'gor_02': {'S_ie_04_naka','S_ie_02_naka'},
                  'gor_03': {'S_kou_kusas02', 'S_kou_kusas03'}}
        def rows(path):
            raw = path.read_bytes()
            return collections.Counter(raw[i:i+36] for i in range(0, len(raw), 36))
        with tempfile.TemporaryDirectory() as directory, open(DEFAULT_ISO, 'rb') as disc:
            root = Path(directory)
            for room, groups in EVENT_GROUPS.items():
                entry = files[f'm/{room}/d']
                disc.seek(entry['offset'])
                model = root / (room+'.d')
                model.write_bytes(disc.read(entry['size']))
                original, tagged, prologue = [root/(room+suffix) for suffix in ('-all','-tagged','-prologue')]
                convert(model, original)
                convert(model, tagged, event_groups=groups)
                convert(model, prologue, hidden=hidden[room])
                self.assertEqual(rows(original/'map.vertices'), rows(tagged/'map.vertices'))
                raw = (tagged/'map.batches').read_bytes()
                vertices = (tagged/'map.vertices').read_bytes()
                kept = collections.Counter()
                seen = set()
                for start, count, texture, flags, wrap_s, wrap_t in struct.iter_unpack('<IIiIII', raw[8:]):
                    group = flags>>26
                    seen.add(group)
                    if group in {1,2,3,5,6,7,9,10}:continue
                    kept.update(vertices[i*36:(i+1)*36] for i in range(start,start+count))
                self.assertEqual(kept, rows(prologue/'map.vertices'), room)
                self.assertTrue(set(groups.values()).issubset(seen), room)
                for ext in ('walls','ground','head'):
                    self.assertEqual((original/f'map.{ext}').read_bytes(), (tagged/f'map.{ext}').read_bytes())

    def test_packed_fields_remain_independent(self):
        flags = batch_flags(2, 1024, 3, 8)
        self.assertEqual((flags & 255, (flags>>8)&65535, (flags>>24)&3, flags>>26), (2,1024,3,8))
        with self.assertRaises(ValueError):batch_flags(0,0,0,64)
