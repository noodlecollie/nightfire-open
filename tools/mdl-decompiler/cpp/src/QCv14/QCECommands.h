#pragma once

#include <cstdint>

namespace QCv14
{
	enum class QCEGame
	{
		HalfLife = 2
	};

	struct QCEVersion
	{
		QCEGame game = QCEGame::HalfLife;
		int32_t version = 0;
		int32_t versionMajor = 0;
		int32_t versionMinor = 0;

		QCEVersion() = default;

		QCEVersion(QCEGame inGame, int32_t inVersion, int32_t inVersionMajor, int32_t inVersionMinor) :
			game(inGame),
			version(inVersion),
			versionMajor(inVersionMajor),
			versionMinor(inVersionMinor)
		{
		}
	};
}  // namespace QCv14
