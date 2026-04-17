#include "models/VideoModesModel.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "udll_int.h"
#include "UIDebug.h"

static constexpr const char* const NAME_VIDEO_MODES = "videoModes";
static constexpr const char* const PROP_LABEL = "label";
static constexpr const char* const PROP_MODE_INDEX = "modeIndex";

static bool ParseDimensions(const Rml::String& label, int& width, int& height)
{
	Rml::String::size_type xLoc = label.find('x');

	if ( xLoc == Rml::String::npos )
	{
		return false;
	}

	width = std::atoi(label.c_str());
	height = std::atoi(label.c_str() + xLoc + 1);

	ASSERT(width > 0);
	ASSERT(height > 0);

	return true;
}

void VideoModesModel::Populate()
{
	m_VidModes.clear();

	int modeIndex = 0;
	const char* modeDesc = nullptr;

	while ( (modeDesc = gEngfuncs.pfnGetModeString(modeIndex)) != nullptr )
	{
		Rml::String label(modeDesc);
		int width = 0;
		int height = 0;

		ParseDimensions(label, width, height);

		m_VidModes.push_back(Entry {std::move(label), modeIndex, width, height});
		++modeIndex;
	}

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(NAME_VIDEO_MODES);
	}
}

bool VideoModesModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	Rml::StructHandle<Entry> entryType = constructor.RegisterStruct<Entry>();

	if ( !entryType )
	{
		return false;
	}

	if ( !entryType.RegisterMember(PROP_LABEL, &Entry::label) ||
		 !entryType.RegisterMember(PROP_MODE_INDEX, &Entry::index) )
	{
		return false;
	}

	if ( !constructor.RegisterArray<std::vector<Entry>>() || !constructor.Bind(NAME_VIDEO_MODES, &m_VidModes) )
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
	return TOTAL_COLUMNS;
}

Rml::String VideoModesModel::DisplayString(size_t row, size_t column) const
{
	if ( row >= m_VidModes.size() )
	{
		return Rml::String();
	}

	switch ( column )
	{
		case LABEL:
		{
			return m_VidModes[row].label;
		}

		case MODE_INDEX:
		{
			return std::to_string(m_VidModes[row].index);
		}

		default:
		{
			break;
		}
	}

	return Rml::String();
}

int VideoModesModel::Width(size_t row) const
{
	return row < m_VidModes.size() ? m_VidModes[row].width : -1;
}

int VideoModesModel::Height(size_t row) const
{
	return row < m_VidModes.size() ? m_VidModes[row].height : -1;
}

int VideoModesModel::VideoMode(size_t row) const
{
	return row < m_VidModes.size() ? m_VidModes[row].index : -1;
}

bool VideoModesModel::RowForDimensions(int width, int height, size_t& outRow) const
{
	for ( size_t index = 0; index < m_VidModes.size(); ++index )
	{
		const Entry& entry = m_VidModes[index];

		if ( entry.width == width && entry.height == height )
		{
			outRow = index;
			return true;
		}
	}

	return false;
}
