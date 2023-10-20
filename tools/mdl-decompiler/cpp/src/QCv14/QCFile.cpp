#include "QCv14/QCFile.h"
#include "QCv14/CommandWriter.h"

namespace QCv14
{
	void QCFile::SetModelName(const ModelName& modelName)
	{
		m_ModelName = modelName;
	}

	void QCFile::Write(std::ostream& stream) const
	{
		CommandWriter writer;

		writer.WriteCommand(stream, m_ModelName);
	}
}  // namespace QCv14
