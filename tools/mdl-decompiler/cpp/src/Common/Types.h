#pragma once

namespace CommonTypes
{
#define COMMONTYPES_MOTION_FLAG_LIST \
		LIST_ITEM(MotionFlag_Invalid, -1) \
		LIST_ITEM(MotionFlag_None, 0) \
		LIST_ITEM(MotionFlag_X, 0x00000001) \
		LIST_ITEM(MotionFlag_Y, 0x00000002) \
		LIST_ITEM(MotionFlag_Z, 0x00000004) \
		LIST_ITEM(MotionFlag_XR, 0x00000008) \
		LIST_ITEM(MotionFlag_YR, 0x00000010) \
		LIST_ITEM(MotionFlag_ZR, 0x00000020) \
		LIST_ITEM(MotionFlag_LX, 0x00000040) \
		LIST_ITEM(MotionFlag_LY, 0x00000080) \
		LIST_ITEM(MotionFlag_LZ, 0x00000100) \
		LIST_ITEM(MotionFlag_AX, 0x00000200) \
		LIST_ITEM(MotionFlag_AY, 0x00000400) \
		LIST_ITEM(MotionFlag_AZ, 0x00000800) \
		LIST_ITEM(MotionFlag_AXR, 0x00001000) \
		LIST_ITEM(MotionFlag_AYR, 0x00002000) \
		LIST_ITEM(MotionFlag_AZR, 0x00004000)

	enum MotionFlag
	{
#define LIST_ITEM(constant, value) constant = value,
		COMMONTYPES_MOTION_FLAG_LIST
#undef LIST_ITEM
	};
}
