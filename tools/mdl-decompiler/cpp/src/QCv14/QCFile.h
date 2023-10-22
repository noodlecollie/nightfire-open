#pragma once

#include <ostream>
#include <vector>
#include "QCv14/QCCommands.h"

namespace QCv14
{
	class QCFile
	{
	public:
		void SetModelName(const QCModelName& modelName);

		const std::vector<QCAttachment> GetAttachments() const;
		void ClearAttachments();
		void AddAttachment(const QCAttachment& attachment);

		void SetBBox(const QCBBox& bbox);

		void Write(std::ostream& stream) const;

	private:
		QCModelName m_ModelName;
		std::vector<QCAttachment> m_Attachments;
		QCBBox m_BBox;
	};
}
