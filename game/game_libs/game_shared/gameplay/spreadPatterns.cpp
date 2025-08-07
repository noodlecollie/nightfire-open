#include "gameplay/spreadPatterns.h"

static Vector2D GaussianSpread(const SpreadPatternArgs& args)
{
	// We make 4 calls to UTIL_SharedRandomFloat. For each shot that is fired, we want to generate 4
	// pseudo-random numbers from a consistent seed, so we increment the seed in a way that will
	// allow each call to UTIL_SharedRandomFloat return a different value.
	// There are 4 calls here because this is what the original Half Life code did:
	// https://github.com/ValveSoftware/halflife/blob/master/dlls/combat.cpp#L1524
	// I don't know why the two random numbers need to be added together instead of just
	// using one number, but I'll leave it as is.
	static constexpr uint32_t NUM_SHARED_CALLS = 4;

	float x = UTIL_SharedRandomFloat(
				  static_cast<uint32_t>(args.randomSeed) + (args.shotNumber * NUM_SHARED_CALLS) + 1,
				  -0.5,
				  0.5) +
		UTIL_SharedRandomFloat(
				  static_cast<uint32_t>(args.randomSeed) + (args.shotNumber * NUM_SHARED_CALLS) + 2,
				  -0.5,
				  0.5);

	float y = UTIL_SharedRandomFloat(
				  static_cast<uint32_t>(args.randomSeed) + (args.shotNumber * NUM_SHARED_CALLS) + 3,
				  -0.5,
				  0.5) +
		UTIL_SharedRandomFloat(
				  static_cast<uint32_t>(args.randomSeed) + (args.shotNumber * NUM_SHARED_CALLS) + 4,
				  -0.5,
				  0.5);

	return Vector2D(x * args.baseSpread.x, y * args.baseSpread.y);
}

static Vector2D SegmentedCircleSpread(const SpreadPatternArgs& args)
{
	static constexpr int NUM_SHARED_CALLS = 3;

	// We split the circle of potential spread into equal segments
	// based on the number of shots we have, so that each shot will
	// live somewhere within the segment. We then apply variance to
	// where that shot actually lands within the segment.
	// Calls to UTIL_SharedRandomFloat() provide the range as [0 1]
	// or [-1 1], as this gives a better random coefficient than
	// passing in the actual angle or spread, which may be tiny and
	// cause issues.

	// If we aren't firing multiple shots, just fall back to Gaussian
	// spread since that's better for single shots.
	if ( args.totalShots < 2 )
	{
		return GaussianSpread(args);
	}

	const float segmentAngle = (static_cast<float>(M_PI) * 2.0f) / static_cast<float>(args.totalShots);
	float shotAngle = static_cast<float>(args.shotNumber) * segmentAngle;

	// First of all, work out how much to vary the angle.
	float angleVariance = UTIL_SharedRandomFloat(
		static_cast<uint32_t>(args.randomSeed) + (args.shotNumber * NUM_SHARED_CALLS) + 1,
		-1.0f,
		1.0f);

	angleVariance *= segmentAngle / 2.0f;

	// Then, work out how much to vary the actual spread from 100% accuracy.
	// This is based on the Gaussian spread code.
	float spreadDist = UTIL_SharedRandomFloat(
						   static_cast<uint32_t>(args.randomSeed) + (args.shotNumber * NUM_SHARED_CALLS) + 2,
						   -0.5,
						   0.5) +
		UTIL_SharedRandomFloat(
						   static_cast<uint32_t>(args.randomSeed) + (args.shotNumber * NUM_SHARED_CALLS) + 3,
						   -0.5,
						   0.5);

	// The spread is divided by 2 here, since the base spread actually represents
	// the total number of radians either side of 100% accuracy, and we're only
	// dealing with deviation in one direction here.
	spreadDist *= args.baseSpread.Length() / 2.0f;

	return Vector2D(spreadDist * cosf(shotAngle + angleVariance), spreadDist * sinf(shotAngle + angleVariance));
}

Vector2D CalculateSpread(const SpreadPatternArgs& args)
{
	switch ( args.pattern )
	{
		case WeaponAtts::SpreadPattern::Gaussian:
		{
			return GaussianSpread(args);
		}

		case WeaponAtts::SpreadPattern::SegmentedCircle:
		{
			return SegmentedCircleSpread(args);
		}

		default:
		{
			ASSERTSZ(false, "Unrecognised spread pattern!");
			return Vector2D();
		}
	}
}
