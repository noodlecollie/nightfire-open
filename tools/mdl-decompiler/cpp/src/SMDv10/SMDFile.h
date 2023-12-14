#pragma once

#include <ostream>
#include "Common/Container.h"
#include "SMDv10/Components.h"

namespace SMDv10
{
	class SMDFile
	{
	public:
		enum class Type
		{
			Reference,
			Animation
		};

		SMDFile(Type smdType);

		void AddTriangle(const Triangle& triangle);
		void AddNode(const Node& node);
		void AddFrame(const Frame& frame);

		void Write(std::ostream& stream) const;

	private:
		Type m_Type = Type::Reference;
		Container<Triangle> m_Triangles;
		Container<Node> m_Nodes;
		Container<Frame> m_Frames;
	};
}  // namespace SMDv10
