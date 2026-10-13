import importlib.util,tempfile,unittest
from pathlib import Path
spec=importlib.util.spec_from_file_location('emulator_smoke',Path(__file__).resolve().parents[1]/'tools/emulator_smoke.py')
smoke=importlib.util.module_from_spec(spec);spec.loader.exec_module(smoke)
class NativeExceptionTests(unittest.TestCase):
 def test_missing_log_and_fresh_profile_errors_are_not_cpu_exceptions(self):
  with tempfile.TemporaryDirectory() as name:
   smoke.check_native_exception(name)
   log=Path(name)/'data/azahar-emu/log/azahar_log.txt';log.parent.mkdir(parents=True);log.write_text('Service.FS <Error> File not found\n')
   smoke.check_native_exception(name)
 def test_exception_dialog_fails_capture_even_while_emulator_is_alive(self):
  with tempfile.TemporaryDirectory() as name:
   log=Path(name)/'data/azahar-emu/log/azahar_log.txt';log.parent.mkdir(parents=True)
   log.write_text('startup\n'*10000+'[117] Core <Critical> core/arm/exception_handler.cpp:LogException:140:\nException Type: Undefined Instruction\n')
   with self.assertRaisesRegex(RuntimeError,'Native application exception'):smoke.check_native_exception(name)
if __name__=='__main__':unittest.main()
