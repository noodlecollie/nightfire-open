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

	void SMDFile::Write(std::ostream& stream) const
	{
		// StreamWriter streamWriter = new StreamWriter(file);
		// streamWriter.WriteLine("version 1");
		// streamWriter.WriteLine("nodes");
		// foreach (Node node in this._NodeCollection)
		// 	streamWriter.WriteLine(node.ToString());
		// streamWriter.WriteLine("end");
		// streamWriter.Flush();
		// streamWriter.WriteLine("skeleton");
		// foreach (Frame time in this._TimeCollection)
		// 	streamWriter.WriteLine(time.ToString());
		// streamWriter.WriteLine("end");
		// streamWriter.Flush();
		// if (this._Reference)
		// {
		// 	streamWriter.WriteLine("triangles");
		// 	foreach (Triangle<VertexV10> triangle in this._TriangleCollection)
		// 	{
		// 	streamWriter.WriteLine(triangle.Texture + ".bmp");
		// 	foreach (VertexV10 vertex in triangle.Vertices)
		// 		streamWriter.WriteLine(vertex.ToString());
		// 	streamWriter.Flush();
		// 	}
		// 	streamWriter.WriteLine("end");
		// 	streamWriter.Flush();
		// }
		// streamWriter.Close();

		stream << "version 1" << std::endl;

		stream << "nodes" << std::endl;

		for ( const Node& node : m_Nodes )
		{
			// TODO
		}

		stream << "end" << std::endl;

		stream << "skeleton" << std::endl;

		for ( const Frame& frame : m_Frames )
		{
			// TODO
		}

		stream << "end" << std::endl;

		if ( m_Type == Type::Reference )
		{
			stream << "triangles" << std::endl;

			for ( const Triangle& triangle : m_Triangles )
			{
				// TODO
			}

			stream << "end" << std::endl;
		}
	}
}
