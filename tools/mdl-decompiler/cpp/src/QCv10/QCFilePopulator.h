#pragma once

#include <string>
#include <memory>
#include <vector>
#include "MDLv14/MDLFile.h"
#include "QCv10/QCFile.h"
#include "QCv10/QCEFile.h"

namespace SMDv10
{
	struct SMDName;
}

namespace QCv10
{
	class QCFilePopulator
	{
	public:
		QCFilePopulator(
			const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
			const std::shared_ptr<QCv10::QCFile>& qcFile,
			const std::shared_ptr<QCv10::QCEFile>& qceFile,
			const std::string& outputDir);

		void Populate();

		const std::vector<SMDv10::SMDName>& GetReferenceSMDNames() const;
		const std::vector<SMDv10::SMDName>& GetAnimationSMDNames() const;

	private:
		void BaseSetup();
		void PopulateBones();
		void PopulateAttachments();
		void PopulateBodyGroups();
		void PopulateBoneControllers();
		void PopulateHitBoxes();
		void PopulateSequences();
		void PopulateSkins();
		void ReadActivity(QCv10::QCSequence& qcSequence, size_t seqIndex);
		std::string RecordAnimationName(const std::string& baseName);
		std::string RecordAnimationName(const std::string& baseName, int32_t blendIndex);

		std::shared_ptr<MDLv14::MDLFile> m_MDLFile;
		std::shared_ptr<QCv10::QCFile> m_QCFile;
		std::shared_ptr<QCv10::QCEFile> m_QCEFile;
		std::string m_OutputDir;
		std::vector<SMDv10::SMDName> m_ReferenceSMDs;
		std::vector<SMDv10::SMDName> m_AnimationSMDs;
	};
}  // namespace QCv10
