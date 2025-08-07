#include <stdexcept>
#include <iomanip>
#include "SMDv10/SMDFileWriter.h"
#include "SMDv10/SMDFile.h"
#include "Common/StreamHelpers.h"

namespace SMDv10
{
	class SMDFile;

	SMDFileWriter::SMDFileWriter(const std::shared_ptr<SMDFile>& smdFile) :
		m_SMDFile(smdFile)
	{
		if ( !m_SMDFile )
		{
			throw std::invalid_argument("Expected valid SMD file to write");
		}
	}

	void SMDFileWriter::Write(std::ostream& stream)
	{
		stream << "version 1" << std::endl;
		stream << "nodes" << std::endl;

		for ( const Node& node : m_SMDFile->GetNodes() )
		{
			WriteNode(stream, node);
		}

		stream << "end" << std::endl;
		stream << "skeleton" << std::endl;

		for ( const Frame& frame : m_SMDFile->GetFrames() )
		{
			WriteFrame(stream, frame);
		}

		stream << "end" << std::endl;
		if ( m_SMDFile->GetType() == SMDFile::Type::Reference )
		{
			stream << "triangles" << std::endl;

			for ( const Triangle& triangle : m_SMDFile->GetTriangles() )
			{
				stream << triangle.texture << ".png" << std::endl;

				for ( const SMDv10::Vertex& vertex : triangle.vertices )
				{
					WriteVertex(stream, vertex);
				}
			}

			stream << "end" << std::endl;
		}
	}

	void SMDFileWriter::WriteVertex(std::ostream& stream, const Vertex& vertex)
	{
		stream << std::setw(3) << static_cast<int>(vertex.bone) << std::setw(0) << " ";
		WriteVec3D(stream, vertex.position);
		stream << " ";
		WriteVec3D(stream, vertex.normal);
		stream << " ";
		WriteFixedFloat(stream, vertex.texU);
		stream << " ";
		WriteFixedFloat(stream, vertex.texV);
		stream << std::endl;
	}

	void SMDFileWriter::WriteNode(std::ostream& stream, const Node& node)
	{
		stream << std::setw(3) << static_cast<int>(node.boneIndex) << std::setw(0) << " \"" << node.name << "\" "
			   << std::setw(3) << static_cast<int>(node.parent) << std::setw(0) << std::endl;
	}

	void SMDFileWriter::WriteFrame(std::ostream& stream, const Frame& frame)
	{
		stream << "time " << frame.value << std::endl;

		for ( const SMDv10::NodeFrame& entry : frame.bones )
		{
			stream << std::setw(3) << static_cast<int>(entry.boneIndex) << std::setw(0) << " ";
			WriteVec3D(stream, entry.position);
			stream << " ";
			WriteVec3D(stream, entry.rotation);
			stream << std::endl;
		}
	}
}  // namespace SMDv10
