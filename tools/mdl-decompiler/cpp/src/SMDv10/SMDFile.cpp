#include "SMDv10/SMDFile.h"

namespace SMDv10
{
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
}
