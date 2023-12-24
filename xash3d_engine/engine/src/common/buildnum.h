#pragma once

// Returns days since Apr 1 2015
int Q_buildnum(void);

// Returns a Xash3D build number. This is left for compability with original Xash3D.
// IMPORTANT: this value must be changed ONLY after updating to newer Xash3D base
// IMPORTANT: this value must be acquired through "build" cvar.
int Q_buildnum_compat(void);
