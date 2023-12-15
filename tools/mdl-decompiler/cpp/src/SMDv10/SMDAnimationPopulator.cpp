#include <stdexcept>
#include "SMDv10/SMDAnimationPopulator.h"
#include "MDLv14/ComponentReflection.h"

namespace SMDv10
{
	SMDAnimationPopulator::SMDAnimationPopulator(
		const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
		const std::string& sequenceName) :
		m_MDLFile(mdlFile),
		m_SequenceName(sequenceName)
	{
		if ( !m_MDLFile )
		{
			throw std::invalid_argument("Expected a valid MDL file");
		}
	}

	std::shared_ptr<SMDFile> SMDAnimationPopulator::Populate()
	{
		// TODO
		return std::shared_ptr<SMDFile>();
	}
}  // namespace SMDv10
