#pragma once

namespace CommonTypes
{
#define COMMONTYPES_MOTION_FLAG_LIST \
		LIST_ITEM(MotionFlag_Invalid, -1) \
		LIST_ITEM(MotionFlag_None, 0) \
		LIST_ITEM(MotionFlag_X, 0x00000001) /*Movement on X*/ \
		LIST_ITEM(MotionFlag_Y, 0x00000002) /*Movement on Y*/ \
		LIST_ITEM(MotionFlag_Z, 0x00000004) /*Movement on Z*/ \
		LIST_ITEM(MotionFlag_XR, 0x00000008) /*Rotation on X*/ \
		LIST_ITEM(MotionFlag_YR, 0x00000010) /*Rotatin on Y*/ \
		LIST_ITEM(MotionFlag_ZR, 0x00000020) /*Rotation on Z*/ \
		LIST_ITEM(MotionFlag_LX, 0x00000040) /*Looping movement on X?*/ \
		LIST_ITEM(MotionFlag_LY, 0x00000080) /*Looping movement on Y?*/ \
		LIST_ITEM(MotionFlag_LZ, 0x00000100) /*Looping movement on Z?*/ \
		LIST_ITEM(MotionFlag_AX, 0x00000200) /*??? movement on X*/ \
		LIST_ITEM(MotionFlag_AY, 0x00000400) /*??? movement on Y*/ \
		LIST_ITEM(MotionFlag_AZ, 0x00000800) /*??? movement on Z*/ \
		LIST_ITEM(MotionFlag_AXR, 0x00001000) /*??? rotation on X*/ \
		LIST_ITEM(MotionFlag_AYR, 0x00002000) /*??? rotation on Y*/ \
		LIST_ITEM(MotionFlag_AZR, 0x00004000) /*??? rotation on Z*/ \
		/*These are a guess based on some funky values (eg. in w_ronin.mdl)*/ \
		LIST_ITEM(MotionFlag_LXR, 0x00008000 | MotionFlag_XR) \
		LIST_ITEM(MotionFlag_LYR, 0x00008000 | MotionFlag_YR) \
		LIST_ITEM(MotionFlag_LZR, 0x00008000 | MotionFlag_ZR)

	enum MotionFlag
	{
#define LIST_ITEM(constant, value) constant = value,
		COMMONTYPES_MOTION_FLAG_LIST
#undef LIST_ITEM
	};
}
