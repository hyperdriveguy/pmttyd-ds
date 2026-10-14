"""Compare all fixed native request tables to independently decoded US bodies."""
import json
from pathlib import Path
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[1]

class OriginalFixedTev(unittest.TestCase):
    def test_all_fixed_request_tables(self):
        executable = ROOT / 'build-host/map_material_tev_tests'
        if not executable.exists():
            self.skipTest('Build host targets first (make test does this)')
        evidence = json.loads((ROOT / 'tools/map_tev_fixed_evidence.json').read_text())
        for program in evidence['literal_programs']:
            with self.subTest(mode=program['mode']):
                actual = json.loads(subprocess.check_output([str(executable), '--fixed', str(program['mode'])], cwd=ROOT))
                expected = [dict(function=r['target'], arguments=r['args'] + [0]*(7-len(r['args']))) for r in program['decoded']['requests']]
                self.assertEqual(actual[:-3], expected)
                self.assertEqual([r['arguments'][0] for r in actual[-3:]], [1, program['decoded']['stage_count'], 0])
