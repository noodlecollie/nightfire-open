#pragma once

#include <ostream>
#include <vector>
#include "QCv14/QCCommands.h"
#include "Common/Container.h"

namespace QCv14
{
	class QCFile
	{
	public:
		void SetDirectory(const QCCD& dir);
		void SetTextureDirectory(const QCCDTexture& textureDir);
		void SetModelName(const QCModelName& modelName);
		void SetBBox(const QCBBox& bbox);
		void SetCBox(const QCCBox& cbox);

		const Container<QCAttachment> GetAttachments() const;
		void ClearAttachments();
		void AddAttachment(const QCAttachment& attachment);

		const Container<QCBodyGroup> GetBodyGroups() const;
		void ClearBodyGroups();
		void AddBodyGroup(const QCBodyGroup& bodyGroup);

		void Write(std::ostream& stream) const;

	private:
		QCCD m_CD;
		QCCDTexture m_CDTexture;
		QCModelName m_ModelName;
		Container<QCAttachment> m_Attachments;
		QCBBox m_BBox;
		QCCBox m_CBox;
		Container<QCBodyGroup> m_BodyGroups;
	};
}
