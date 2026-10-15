#!/usr/bin/env python3
"""Stage exact unchanged original US BFN for retained source font setup."""
import argparse
import hashlib
import json
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory

SOURCE_PATH = "f/papermarioset_US.bfn"
SOURCE_SIZE = 66560
SOURCE_SHA256 = "bb9cf410e5d916a1268ff503fc4f1ad7e8630b381cd781958eb0be32abb56b45"
PRODUCT = "papermarioset_US.bfn"

def export(iso, output):
    iso, output = Path(iso), Path(output)
    with iso.open("rb") as source:
        if source.read(8) != b"G8ME01\0\0":
            raise ValueError("Original US revision-0 disc required")
    files = inventory(iso)
    if files["disc_id"] != "G8ME01":
        raise ValueError("Original US disc required")
    matches = [entry for entry in files["files"] if entry["path"] == SOURCE_PATH]
    if len(matches) != 1 or matches[0]["size"] != SOURCE_SIZE:
        raise ValueError("Original US BFN extent changed")
    entry = matches[0]
    with iso.open("rb") as source:
        source.seek(entry["offset"])
        raw = source.read(entry["size"])
    if len(raw) != SOURCE_SIZE or hashlib.sha256(raw).hexdigest() != SOURCE_SHA256:
        raise ValueError("Original US BFN identity changed")
    metadata = dict(format="original-unmodified-BFN", source_path=SOURCE_PATH,
                    source_offset=entry["offset"], source_size=SOURCE_SIZE,
                    sha256=SOURCE_SHA256, product=PRODUCT,
                    converter_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                    conversion="None: exact whole original file; mutable width patches occur only in retained source setup",
                    license="Original Nintendo/Intelligent Systems asset; no new license or redistribution rights")
    if output.exists() and any(output.iterdir()):
        manifest = output / "source-font.json"
        product = output / PRODUCT
        if (manifest.is_file() and product.is_file()
                and json.loads(manifest.read_text()) == metadata
                and product.read_bytes() == raw
                and {p.name for p in output.iterdir()} == {PRODUCT, manifest.name}):
            return metadata
        raise ValueError("New original import requires an empty destination; refusing to overwrite existing edits")
    output.mkdir(parents=True, exist_ok=True)
    (output / PRODUCT).write_bytes(raw)
    (output / "source-font.json").write_text(json.dumps(metadata, indent=2) + "\n")
    return metadata

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iso", default=DEFAULT_ISO)
    parser.add_argument("--output", default="romfs/source-font")
    args = parser.parse_args()
    print(json.dumps(export(args.iso, args.output), indent=2))
