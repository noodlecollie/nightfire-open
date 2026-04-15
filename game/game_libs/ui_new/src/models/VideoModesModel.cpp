#include "models/VideoModesModel.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "udll_int.h"

static constexpr const char* const NAME_VIDEO_MODES = "videoModes";

void VideoModesModel::Populate()
{
	m_VidModes.clear();

	int modeIndex = 0;
	const char* modeDesc = nullptr;

	while ( (modeDesc = gEngfuncs.pfnGetModeString(modeIndex++)) != nullptr )
	{
		m_VidModes.push_back(Rml::String(modeDesc));
	}

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(NAME_VIDEO_MODES);
	}
}

bool VideoModesModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.RegisterArray<std::vector<Rml::String>>() || !constructor.Bind(NAME_VIDEO_MODES, &m_VidModes) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

size_t VideoModesModel::Rows() const
{
	return m_VidModes.size();
}

size_t VideoModesModel::Columns() const
{
	return 1;
}

Rml::String VideoModesModel::DisplayString(size_t row, size_t column) const
{
	if ( column == 0 && row < m_VidModes.size() )
	{
		return m_VidModes[row];
	}

	return Rml::String();
}
