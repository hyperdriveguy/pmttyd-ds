import struct
import tempfile
import unittest
from pathlib import Path
from opening_assets import cached_maps_complete


class OpeningCacheTests(unittest.TestCase):
    def test_missing_map_companion_invalidates_complete_cache(self):
        with tempfile.TemporaryDirectory() as directory:
            out=Path(directory)
            variants=['aaa_00','aaa_00-interior','muj_20','gor_10']
            for variant in variants:
                prefix='opening-'+variant
                for suffix in ['vertices','uvanim','meshanim']:
                    (out/(prefix+'.'+suffix)).write_bytes(b'local-test')
                (out/(prefix+'.batches')).write_bytes(struct.pack('<II',0,1))
                (out/(prefix+'-0.t3x')).write_bytes(b'local-test')
            self.assertTrue(cached_maps_complete(out))
            for suffix in ['batches','uvanim','meshanim','vertices']:
                file=out/('opening-muj_20.'+suffix)
                content=file.read_bytes();file.unlink()
                self.assertFalse(cached_maps_complete(out))
                file.write_bytes(content)
            texture=out/'opening-gor_10-0.t3x';texture.unlink()
            self.assertFalse(cached_maps_complete(out))
            texture.write_bytes(b'local-test')
            (out/'opening-muj_20.batches').write_bytes(b'truncated')
            self.assertFalse(cached_maps_complete(out))


if __name__=='__main__':unittest.main()
