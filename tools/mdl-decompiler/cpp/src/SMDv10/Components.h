#pragma once

#include <cstdint>
#include "Common/Math.h"
#include "Common/Container.h"

namespace SMDv10
{
	struct Node
	{
		int8_t index = 0;
		std::string name;
		int8_t parent = -1;

		Node() = default;

		Node(int8_t inIndex, const std::string& inName, int8_t inParent) :
			index(inIndex),
			name(inName),
			parent(inParent)
		{
		}
	};

	struct NodeFrame
	{
		size_t node = 0;
		Vec3D position;
		Vec3D rotation;

		NodeFrame() = default;

		NodeFrame(size_t inNode, const Vec3D& inPosition, const Vec3D& inRotation) :
			node(inNode),
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

		Vertex vertices[NUM_VERTICES] = {};

		Triangle() = default;

		Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
		{
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
		}

		void Flip()
		{
			Vertex temp = vertices[0];
			vertices[0] = vertices[2];
			vertices[2] = temp;
		}
	};
}  // namespace SMDv10
