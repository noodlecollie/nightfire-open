#include "baseMessageReader.h"

namespace Messages
{
	bool CBaseMessageReader::ReadMessage(void* buffer, int size)
	{
		ResetInternalData();
		m_ReadSuccessfully = false;

		if ( buffer && size > 0 )
		{
			if ( ReadMessageInternal(buffer, size) )
			{
				m_ReadSuccessfully = true;
			}
			else
			{
				ResetInternalData();
				m_ReadSuccessfully = false;
			}
		}

		return m_ReadSuccessfully;
	}

	bool CBaseMessageReader::ReadSuccessfully() const
	{
		return m_ReadSuccessfully;
	}

	CUtlString CBaseMessageReader::ErrorString() const
	{
		return m_ErrorString;
	}

	void CBaseMessageReader::SetErrorString(const CUtlString& str)
	{
		m_ErrorString = str;
	}

	void CBaseMessageReader::ResetInternalData()
	{
	}
}  // namespace Messages
