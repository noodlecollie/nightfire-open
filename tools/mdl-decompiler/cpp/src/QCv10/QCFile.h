#pragma once

#include <ostream>
#include <vector>
#include "QCv10/QCCommands.h"
#include "Common/Container.h"

namespace QCv10
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
		void SetEyePosition(const QCEyePosition& eyePosition);
		void SetPivot(const QCPivot& pivot);
		void SetMirrorBone(const QCMirrorBone& mirrorBone);
		void SetRenameBone(const QCRenameBone& renameBone);
		void SetScale(const QCScale& scale);
		void SetTypeFlags(const QCTypeFlags& typeFlags);
		void SetOrigin(const QCOrigin& origin);
		void SetTextureGroup(QCTextureGroup&& textureGroup);

		const Container<QCAttachment>& GetAttachments() const;
		void ClearAttachments();
		void AddAttachment(const QCAttachment& attachment);

		const Container<QCBodyGroup>& GetBodyGroups() const;
		void ClearBodyGroups();
		void AddBodyGroup(const QCBodyGroup& bodyGroup);

		const Container<QCBody>& GetBodies() const;
		void ClearBodies();
		void AddBody(const QCBody& body);

		const Container<QCBoneController>& GetControllers() const;
		void ClearControllers();
		void AddController(const QCBoneController& controller);

		const Container<QCHitBox>& GetHitBoxes() const;
		void ClearHitBoxes();
		void AddHitBox(const QCHitBox& hitbox);

		const Container<QCSequence>& GetSequences() const;
		void ClearSequences();
		void AddSequence(const QCSequence& sequence);

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
		QCPivot m_Pivot;
		QCScale m_Scale;
		QCOrigin m_Origin;
		QCTypeFlags m_TypeFlags;
		QCMirrorBone m_MirrorBone;
		QCRenameBone m_RenameBone;
		QCEyePosition m_EyePosition;
		QCTextureGroup m_TextureGroup;
		Container<QCAttachment> m_Attachments;
		Container<QCBodyGroup> m_BodyGroups;
		Container<QCBody> m_Bodies;
		Container<QCBoneController> m_BoneControllers;
		Container<QCHitBox> m_HitBoxes;
		Container<QCSequence> m_Sequences;
		std::vector<std::string> m_ReferencedModelNames;
	};
}
