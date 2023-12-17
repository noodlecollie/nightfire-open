#include "SMDv10/SMDFile.h"
#include "SMDv10/ComponentReflection.h"
#include "Common/Math.h"

namespace SMDv10
{
	SMDFile::SMDFile(Type smdType) :
		m_Type(smdType)
	{
	}

	void SMDFile::AddTriangle(const Triangle& triangle)
	{
		m_Triangles.emplace_back(triangle);
	}

	void SMDFile::AddNode(const Node& node)
	{
		m_Nodes.emplace_back(node);
	}

	void SMDFile::AddFrame(const Frame& frame)
	{
		m_Frames.emplace_back(frame);
	}

	SMDFile::Type SMDFile::GetType() const
	{
		return m_Type;
	}

	const Container<Triangle>& SMDFile::GetTriangles() const
	{
		return m_Triangles;
	}

	const Container<Node>& SMDFile::GetNodes() const
	{
		return m_Nodes;
	}

	const Container<Frame>& SMDFile::GetFrames() const
	{
		return m_Frames;
	}

	void SMDFile::Rotate(float degrees)
	{
		const float radians = DegreesToRadians(degrees);

		for ( Frame& frame : m_Frames )
		{
			for ( NodeFrame& bone : frame.bones )
			{
				const int8_t boneIndex = bone.boneIndex;
				const int8_t parentBoneIndex = m_Nodes.GetElementChecked(boneIndex).parent;

				// Only manipulate root bones.
				if ( parentBoneIndex >= 0 )
				{
					continue;
				}

				bone.rotation.z += radians;

				const int xSign = bone.position.x < 0.0f ? -1 : (bone.position.x > 0.0f ? 1 : 0);

				const float sqrtOfXSquaredPlusYSquared = sqrtf(
					(bone.position.x * bone.position.x) + (bone.position.y * bone.position.y));

				const float ratioOfYOverXPlusNewRotation = atanf(bone.position.y / bone.position.x) + radians;

				bone.position.x = cosf(ratioOfYOverXPlusNewRotation) * sqrtOfXSquaredPlusYSquared;
				bone.position.y = sinf(ratioOfYOverXPlusNewRotation) * sqrtOfXSquaredPlusYSquared;

				if ( xSign == -1 )
				{
					bone.position.x *= -1.0f;
					bone.position.y *= -1.0f;
				}
			}
		}
	}
}  // namespace SMDv10
