#pragma once

namespace WeaponAtts
{
	struct CrosshairParameters;
}

namespace CrosshairCvars
{
	void Init();

	bool SpreadVisualisationEnabled();
	bool CrosshairOverrideEnabled();
	void PopulateCrosshairParametersFromDebugCvars(WeaponAtts::CrosshairParameters& params);

	int AttackModeForSpreadVisualisation();
	float RadiusMin();
	float RadiusMax();
	float BarLengthMin();
	float BarLengthMax();
}  // namespace CrosshairCvars
