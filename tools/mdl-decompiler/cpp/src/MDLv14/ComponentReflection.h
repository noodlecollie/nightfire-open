#pragma once

#include "Traits/Reflection.h"
#include "MDLv14/Components.h"

namespace Reflection
{
	template<>
	struct Traits<MDLv14::TypeFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::TypeFlag";
	};

	template<>
	struct Traits<MDLv14::LevelOfDetailFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::LevelOfDetailFlag";
	};

	template<>
	struct Traits<MDLv14::MotionFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::MotionFlag";
	};

	template<>
	struct Traits<MDLv14::SequenceFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::SequenceFlag";
	};

	template<>
	struct Traits<MDLv14::NodeFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::NodeFlag";
	};

	template<>
	struct Traits<MDLv14::CountOffsetPair>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::CountOffsetPair";
	};

	template<>
	struct Traits<MDLv14::BoundingBox>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BoundingBox";
	};

	template<>
	struct Traits<MDLv14::EyePosition>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::EyePosition";
	};

	template<>
	struct Traits<MDLv14::Header>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Header";
	};

	template<>
	struct Traits<MDLv14::Bone>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Bone";
	};

	template<>
	struct Traits<MDLv14::BoneController>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BoneController";
	};

	template<>
	struct Traits<MDLv14::HitBox>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::HitBox";
	};

	template<>
	struct Traits<MDLv14::Event>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Event";
	};

	template<>
	struct Traits<MDLv14::Pivot>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Pivot";
	};

	template<>
	struct Traits<MDLv14::Sequence>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Sequence";
	};

	template<>
	struct Traits<MDLv14::AnimationValue>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::AnimationValue";
	};

	template<>
	struct Traits<MDLv14::SequenceGroup>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::SequenceGroup";
	};

	template<>
	struct Traits<MDLv14::LevelOfDetail>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::LevelOfDetail";
	};

	template<>
	struct Traits<MDLv14::Texture>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Texture";
	};

	template<>
	struct Traits<MDLv14::BodyGroup>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BodyGroup";
	};

	template<>
	struct Traits<MDLv14::Attachment>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Attachment";
	};

	template<>
	struct Traits<MDLv14::Sounds>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Sounds";
	};

	template<>
	struct Traits<MDLv14::SoundGroup>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::SoundGroup";
	};

	template<>
	struct Traits<MDLv14::TriangleMap>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::TriangleMap";
	};

	template<>
	struct Traits<MDLv14::Vertex>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Vertex";
	};

	template<>
	struct Traits<MDLv14::Normal>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Normal";
	};

	template<>
	struct Traits<MDLv14::TextureCoOrdinate>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::TextureCoOrdinate";
	};

	template<>
	struct Traits<MDLv14::BlendingScales>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BlendingScales";
	};

	template<>
	struct Traits<MDLv14::Blending>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Blending";
	};

	template<>
	struct Traits<MDLv14::BoneFixUp>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BoneFixUp";
	};

	template<>
	struct Traits<MDLv14::Mesh>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Mesh";
	};

	template<>
	struct Traits<MDLv14::ModelInfo>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::ModelInfo";
	};

	template<>
	struct Traits<MDLv14::Model>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Model";
	};
}  // namespace Reflection
