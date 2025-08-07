#pragma once

#include "Traits/Reflection.h"
#include "QCv10/QCCommands.h"

namespace Reflection
{
	template<>
	struct Traits<QCv10::QCModelName>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCModelName";
	};

	template<>
	struct Traits<QCv10::QCAttachment>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCAttachment";
	};

	template<>
	struct Traits<QCv10::QCBBox>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCBBox";
	};

	template<>
	struct Traits<QCv10::QCBodyGroupItem>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCBodyGroupItem";
	};

	template<>
	struct Traits<QCv10::QCBodyGroup>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCBodyGroup";
	};

	template<>
	struct Traits<QCv10::QCCBox>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCCBox";
	};

	template<>
	struct Traits<QCv10::QCCD>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCCD";
	};

	template<>
	struct Traits<QCv10::QCCDTexture>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCCDTexture";
	};

	template<>
	struct Traits<QCv10::QCClipToTextures>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCClipToTextures";
	};

	template<>
	struct Traits<QCv10::QCExternalTextures>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCExternalTextures";
	};

	template<>
	struct Traits<QCv10::QCRoot>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCRoot";
	};

	template<>
	struct Traits<QCv10::QCBoneController>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCBoneController";
	};

	template<>
	struct Traits<QCv10::QCHitBox>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCHitBox";
	};

	template<>
	struct Traits<QCv10::QCEyePosition>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCEyePosition";
	};

	template<>
	struct Traits<QCv10::QCPivot>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCPivot";
	};

	template<>
	struct Traits<QCv10::QCMirrorBone>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCMirrorBone";
	};

	template<>
	struct Traits<QCv10::QCRenameBone>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCRenameBone";
	};

	template<>
	struct Traits<QCv10::QCScale>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCScale";
	};

	template<>
	struct Traits<QCv10::QCTypeFlags>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCTypeFlags";
	};

	template<>
	struct Traits<QCv10::QCOrigin>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCOrigin";
	};

	template<>
	struct Traits<QCv10::QCTextureGroup>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCTextureGroup";
	};

	template<>
	struct Traits<QCv10::QCBody>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCBody";
	};

	template<>
	struct Traits<QCv10::QCOptionActivity>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCOptionActivity";
	};

	template<>
	struct Traits<QCv10::QCOptionBlend>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCOptionBlend";
	};

	template<>
	struct Traits<QCv10::QCOptionEvent>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCOptionEvent";
	};

	template<>
	struct Traits<QCv10::QCOptionTransition>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCOptionTransition";
	};

	template<>
	struct Traits<QCv10::QCOptionPivot>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCOptionPivot";
	};

	template<>
	struct Traits<QCv10::QCSequence>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCSequence";
	};
}  // namespace Reflection
