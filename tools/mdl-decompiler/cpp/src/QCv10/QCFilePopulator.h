#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "MDLv14/MDLFile.h"
#include "QCv10/QCFile.h"
#include "QCv10/QCEFile.h"

namespace QCv10
{
	class QCFilePopulator
	{
	public:
		using ModelNameMap = std::unordered_map<std::string, std::string>;

		QCFilePopulator(
			const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
			const std::shared_ptr<QCv10::QCFile>& qcFile,
			const std::shared_ptr<QCv10::QCEFile>& qceFile,
			const std::string& outputDir);

		void Populate();

		const ModelNameMap& GetReferencedModelNames() const;

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

		std::shared_ptr<MDLv14::MDLFile> m_MDLFile;
		std::shared_ptr<QCv10::QCFile> m_QCFile;
		std::shared_ptr<QCv10::QCEFile> m_QCEFile;
		std::string m_OutputDir;
		ModelNameMap m_ReferencedModelNames;
	};
}  // namespace QCv10
