#!/usr/bin/env python3
"""Build original submenu products in empty scratch, then publish build outputs."""
import argparse
import os
from pathlib import Path
import tempfile
from battle_submenu_surface_assets import build
from iso_assets import DEFAULT_ISO

PRODUCTS = tuple("battle-submenu-surfaces." + ext for ext in ("bin", "t3x", "json"))

def compile_assets(iso=DEFAULT_ISO, output="romfs"):
    output = Path(output)
    # The original importer always sees new empty paths. Existing shared
    # extraction caches and editable originals are never conversion inputs.
    with tempfile.TemporaryDirectory(prefix="pm-submenu-") as directory:
        scratch = Path(directory)
        metadata = build(iso, scratch / "products", scratch / "cache")
        data = {name: (scratch / "products" / name).read_bytes() for name in PRODUCTS}
        output.mkdir(parents=True, exist_ok=True)
        for name, payload in data.items():
            destination = output / name
            if destination.is_file() and destination.read_bytes() == payload:
                continue
            temporary = None
            try:
                with tempfile.NamedTemporaryFile(prefix=name + ".", dir=output, delete=False) as stream:
                    temporary = Path(stream.name)
                    stream.write(payload)
                os.replace(temporary, destination)
            finally:
                if temporary is not None:
                    temporary.unlink(missing_ok=True)
        return metadata

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iso", default=DEFAULT_ISO)
    parser.add_argument("--output", default="romfs")
    arguments = parser.parse_args()
    compile_assets(arguments.iso, arguments.output)
