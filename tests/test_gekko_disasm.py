"""Synthetic instruction vectors and listing preservation for the Gekko decoder."""
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from gekko_disasm import correct_disassembly, decode_paired


def aform(xo, d=3, a=4, b=5, c=6, rc=0):
    return 4 << 26 | d << 21 | a << 16 | b << 11 | c << 6 | xo << 1 | rc


def xform(xo, d=3, a=4, b=5, rc=0):
    return 4 << 26 | d << 21 | a << 16 | b << 11 | xo << 1 | rc


class GekkoDecoderTests(unittest.TestCase):
    def test_all_four_operand_arithmetic_and_record_suffix(self):
        for xo, name in [(10, 'ps_sum0'), (11, 'ps_sum1'), (14, 'ps_madds0'),
                         (15, 'ps_madds1'), (23, 'ps_sel'), (28, 'ps_msub'),
                         (29, 'ps_madd'), (30, 'ps_nmsub'), (31, 'ps_nmadd')]:
            for rc in (0, 1):
                with self.subTest(name=name, rc=rc):
                    self.assertEqual(decode_paired(aform(xo, rc=rc)),
                                     f"{name}{'.' if rc else ''} f3, f4, f6, f5")

    def test_binary_multiply_and_estimates(self):
        for xo, name in [(18, 'ps_div'), (20, 'ps_sub'), (21, 'ps_add')]:
            self.assertEqual(decode_paired(aform(xo, c=0)), f'{name} f3, f4, f5')
        for xo, name in [(12, 'ps_muls0'), (13, 'ps_muls1'), (25, 'ps_mul')]:
            self.assertEqual(decode_paired(aform(xo, b=0)), f'{name} f3, f4, f6')
        for xo, name in [(24, 'ps_res'), (26, 'ps_rsqrte')]:
            self.assertEqual(decode_paired(aform(xo, a=0, c=0)), f'{name} f3, f5')

    def test_moves_merges_compares_and_cache_operation(self):
        for xo, name in [(40, 'ps_neg'), (72, 'ps_mr'), (136, 'ps_nabs'), (264, 'ps_abs')]:
            self.assertEqual(decode_paired(xform(xo, a=0, rc=1)), f'{name}. f3, f5')
        for xo, name in [(528, 'ps_merge00'), (560, 'ps_merge01'),
                         (592, 'ps_merge10'), (624, 'ps_merge11')]:
            self.assertEqual(decode_paired(xform(xo)), f'{name} f3, f4, f5')
        for xo, name in [(0, 'ps_cmpu0'), (32, 'ps_cmpo0'), (64, 'ps_cmpu1'), (96, 'ps_cmpo1')]:
            self.assertEqual(decode_paired(xform(xo, d=7 << 2)), f'{name} cr7, f4, f5')
        self.assertEqual(decode_paired(xform(1014, d=0)), 'dcbz_l r4, r5')

    def test_quantized_displacements_w_i_and_update(self):
        for major, name in [(56, 'psq_l'), (57, 'psq_lu'), (60, 'psq_st'), (61, 'psq_stu')]:
            for disp in (-2048, -1, 0, 2047):
                for w in (0, 1):
                    for i in range(8):
                        word = major << 26 | 31 << 21 | 1 << 16 | w << 15 | i << 12 | disp & 0xfff
                        self.assertEqual(decode_paired(word), f'{name} f31, {disp}(r1), {w}, {i}')

    def test_indexed_quantized_fields_do_not_become_arithmetic(self):
        for xo, name in [(6, 'psq_lx'), (7, 'psq_stx'), (38, 'psq_lux'), (39, 'psq_stux')]:
            for w in (0, 1):
                for i in range(8):
                    word = xform(xo) | w << 10 | i << 7
                    self.assertEqual(decode_paired(word), f'{name} f3, r4, r5, {w}, {i}')

    def test_invalid_encodings_never_fall_back_to_newer_powerpc(self):
        for word in [aform(16), aform(18), aform(12), aform(24),
                     xform(40), xform(0), xform(1014), xform(6, rc=1),
                     xform(38, a=0), 57 << 26]:
            self.assertEqual(decode_paired(word), f'.long 0x{word:08x}')
        self.assertIsNone(decode_paired(0x4e800020))
        self.assertIsNone(decode_paired(0xc0230000))

    def test_fixed_word_regressions_from_mislabelled_formats(self):
        # Distinct hard-coded vectors make operand ordering/bit widths independently visible.
        self.assertEqual(decode_paired(0x11860018), 'ps_muls0 f12, f6, f0')
        self.assertEqual(decode_paired(0x1188601e), 'ps_madds1 f12, f8, f0, f12')
        self.assertEqual(decode_paired(0x10a12956), 'ps_sum1 f5, f1, f5, f5')
        self.assertEqual(decode_paired(0xf3e180b8), 'psq_st f31, 184(r1), 1, 0')
        self.assertEqual(decode_paired(0xe3e18fb8), 'psq_l f31, -72(r1), 1, 0')

    def test_preserves_addresses_bytes_annotations_and_other_instructions(self):
        text = ('header\r\n'
                '8029dd8c: 11 86 00 18  \tvextdubvlx 12, 6, 0, 0 # authored note\r\n'
                '8029dd90:\t4e800020 \tblr\r\n'
                '8029dd94: f0030004\t<unknown> ; keep annotation\r\n')
        corrected, stats = correct_disassembly(text)
        self.assertEqual(corrected,
                         'header\r\n'
                         '8029dd8c: 11 86 00 18  \tps_muls0 f12, f6, f0 # authored note\r\n'
                         '8029dd90:\t4e800020 \tblr\r\n'
                         '8029dd94: f0030004\tpsq_st f0, 4(r3), 0, 0 ; keep annotation\r\n')
        self.assertEqual(stats, dict(instructions=3, paired=2, changed=2, unrecognized=0))
        self.assertEqual(correct_disassembly(corrected)[1]['changed'], 0)

    def test_missing_final_newline_and_empty_listing(self):
        text = '80000000: 10 00 00 20   vsx_wrong'
        fixed, stats = correct_disassembly(text)
        self.assertEqual(fixed, '80000000: 10 00 00 20   .long 0x10000020')
        self.assertFalse(fixed.endswith('\n'))
        self.assertEqual(stats['unrecognized'], 1)
        self.assertEqual(correct_disassembly('')[0], '')

    def test_cli_check_is_read_only_and_repair_is_idempotent(self):
        tool = Path(__file__).resolve().parents[1] / 'tools/gekko_disasm.py'
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / 'fixture.disasm'
            original = b'fixture.elf: file format elf32-powerpc\n80000000: f0 03 00 04   xsaddsp 0, 35, 0\n'
            path.write_bytes(original)
            cmd = [sys.executable, str(tool)]
            self.assertEqual(subprocess.run(cmd + ['--check', str(path)], capture_output=True).returncode, 1)
            self.assertEqual(path.read_bytes(), original)
            self.assertEqual(subprocess.run(cmd + ['--in-place', temp], capture_output=True).returncode, 0)
            self.assertIn(b'psq_st f0, 4(r3), 0, 0', path.read_bytes())
            self.assertEqual(subprocess.run(cmd + ['--check', temp], capture_output=True).returncode, 0)
            binary = Path(temp) / 'input.elf'
            binary.write_bytes(b'\x7fELF\0')
            self.assertNotEqual(subprocess.run(cmd + ['--in-place', str(binary)], capture_output=True).returncode, 0)
            self.assertEqual(binary.read_bytes(), b'\x7fELF\0')
