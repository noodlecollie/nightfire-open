#pragma once

#include <functional>
#include "MathLib/vec3.h"

class CBasePlayer;
typedef struct entvars_s entvars_t;

namespace Messages
{
	class CBaseMessageWriter
	{
	public:
		// By default, the message is sent to all clients.
		CBasePlayer* GetTargetClient() const;
		void SetTargetClient(CBasePlayer* client);
		void ClearTargetClient();

		// By default, the transmitted origin is null.
		const Vector* GetOrigin() const;
		void SetOrigin(const Vector& origin);
		void ClearOrigin();

	protected:
		CBaseMessageWriter(const char* name, int id);

		bool BeginMessage();
		void EndMessage();
		bool CanWriteMessage() const;

		// TODO: Expose a default virtual WriteMessage() function instead of this?
		inline void WriteMessageWrapper(const std::function<void(void)>& writeFunc)
		{
			if ( BeginMessage() )
			{
				writeFunc();
				EndMessage();
			}
		}

	private:
		const char* m_MessageName = nullptr;
		int m_iMessageId = 0;
		CBasePlayer* m_pTargetClient = nullptr;

		Vector m_vecOrigin;
		const float* m_pMsgOrigin = nullptr;
	};
}  // namespace Messages
