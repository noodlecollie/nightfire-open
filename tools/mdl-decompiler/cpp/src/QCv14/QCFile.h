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
		void SetClipToTextures(const QCClipToTextures& clipToTextures);
		void SetExternalTextures(const QCExternalTextures& externalTextures);
		void SetRoot(const QCRoot root);

		const Container<QCAttachment>& GetAttachments() const;
		void ClearAttachments();
		void AddAttachment(const QCAttachment& attachment);

		const Container<QCBodyGroup>& GetBodyGroups() const;
		void ClearBodyGroups();
		void AddBodyGroup(const QCBodyGroup& bodyGroup);

		const Container<QCBoneController>& GetControllers() const;
		void ClearControllers();
		void AddController(const QCBoneController& controller);

		const Container<QCHitBox>& GetHitBoxes() const;
		void ClearHitBoxes();
		void AddHitBox(const QCHitBox& hitbox);

		void Write(std::ostream& stream) const;

	private:
		QCCD m_CD;
		QCCDTexture m_CDTexture;
		QCClipToTextures m_ClipToTextures;
		QCExternalTextures m_ExternalTextures;
		QCModelName m_ModelName;
		QCBBox m_BBox;
		QCCBox m_CBox;
		QCRoot m_Root;
		Container<QCAttachment> m_Attachments;
		Container<QCBodyGroup> m_BodyGroups;
		Container<QCBoneController> m_BoneControllers;
		Container<QCHitBox> m_HitBoxes;
	};
}
