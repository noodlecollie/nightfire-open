#pragma once

#include <memory>
#include <unordered_map>
#include "MDLv14/MDLFile.h"
#include "SMDv10/SMDFile.h"

namespace SMDv10
{
	class SMDAnimationPopulator
	{
	public:
		SMDAnimationPopulator(
			const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
			const std::string& sequenceName);

		std::shared_ptr<SMDFile> Populate();

	private:
		std::shared_ptr<MDLv14::MDLFile> m_MDLFile;
		std::string m_SequenceName;
	};
}  // namespace SMDv10
