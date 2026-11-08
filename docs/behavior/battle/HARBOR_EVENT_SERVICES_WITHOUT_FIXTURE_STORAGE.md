# Harbor event services without fixture storage

`HarborTutorialPacked::registerEventServices()` configures the existing native
sound, movement, jump, takeoff and camera callbacks and binds the original event
function addresses. It performs no guest mapping, guest stores, source heap
initialization or actor construction. Call it once on a fresh registry before
any event is executed. A duplicate bind returns false, following the registry's
existing native policy; configuration is not a source gameplay invocation.

The legacy `provision()` retains its existing fixed-storage allocation and
initialization order, then delegates the unchanged service-registration tail to
this method. Existing fixture callers therefore receive the same setup. A real
source startup must select existing process storage and pool adoption before
calling these services; it must not run `provision()` and then try to adopt.

This separation grants no readiness or source completion. Actual BattleInit,
UnitSetup/Entry, shared VM, source map producer, physical input, source camera,
resource ownership and retained Main must still supply their genuine owners.
Callbacks continue to borrow the same Packed object and VM; those owners must
outlive all registry consumers. Sound callbacks retain original requests, not
proof of audible playback.

The focused test confirms that registration leaves the BattleWork global,
legacy B/U/P spans, map globals and arena unmapped. Original GetHp/GetMaxHp
callbacks then read an independently admitted relocated registry unit, without
creating legacy aliases. This is an explicit host memory fixture, not original
battle startup. Strict host, ARM compilation and ASan/UBSan of the modified TU
and test passed against the matching frozen13438 core. That archive is not
instrumented; leak checking is disabled in the ptrace sandbox. Full consistent
host/native gates remain required before integration or commit.
