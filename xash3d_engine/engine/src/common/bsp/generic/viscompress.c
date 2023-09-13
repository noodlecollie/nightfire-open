#include "common/bsp/generic/viscompress.h"

static qboolean ProcessRows(
	const qboolean compress,
	const byte* inUncompressed,
	size_t inSize,
	size_t rowSize,
	byte* outCompressed,
	size_t outMaxSize,
	size_t* outSize)
{
	size_t baseIndex;
	size_t accumulatedSize = 0;
	qboolean success = false;

	if ( !inUncompressed || inSize < 1 || !outCompressed || outMaxSize < 1 || inSize % rowSize != 0 )
	{
		if ( outSize )
		{
			*outSize = 0;
		}

		return false;
	}

	for ( baseIndex = 0; baseIndex < inSize && success; baseIndex += rowSize )
	{
		const byte* inRow = inUncompressed + baseIndex;
		size_t outRowSize = 0;

		success = compress
			? VisCompress(inRow, rowSize, outCompressed + accumulatedSize, outMaxSize - accumulatedSize, &outRowSize)
			: VisDecompress(inRow, rowSize, outCompressed + accumulatedSize, outMaxSize - accumulatedSize, &outRowSize);

		accumulatedSize += outRowSize;
	}

	if ( outSize )
	{
		*outSize = accumulatedSize;
	}

	return success;
}

qboolean VisCompress(const byte* inUncompressed, size_t inSize, byte* outCompressed, size_t outMaxSize, size_t* outSize)
{
	size_t inIndex;
	size_t outIndex = 0;
	qboolean truncated = false;

	if ( !inUncompressed || inSize < 1 || !outCompressed || outMaxSize < 1 )
	{
		if ( outSize )
		{
			*outSize = 0;
		}

		return false;
	}

	for ( inIndex = 0; inIndex < inSize && outIndex < outMaxSize;
		  ++inIndex, truncated = outIndex >= outMaxSize && inIndex < inSize )
	{
		byte run;
		byte inValue = inUncompressed[inIndex];
		outCompressed[outIndex++] = inValue;

		if ( outIndex >= outMaxSize )
		{
			// Continue instead of breaking, so the truncation bool is set.
			continue;
		}

		if ( inValue )
		{
			// Not a zero, continue to the next byte.
			continue;
		}

		// The byte was zero. RLE the run of zeroes.
		for ( run = 1; run < 255 && inIndex + run < inSize; ++run )
		{
			if ( inUncompressed[inIndex + run] )
			{
				break;
			}
		}

		outCompressed[outIndex++] = run;

		// inIndex is incremented by the loop automatically, so go one less here.
		inIndex += run - 1;
	}

	if ( outSize )
	{
		*outSize = outIndex;
	}

	return !truncated;
}

qboolean
VisDecompress(const byte* inCompressed, size_t inSize, byte* outUncompressed, size_t outMaxSize, size_t* outSize)
{
	size_t inIndex;
	size_t outIndex = 0;
	qboolean truncated = false;

	if ( !inCompressed || inSize < 1 || !outUncompressed || outMaxSize < 1 )
	{
		if ( outSize )
		{
			*outSize = 0;
		}

		return false;
	}

	for ( inIndex = 0; inIndex < inSize && outIndex < outMaxSize;
		  ++inIndex, truncated = outIndex >= outMaxSize && inIndex < inSize )
	{
		byte inByte = inCompressed[inIndex];
		byte run;
		if ( inByte )
		{
			outUncompressed[outIndex++] = inByte;
			continue;
		}

		// When a zero is encountered, the next byte specifies the length of the run.
		for ( run = inCompressed[++inIndex]; run > 0 && outIndex < outMaxSize; --run )
		{
			outUncompressed[outIndex++] = 0;
		}
	}

	if ( outSize )
	{
		*outSize = outIndex;
	}

	return !truncated;
}

qboolean VisDecompressToKnownSize(const byte* inCompressed, byte* outUncompressed, size_t outMaxSize)
{
	size_t inIndex;
	size_t outIndex = 0;

	if ( !inCompressed || !outUncompressed || outMaxSize < 1 )
	{
		return false;
	}

	for ( inIndex = 0; outIndex < outMaxSize; ++inIndex )
	{
		byte inByte = inCompressed[inIndex];
		byte run;
		if ( inByte )
		{
			outUncompressed[outIndex++] = inByte;
			continue;
		}

		// When a zero is encountered, the next byte specifies the length of the run.
		for ( run = inCompressed[++inIndex]; run > 0 && outIndex < outMaxSize; --run )
		{
			outUncompressed[outIndex++] = 0;
		}
	}

	return true;
}

qboolean VisCompressAllByRow(
	const byte* inUncompressed,
	size_t inSize,
	size_t rowSize,
	byte* outCompressed,
	size_t outMaxSize,
	size_t* outSize)
{
	return ProcessRows(true, inUncompressed, inSize, rowSize, outCompressed, outMaxSize, outSize);
}

qboolean VisDecompressAllByRow(
	const byte* inUncompressed,
	size_t inSize,
	size_t rowSize,
	byte* outCompressed,
	size_t outMaxSize,
	size_t* outSize)
{
	return ProcessRows(false, inUncompressed, inSize, rowSize, outCompressed, outMaxSize, outSize);
}

size_t VisUncompressedRowBytesRequired(size_t numLeaves)
{
	// Taken from existing code.
	return (numLeaves + 7) / 8;
}
