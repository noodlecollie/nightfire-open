#pragma once

#include "MDLv14/Components.h"
#include "QCv10/QCCommands.h"

namespace Conversion
{
	const char* ActivityName(MDLv14::Activity activity);
	const char* ActivityName(QCv10::Activity activity);

	MDLv14::Activity ConvertActivity_IntToV14(int32_t activity);
	QCv10::Activity ConvertActivity_V14ToV10(MDLv14::Activity activity);
}  // namespace Conversion
