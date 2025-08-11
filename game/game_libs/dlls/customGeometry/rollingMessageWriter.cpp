#include "customGeometry/rollingMessageWriter.h"
#include "customGeometry/messageWriter.h"

namespace CustomGeometry
{
	CRollingMessageWriter::CRollingMessageWriter(Category category) :
		m_Category(category)
	{
		WriteClearMessage();
	}

	CRollingMessageWriter::~CRollingMessageWriter()
	{
		Finalise();
	}

	void CRollingMessageWriter::BeginGeometry(DrawType drawType, uint32_t colour, float scale, float lifetime)
	{
		Finalise();
		CreateGeometryItem(drawType, colour, scale, lifetime);
	}

	void CRollingMessageWriter::Finalise()
	{
		if ( !m_CurrentGeometry )
		{
			return;
		}

		WriteGeometryMessage();
		m_CurrentGeometry.reset();
	}

	bool CRollingMessageWriter::AddLine(const Vector& p0, const Vector& p1)
	{
		ASSERT(m_CurrentGeometry.get() && m_CurrentGeometry->GetDrawType() == DrawType::Lines);

		if ( !m_CurrentGeometry )
		{
			return false;
		}

		EnsureCanAdd(2, 2);
		return m_CurrentGeometry->AddLine(p0, p1);
	}

	void CRollingMessageWriter::WriteClearMessage()
	{
		CMessageWriter(m_Category).WriteClearMessage();
	}

	void CRollingMessageWriter::WriteGeometryMessage()
	{
		ASSERT(m_CurrentGeometry.get());

		if ( m_CurrentGeometry->GetPoints().Count() < 1 || m_CurrentGeometry->GetIndices().Count() < 1 )
		{
			m_CurrentGeometry.reset();
			return;
		}

		CMessageWriter(m_Category).WriteMessage(*m_CurrentGeometry);
	}

	void CRollingMessageWriter::CreateGeometryItem(DrawType drawType, uint32_t colour, float scale, float lifetime)
	{
		m_CurrentGeometry.reset(new CGeometryItem());

		m_CurrentGeometry->SetDrawType(drawType);
		m_CurrentGeometry->SetColour(colour);
		m_CurrentGeometry->SetScale(scale);
		m_CurrentGeometry->SetLifetimeSecs(lifetime);
	}

	void CRollingMessageWriter::EnsureCanAdd(size_t pointsToAdd, size_t indicesToAdd)
	{
		ASSERT(m_CurrentGeometry.get());

		if ( !CanAdd(pointsToAdd, indicesToAdd) )
		{
			WriteGeometryMessage();
			CreateGeometryItem(
				m_CurrentGeometry->GetDrawType(),
				m_CurrentGeometry->GetColour(),
				m_CurrentGeometry->GetScale(),
				m_CurrentGeometry->GetLifetimeSecs());
		}
	}

	bool CRollingMessageWriter::CanAdd(size_t pointsToAdd, size_t indicesToAdd) const
	{
		ASSERT(m_CurrentGeometry.get());

		const size_t newPointCount = static_cast<size_t>(m_CurrentGeometry->GetPoints().Count()) + pointsToAdd;
		const size_t newIndexCount = static_cast<size_t>(m_CurrentGeometry->GetIndices().Count()) + indicesToAdd;

		if ( newPointCount > MAX_POINTS_PER_MSG )
		{
			return false;
		}

		if ( newIndexCount > MAX_INDICES_PER_MSG )
		{
			return false;
		}

		if ( CMessageWriter::CalcRawMessageBytes(newPointCount, newIndexCount) > MAX_MSG_LENGTH )
		{
			return false;
		}

		return true;
	}

}  // namespace CustomGeometry
