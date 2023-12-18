#pragma once

#include "Traits/Reflection.h"
#include "MDLv14/Components.h"

namespace Reflection
{
	template<>
	struct Traits<MDLv14::TypeFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::TypeFlag";
		static constexpr size_t READ_SIZE = 4;
	};

	template<>
	struct Traits<MDLv14::LevelOfDetailFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::LevelOfDetailFlag";
		static constexpr size_t READ_SIZE = 4;
	};

	template<>
	struct Traits<CommonTypes::MotionFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "CommonTypes::MotionFlag";
		static constexpr size_t READ_SIZE = 4;
	};

	template<>
	struct Traits<MDLv14::SequenceFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::SequenceFlag";
		static constexpr size_t READ_SIZE = 4;
	};

	template<>
	struct Traits<MDLv14::NodeFlag>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::NodeFlag";
		static constexpr size_t READ_SIZE = 4;
	};

	template<>
	struct Traits<MDLv14::MDLVec3D>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::MDLVec3D";
		static constexpr size_t READ_SIZE = 3 * 4;
	};

	template<>
	struct Traits<MDLv14::CountOffsetPair>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::CountOffsetPair";
		static constexpr size_t READ_SIZE = 2 * 4;
	};

	template<>
	struct Traits<MDLv14::BoundingBox>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BoundingBox";
		static constexpr size_t READ_SIZE = 6 * 4;
	};

	template<>
	struct Traits<MDLv14::EyePosition>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::EyePosition";
		static constexpr size_t READ_SIZE = 3 * 4;
	};

	template<>
	struct Traits<MDLv14::Header>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Header";
		static constexpr size_t READ_SIZE =  //
			68 +  // strings
			(23 * 4) +  // ints
			(9 * 2 * 4) +  // count-offset pairs
			(15 * 4) +  // Vec3Ds
			(48 * 4);  // model offsets
	};

	template<>
	struct Traits<MDLv14::Bone>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Bone";
		static constexpr size_t READ_SIZE = 32 + (20 * 4);
	};

	template<>
	struct Traits<MDLv14::BoneController>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BoneController";
		static constexpr size_t READ_SIZE = 6 * 4;
	};

	template<>
	struct Traits<MDLv14::HitBox>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::HitBox";
		static constexpr size_t READ_SIZE = (2 * 4) + (6 * 4);
	};

	template<>
	struct Traits<MDLv14::Event>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Event";
		static constexpr size_t READ_SIZE = (3 * 4) + 64;
	};

	template<>
	struct Traits<MDLv14::Pivot>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Pivot";
		static constexpr size_t READ_SIZE = 5 * 4;
	};

	template<>
	struct Traits<MDLv14::Sequence>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Sequence";
		static constexpr size_t READ_SIZE =  //
			32 +  // strings
			(5 * 4) +  // floats
			(21 * 4) +  // ints
			(4 * 4) +  // count-offset pairs
			(9 * 4);  // Vec3Ds
	};

	template<>
	struct Traits<MDLv14::AnimationValue>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::AnimationValue";
		static constexpr size_t READ_SIZE = 2;
	};

	template<>
	struct Traits<MDLv14::SequenceGroup>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::SequenceGroup";
		static constexpr size_t READ_SIZE = 32 + 64 + (2 * 4);
	};

	template<>
	struct Traits<MDLv14::LevelOfDetail>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::LevelOfDetail";
		static constexpr size_t READ_SIZE = 5 * 4;
	};

	template<>
	struct Traits<MDLv14::Texture>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Texture";
		static constexpr size_t READ_SIZE = (2 * 64) + (2 * 4);
	};

	template<>
	struct Traits<MDLv14::BodyGroup>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BodyGroup";
		static constexpr size_t READ_SIZE = 64 + (3 * 4);
	};

	template<>
	struct Traits<MDLv14::Attachment>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Attachment";
		static constexpr size_t READ_SIZE = 32 + (2 * 4) + (12 * 4);
	};

	template<>
	struct Traits<MDLv14::Sounds>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Sounds";
		static constexpr size_t READ_SIZE = 4 + (10 * 64);
	};

	template<>
	struct Traits<MDLv14::SoundGroup>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::SoundGroup";
		static constexpr size_t READ_SIZE = 32 + 4;
	};

	template<>
	struct Traits<MDLv14::TriangleMap>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::TriangleMap";
		static constexpr size_t READ_SIZE = 2;
	};

	template<>
	struct Traits<MDLv14::Vertex>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Vertex";
		static constexpr size_t READ_SIZE = 4 * 4;
	};

	template<>
	struct Traits<MDLv14::Normal>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Normal";
		static constexpr size_t READ_SIZE = 4 * 4;
	};

	template<>
	struct Traits<MDLv14::TextureCoOrdinate>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::TextureCoOrdinate";
		static constexpr size_t READ_SIZE = 2 * 4;
	};

	template<>
	struct Traits<MDLv14::BlendingScales>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BlendingScales";
		static constexpr size_t READ_SIZE = 4 * 4;
	};

	template<>
	struct Traits<MDLv14::Blending>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Blending";
		static constexpr size_t READ_SIZE = 4;
	};

	template<>
	struct Traits<MDLv14::BoneFixUp>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::BoneFixUp";
		static constexpr size_t READ_SIZE = 12 * 4;
	};

	template<>
	struct Traits<MDLv14::Mesh>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Mesh";
		static constexpr size_t READ_SIZE = 24 + (4 * 2);
	};

	template<>
	struct Traits<MDLv14::ModelInfo>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::ModelInfo";
		static constexpr size_t READ_SIZE = 3 * 4;
	};

	template<>
	struct Traits<MDLv14::Model>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "MDLv14::Model";
		static constexpr size_t READ_SIZE = 32 + (25 * 4);
	};
}  // namespace Reflection
