/*
viscompress.h - For compressing and decompressing RLE-encoded vis data.
Copyright (C) 2017 NoodleCollie

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include <stddef.h>
#include "BuildPlatform/Typedefs.h"

// Functions return false if a truncation occurred, or the operation was generally unsuccessful.

qboolean
VisCompress(const byte* inUncompressed, size_t inSize, byte* outCompressed, size_t outMaxSize, size_t* outSize);
qboolean
VisDecompress(const byte* inCompressed, size_t inSize, byte* outUncompressed, size_t outMaxSize, size_t* outSize);

// For when the input size is unknown (as is the case in a model_t).
// The number of uncompressed bytes is known, given the number of leaves that there are.
// The input data is continually processed until the output buffer is compltetely filled.
qboolean VisDecompressToKnownSize(const byte* inCompressed, byte* outUncompressed, size_t outMaxSize);

// inSize must be a multiple of rowSize.
qboolean VisCompressAllByRow(
	const byte* inUncompressed,
	size_t inSize,
	size_t rowSize,
	byte* outCompressed,
	size_t outMaxSize,
	size_t* outSize);
qboolean VisDecompressAllByRow(
	const byte* inUncompressed,
	size_t inSize,
	size_t rowSize,
	byte* outCompressed,
	size_t outMaxSize,
	size_t* outSize);

size_t VisUncompressedRowBytesRequired(size_t numLeaves);
