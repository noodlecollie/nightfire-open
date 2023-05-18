#include "baseMessageWriter.h"
#include "standard_includes.h"

namespace Messages
{
	CBaseMessageWriter::CBaseMessageWriter(const char* name, int id) :
		m_MessageName(name),
		m_iMessageId(id)
	{
		ASSERT(name);
		ASSERT(id != -1);
	}

	CBasePlayer* CBaseMessageWriter::GetTargetClient() const
	{
		return m_pTargetClient;
	}

	void CBaseMessageWriter::SetTargetClient(CBasePlayer* client)
	{
		m_pTargetClient = client;
	}

	void CBaseMessageWriter::ClearTargetClient()
	{
		m_pTargetClient = nullptr;
	}

	const Vector* CBaseMessageWriter::GetOrigin() const
	{
		return m_pMsgOrigin ? &m_vecOrigin : nullptr;
	}

	void CBaseMessageWriter::SetOrigin(const Vector& origin)
	{
		m_vecOrigin = origin;
		m_pMsgOrigin = static_cast<const float*>(m_vecOrigin);
	}

	void CBaseMessageWriter::ClearOrigin()
	{
		m_vecOrigin = Vector();
		m_pMsgOrigin = nullptr;
	}

	bool CBaseMessageWriter::BeginMessage()
	{
		if ( !CanWriteMessage() )
		{
			return false;
		}

		MESSAGE_BEGIN(
			m_pTargetClient ? MSG_ONE : MSG_ALL,
			m_iMessageId,
			m_pMsgOrigin,
			m_pTargetClient ? m_pTargetClient->pev : nullptr);
		return true;
	}

	void CBaseMessageWriter::EndMessage()
	{
		MESSAGE_END();
	}

	bool CBaseMessageWriter::CanWriteMessage() const
	{
		if ( m_iMessageId == 0 )
		{
			ALERT(at_warning, "Attempted to send '%s' message before registration!\n", m_MessageName);
			return false;
		}

		return true;
	}
}  // namespace Messages
