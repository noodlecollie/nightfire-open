#pragma once

#include <cstdint>
#include "Common/Math.h"
#include "Common/Container.h"

namespace SMDv10
{
	struct Node
	{
		int8_t boneIndex = -1;
		std::string name;
		int8_t parent = -1;

		Node() = default;

		Node(int8_t inBoneIndex, const std::string& inName, int8_t inParent) :
			boneIndex(inBoneIndex),
			name(inName),
			parent(inParent)
		{
		}
	};

	struct NodeFrame
	{
		int8_t boneIndex = -1;
		Vec3D position;
		Vec3D rotation;

		NodeFrame() = default;

		NodeFrame(int8_t inBoneIndex, const Vec3D& inPosition, const Vec3D& inRotation) :
			boneIndex(inBoneIndex),
			position(inPosition),
			rotation(inRotation)
		{
		}
	};

	struct Frame
	{
		int32_t value = 0;
		Container<NodeFrame> bones;

		Frame() = default;

		Frame(int32_t inValue, Container<NodeFrame> inBones) :
			value(inValue),
			bones(std::move(inBones))
		{
		}
	};

	struct Vertex
	{
		int8_t bone = 0;
		Vec3D position;
		Vec3D normal;
		float texU = 0.0f;
		float texV = 0.0f;

		Vertex() = default;

		Vertex(int8_t inBone, const Vec3D& inPosition, const Vec3D& inNormal, float inTexU, float inTexV) :
			bone(inBone),
			position(inPosition),
			normal(inNormal),
			texU(inTexU),
			texV(inTexV)
		{
		}
	};

	struct Triangle
	{
		static constexpr size_t NUM_VERTICES = 3;

		std::string texture;
		Vertex vertices[NUM_VERTICES] = {};

		Triangle() = default;

		Triangle(std::string inTexture, Vertex v0, Vertex v1, Vertex v2)
		{
			texture = std::move(inTexture);
			vertices[0] = std::move(v0);
			vertices[1] = std::move(v1);
			vertices[2] = std::move(v2);
		}

		void Flip()
		{
			Vertex temp = vertices[0];
			vertices[0] = vertices[2];
			vertices[2] = temp;
		}
	};
}  // namespace SMDv10
