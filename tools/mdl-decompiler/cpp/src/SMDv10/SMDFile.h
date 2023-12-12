#pragma once

#include "Common/Container.h"
#include "SMDv10/Components.h"

namespace SMDv10
{
	class SMDFile
	{
	public:
		void AddTriangle(const Triangle& triangle);
		void AddNode(const Node& node);
		void AddFrame(const Frame& frame);

	private:
		Container<Triangle> m_Triangles;
		Container<Node> m_Nodes;
		Container<Frame> m_Frames;
	};
}  // namespace SMDv10
