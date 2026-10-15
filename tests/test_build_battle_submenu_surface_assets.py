import sys
from pathlib import Path
import tempfile
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from build_battle_submenu_surface_assets import compile_assets, PRODUCTS
from iso_assets import DEFAULT_ISO

class SubmenuBuildPublication(unittest.TestCase):
    def test_repeated_actual_conversion_preserves_other_outputs_and_unchanged_products(self):
        if not Path(DEFAULT_ISO).is_file():
            self.skipTest("Original read-only US ISO unavailable")
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / "romfs"
            output.mkdir()
            (output / "other.bin").write_bytes(b"unrelated build output")
            first = compile_assets(output=output)
            products = {name: ((output / name).read_bytes(), (output / name).stat().st_mtime_ns) for name in PRODUCTS}
            self.assertEqual(compile_assets(output=output), first)
            self.assertEqual({name: ((output / name).read_bytes(), (output / name).stat().st_mtime_ns) for name in PRODUCTS}, products)
            self.assertEqual((output / "other.bin").read_bytes(), b"unrelated build output")
            (output / PRODUCTS[0]).write_bytes(b"interrupted compiled product")
            compile_assets(output=output)
            self.assertEqual((output / PRODUCTS[0]).read_bytes(), products[PRODUCTS[0]][0])
            self.assertEqual({p.name for p in output.iterdir()}, {*PRODUCTS, "other.bin"})

if __name__ == "__main__":
    unittest.main()
