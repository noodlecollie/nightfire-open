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
		void SetModelName(const QCModelName& modelName);
		void SetBBox(const QCBBox& bbox);

		const Container<QCAttachment> GetAttachments() const;
		void ClearAttachments();
		void AddAttachment(const QCAttachment& attachment);

		const Container<QCBodyGroup> GetBodyGroups() const;
		void ClearBodyGroups();
		void AddBodyGroup(const QCBodyGroup& bodyGroup);

		void Write(std::ostream& stream) const;

	private:
		QCModelName m_ModelName;
		Container<QCAttachment> m_Attachments;
		QCBBox m_BBox;
		Container<QCBodyGroup> m_BodyGroups;
	};
}
