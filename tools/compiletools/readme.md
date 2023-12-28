Nightfire Open Compile Tools
============================

OK, let's face it: this code is all but unmaintainable. Therefore there are two options, as far as I can see:

1. Properly refactor all the code so that features toggled by `#define`s in `cmdlib.h` are decoupled from one another, and so that the super-long functions and global lists exist as more easily understandable classes.
2. Work on getting the code to do what we need it to do for NF and nothing more, dismantling anything non-essential that gets in the way.

I'm going for option 2, because option 1 would require more time and effort than I'm willing to dedicate to this repo.

This means that you shouldn't necessarily assume that features can be compiled in and out by changing `cmdlib.h` any more. We simply don't need the flexibility for that - we just need something that will work with the Xash3D engine. Any required flexibility will be built in manually at a later date, and I will be removing `#define`d features we don't need to make development on new features less complicated.

*StudioMDL sources are from https://github.com/fnky/studiomdl*

Eventually, this code should be ported to use the https://github.com/SNMetamorph/PrimeXT codebase.
