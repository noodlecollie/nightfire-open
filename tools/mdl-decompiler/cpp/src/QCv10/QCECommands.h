#pragma once

#include <cstdint>

namespace QCv10
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

	struct QCEReplaceActivity
	{
		std::string sequenceName;
		std::string activity;

		QCEReplaceActivity() = default;

		QCEReplaceActivity(const std::string& inSequenceName, const std::string& inActivity) :
			sequenceName(inSequenceName),
			activity(inActivity)
		{
		}
	};
}  // namespace QCv10
