#pragma once

#include <RmlUi/Core/Types.h>

enum class CameraOrbitMode
{
	None,
	DragYaw,
	DragPitchYaw
};

static inline bool ParseCameraOrbitModeString(const Rml::String& str, CameraOrbitMode& outMode)
{
	if ( str == "pitch-yaw" )
	{
		outMode = CameraOrbitMode::DragPitchYaw;
		return true;
	}
	else if ( str == "yaw" )
	{
		outMode = CameraOrbitMode::DragYaw;
		return true;
	}
	else if ( str == "none" )
	{
		outMode = CameraOrbitMode::None;
		return true;
	}

	return false;
}

static inline const char* CameraOrbitModeString(CameraOrbitMode mode)
{
	switch ( mode )
	{
		case CameraOrbitMode::DragPitchYaw:
		{
			return "pitch-yaw";
		}

		case CameraOrbitMode::DragYaw:
		{
			return "yaw";
		}

		default:
		{
			return "none";
		}
	}
}
