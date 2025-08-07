#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>

class CBotUserInputSimulator
{
public:
	// The assumed max number of times a key can be cycled per second in our key press model:
	static constexpr float KEY_PRESS_RELEASE_CYCLES_PER_SECOND = 7.0f;

	static constexpr float PRESS_DURATION = 1.0f / (KEY_PRESS_RELEASE_CYCLES_PER_SECOND * 2.0f);
	static constexpr float INFINITE_DURATION = std::numeric_limits<float>::max();

	CBotUserInputSimulator();

	void Think();

	int GetButtons() const;
	void SetButtons(int flags, float pressDuration);

	inline void PressButton(int flags)
	{
		SetButtons(flags, PRESS_DURATION);
	}

	inline void HoldButton(int flags)
	{
		SetButtons(flags, INFINITE_DURATION);
	}

	inline void ReleaseButton(int flags)
	{
		SetButtons(flags, 0);
	}

private:
	static constexpr size_t MAX_FLAGS = 8 * sizeof(int);

	void SetIndividualButton(uint32_t index, float pressDuration);
	void UpdateCurrentButtons();

	int m_iCurrentButtons;
	float m_flButtonReleaseTimes[MAX_FLAGS];
};
