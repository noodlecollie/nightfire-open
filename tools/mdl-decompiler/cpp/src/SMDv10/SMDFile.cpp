#include "SMDv10/SMDFile.h"

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
}
