#pragma once

namespace CrosshairCvars
{
	void Init();

	bool SpreadVisualisationEnabled();
	bool CrosshairOverrideEnabled();

	int AttackModeForSpreadVisualisation();
	float RadiusMin();
	float RadiusMax();
	float BarLengthMin();
	float BarLengthMax();
}
