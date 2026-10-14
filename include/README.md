# Public header map

Headers are grouped by filename prefix to match [`source/README.md`](../source/README.md)
and implementation/test searches. Host-testable contracts belong in the
portable headers; libctru, Citro3D, NDSP and SD ownership stays in platform
adapters. See [`docs/ARCHITECTURE.md`](../docs/ARCHITECTURE.md) for runtime
composition and [`docs/behavior/README.md`](../docs/behavior/README.md) for the
subsystem contract index.

```sh
rg --files include source tests | rg '/(battle|event|field)_'
rg -n 'TypeName' include source tests
```
