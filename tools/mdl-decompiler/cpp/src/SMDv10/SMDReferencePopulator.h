#pragma once

#include <memory>
#include <unordered_map>
#include "MDLv14/MDLFile.h"
#include "SMDv10/SMDFile.h"

namespace SMDv10
{
	struct SMDName
	{
		std::string nameInMDL;
		std::string outputNameOnDisk;

		SMDName() = default;

		SMDName(std::string inNameInMDL, std::string inOutputNameOnDisk) :
			nameInMDL(std::move(inNameInMDL)),
			outputNameOnDisk(std::move(inOutputNameOnDisk))
		{
		}
	};

	class SMDReferencePopulator
	{
	public:
		SMDReferencePopulator(
			const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
			const std::string& modelName);

		std::shared_ptr<SMDFile> Populate();

	private:
		std::shared_ptr<MDLv14::MDLFile> m_MDLFile;
		std::string m_ModelName;
	};
}  // namespace SMDv10
