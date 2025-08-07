#pragma once

#include <memory>
#include <ostream>

namespace SMDv10
{
	class SMDFile;
	struct Vertex;
	struct Node;
	struct Frame;

	class SMDFileWriter
	{
	public:
		SMDFileWriter(const std::shared_ptr<SMDFile>& smdFile);

		void Write(std::ostream& stream);

	private:
		void WriteVertex(std::ostream& stream, const Vertex& vertex);
		void WriteNode(std::ostream& stream, const Node& node);
		void WriteFrame(std::ostream& stream, const Frame& frame);

		std::shared_ptr<SMDFile> m_SMDFile;
	};
}
