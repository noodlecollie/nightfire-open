#include "SMDv10/SMDFile.h"

namespace SMDv10
{
	void SMDFile::AddTriangle(const Triangle& triangle)
	{
		m_Triangles.emplace_back(triangle);
	}
}
