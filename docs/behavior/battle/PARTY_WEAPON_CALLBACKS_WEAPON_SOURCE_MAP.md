# Complete party callback source weapon order

Companion to [complete callback protocol](PARTY_WEAPON_CALLBACKS_COMPLETE_SHARED_PROTOCOL.md).
US G8ME01 revision0;[verifier](../../../tools/verify_all_party_weapon_callbacks.py)
pins all28 leading16-byte weapon heads and exact28 original name strings,
seven unit-definition+0xc0 fields/full key1 tables,startup,and all callback bodies.
These heads identify key/icon-leading fields,NOT complete weapon/action/event
semantics or runtime availability. Reuse original msg/US data and icon assets.

Row order0/1/2/3 corresponds to always/L>=0/L>=1/L>=2 using one captured signed
current-party level. It is source order,not alphabetical/localized menu sorting.

| Party | Row0 | Row1 | Row2 | Row3 |
| --- | --- | --- | --- | --- |
| Goombella | 80393ea8 | 80393f68 | 80394028 | 803940e8 |
| Koops | 8038f55c | 8038f61c | 8038f6dc | 8038f79c |
| Flurrie | 80378da0 | 80378e60 | 80378f20 | 80378fe0 |
| Yoshi | 8037ca64 | 8037cb24 | 8037cee4 | 8037cfa4 |
| Vivian | 8038bee0 | 8038bfa0 | 8038c060 | 8038c120 |
| Bobbery | 80387c20 | 80387ce0 | 80387da0 | 80387e60 |
| Ms. Mowz | 80381cf0 | 80381f30 | 80381e70 | 80381ff0 |

Exact original name keys by the SAME row order:

| Party | Row0 key | Row1 key | Row2 key | Row3 key |
| --- | --- | --- | --- | --- |
| Goombella | `btl_wn_pkr_normal` | `btl_wn_pkr_lv1` | `btl_wn_pkr_lv2` | `btl_wn_pkr_lv3` |
| Koops | `btl_wn_pnk_normal` | `btl_wn_pnk_lv1` | `btl_wn_pnk_lv2` | `btl_wn_pnk_lv3_tsuranuki` |
| Flurrie | `btl_wn_pwd_normal` | `btl_wn_pwd_lv1` | `btl_wn_pwd_lv2` | `btl_wn_pwd_lv3` |
| Yoshi | `btl_wn_pys_normal` | `btl_wn_pys_lv1` | `btl_wn_pys_lv2` | `btl_wn_pys_lv3` |
| Vivian | `btl_wn_ptr_normal` | `btl_wn_ptr_lv1` | `btl_wn_ptr_lv2` | `btl_wn_ptr_lv3` |
| Bobbery | `btl_wn_pbm_normal` | `btl_wn_pbm_lv1` | `btl_wn_pbm_lv2` | `btl_wn_pbm_lv3` |
| Ms. Mowz | `btl_wn_pcr_normal` | `btl_wn_pcr_steal` | `btl_wn_pcr_madowase` | `btl_wn_pcr_kiss` |

Ms. Mowz's row1 pointer is numerically ABOVE row2;Yoshi's row1→row2 gap is
larger than one192-byte weapon record. Do not generate these tables by assuming
four adjacent objects or sorting pointers. Exact base/delta instructions are
pinned in their original callback bodies. Koops first two icons are literal
344/345 despite name/pointer resolution;all other icon steps use the row's
current weapon+4 halfword as documented in the protocol.

[Seven source key1 chains](PARTY_WEAPON_CALLBACK_KEY1_SOURCE_BINDINGS.md)
remain the source binding entry point. Its earlier statement that only Goombella
was complete describes that original delivery;the new all-callback contract
closes the six formerly open local bodies. Runtime unit+18 tables can change,
so source-definition evidence is not a transitive runtime ownership proof.

Implement callable registrations,original table/order/address translation,and
literal key reuse with original controls/text. Do not paraphrase dialogue or
invent party-specific attack events to fill unrecovered source graphs. Event/
weapon behavior,unit provisioning,query reachability,Game/platform/original
runtime and full-game requirements remain separate pending requests.
