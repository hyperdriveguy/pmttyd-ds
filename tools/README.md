# Tool map

Tools are grouped by filename prefix. Most are read-only inspectors, asset
converters, or source verifiers; generated packs and reports belong under the
ignored `local-assets/`, `romfs/`, or `artifacts/` paths described in
[`docs/ASSETS.md`](../docs/ASSETS.md).

| Prefix or name | Purpose |
| --- | --- |
| `verify_*.py` | Contract/source/asset verifiers. Run a focused verifier named by the relevant behavior document. |
| `*_assets.py`, `build_assets.py` | ISO-backed converters and native pack builders. They read the ISO without modifying it. |
| `audio_*`, `musyx_*`, `sfx_*`, `switch_audio.py` | Audio extraction, inventory, conversion and mapping. |
| `battle_*`, `event_*`, `field_*`, `opening_*`, `message_*`, `camera_*` | Subsystem assets, evidence and fixture preparation. |
| `iso_assets.py`, `behavior_evidence.py`, `gekko_disasm.py`, `inspect.py` | Read-only ISO inventory and binary/source research. |
| `emulator_smoke.py`, `make_*checkpoint.py`, `make_*fixture.py` | Isolated emulator fixtures and capture helpers. |
| `check_doc_links.py` | Repository Markdown link check. |

Find related utilities with `rg --files tools | rg '/(battle|event|field|opening)_|^tools/(battle|event|field|opening)_'`.
For a specific output, search its basename and converter in the asset ledger
rather than editing generated data directly.
