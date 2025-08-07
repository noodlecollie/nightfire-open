#pragma once

namespace Messages
{
	template<typename T>
	class CMessageRegistrationHelper
	{
	public:
		static inline void RegisterUserMessage()
		{
			if ( IsUserMessageRegistered() )
			{
				return;
			}

			m_iStaticMessageId = REG_USER_MSG(T::MESSAGE_NAME, T::MESSAGE_SIZE);
		}

		static inline int StaticUserMessageId()
		{
			return m_iStaticMessageId;
		}

		static inline bool IsUserMessageRegistered()
		{
			return m_iStaticMessageId != 0;
		}

	private:
		static int m_iStaticMessageId;
	};
}  // namespace Messages

template<typename T>
int Messages::CMessageRegistrationHelper<T>::m_iStaticMessageId = 0;
