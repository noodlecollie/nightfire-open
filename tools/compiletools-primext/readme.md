OK, so I got most of the way through integrating these tools, but then realised that double precision floating point numbers seem to be pretty critical, and I'd integrated the tools with the MathLib library which uses single-precision floats for the vectors.

I'm going to leave the code here as a reference, but not hook it into CMake. One potential way forward might be:

1. Update MathLib to expose two different vector types, for single precision and double precision.
2. Factor out several of the important features of the engine: the file system/search path stuff, and asset loading/saving.
3. Port the PrimeXT map compiler code, but re-write it to use the appropriate vectors, and delegate file system and asset operations to these other libraries.

This would be quite a long-winded thing to do, because it would essentially require re-writing the compile code from scratch, even if it is from a reference. For now, to add the compiler features I want, I'm just going to continue to use the compilers in the `compiletools` directory, even though they're horrendous and messy.
