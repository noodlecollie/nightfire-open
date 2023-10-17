#include "MDLv14/MDLFile.h"
#include "BufferedFileReader.h"
#include "MDLv14/ComponentReader.h"

namespace MDLv14
{
	MDLFile::MDLFile(const BufferedFile& file)
	{
		BufferedFileReader fileReader = file.CreateReader();
		MDLv14::ComponentReader componentReader;

		m_Header = componentReader.ReadComponent<MDLv14::Header>(fileReader);
		m_Bones =
			componentReader.ReadComponentArray<MDLv14::Bone>(fileReader, m_Header.bones.count, m_Header.bones.offset);
	}
}  // namespace MDLv14
