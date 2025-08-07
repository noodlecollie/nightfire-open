#pragma once

#include "utlstring.h"

namespace Messages
{
	class CBaseMessageReader
	{
	public:
		bool ReadMessage(void* buffer, int size);

		bool ReadSuccessfully() const;
		CUtlString ErrorString() const;

	protected:
		virtual bool ReadMessageInternal(void* buffer, int size) = 0;
		virtual void ResetInternalData();

		void SetErrorString(const CUtlString& str);

	private:
		bool m_ReadSuccessfully = false;
		CUtlString m_ErrorString;
	};
}  // namespace Messages
