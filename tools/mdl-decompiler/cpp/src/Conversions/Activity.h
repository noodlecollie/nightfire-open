#pragma once

#include "MDLv14/Components.h"
#include "QCv10/QCCommands.h"

const char* ActivityName(MDLv14::Activity activity);
const char* ActivityName(QCv10::Activity activity);

bool ConvertActivity_IntToV14(int32_t inActivity, MDLv14::Activity& outActivity);
bool ConvertActivity_V14ToV10(MDLv14::Activity inActivity, QCv10::Activity& outActivity);
