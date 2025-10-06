#pragma once

static constexpr float SURFACE_IMPACT_ATTENUATION = 1.2f;

// Stronger attenuation, because player hit sounds are also played
// locally on the attacker's side to make them more obvious.
static constexpr float BODY_IMPACT_ATTENUATION = 2.0f;

static constexpr float BODY_IMPACT_LOCAL_VOLUME = 0.7f;
static constexpr float RONIN_IMPACT_LOCAL_VOLUME = 0.35f;
