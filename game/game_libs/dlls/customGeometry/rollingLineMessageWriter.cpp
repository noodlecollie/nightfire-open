#include "customGeometry/rollingLineMessageWriter.h"
#include "customGeometry/messageWriter.h"

namespace CustomGeometry
{
	CRollingLineMessageWriter::CRollingLineMessageWriter(Category category) :
		m_Category(category)
	{
		WriteClearMessage();
	}

	CRollingLineMessageWriter::~CRollingLineMessageWriter()
	{
		Finalise();
	}

	void CRollingLineMessageWriter::BeginGeometry(uint32_t colour, float scale, float lifetime)
	{
		Finalise();
		CreateGeometryItem(colour, scale, lifetime);
	}

	void CRollingLineMessageWriter::Finalise()
	{
		if ( !m_CurrentGeometry )
		{
			return;
		}

		WriteGeometryMessage();
		m_CurrentGeometry.reset();
	}

	bool CRollingLineMessageWriter::AddLine(const Vector& p0, const Vector& p1)
	{
		ASSERT(m_CurrentGeometry.get() && m_CurrentGeometry->GetDrawType() == DrawType::Lines);

		if ( !m_CurrentGeometry )
		{
			return false;
		}

		if ( ExtendsCurrentLine(p0, p1) )
		{
			m_CurrentGeometry->GetPoint(m_CurrentGeometry->GetPointCount() - 1) = p1;
			return true;
		}

		EnsureCanAdd(2, 2);

		return m_CurrentGeometry->AddLine(p0, p1);
	}

	void CRollingLineMessageWriter::WriteClearMessage()
	{
		CMessageWriter(m_Category).WriteClearMessage();
	}

	void CRollingLineMessageWriter::WriteGeometryMessage()
	{
		ASSERT(m_CurrentGeometry.get());

		if ( m_CurrentGeometry->GetPoints().Count() < 1 || m_CurrentGeometry->GetIndices().Count() < 1 )
		{
			m_CurrentGeometry.reset();
			return;
		}

		CMessageWriter(m_Category).WriteMessage(*m_CurrentGeometry);
	}

	void CRollingLineMessageWriter::CreateGeometryItem(uint32_t colour, float scale, float lifetime)
	{
		m_CurrentGeometry.reset(new CGeometryItem());

		m_CurrentGeometry->SetDrawType(DrawType::Lines);
		m_CurrentGeometry->SetColour(colour);
		m_CurrentGeometry->SetScale(scale);
		m_CurrentGeometry->SetLifetimeSecs(lifetime);
	}

	void CRollingLineMessageWriter::EnsureCanAdd(size_t pointsToAdd, size_t indicesToAdd)
	{
		ASSERT(m_CurrentGeometry.get());

		if ( !CanAdd(pointsToAdd, indicesToAdd) )
		{
			WriteGeometryMessage();
			CreateGeometryItem(
				m_CurrentGeometry->GetColour(),
				m_CurrentGeometry->GetScale(),
				m_CurrentGeometry->GetLifetimeSecs());
		}
	}

	bool CRollingLineMessageWriter::CanAdd(size_t pointsToAdd, size_t indicesToAdd) const
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

	bool CRollingLineMessageWriter::ExtendsCurrentLine(const Vector& p0, const Vector& p1) const
	{
		if ( !m_CurrentGeometry )
		{
			return false;
		}

		const CUtlVector<Vector>& points = m_CurrentGeometry->GetPoints();
		const int count = points.Count();

		if ( count < 2 )
		{
			return false;
		}

		const Vector& lastPoint = points[count - 1];
		const Vector& prevPoint = points[count - 2];
		const Vector delta = (lastPoint - prevPoint).Normalize();
		const Vector p0Delta = (p0 - prevPoint).Normalize();
		const Vector p1Delta = (p1 - prevPoint).Normalize();

		const bool p0OnLine = VectorCompareEpsilon(p0, lastPoint, 0.001f) || DotProduct(delta, p0Delta) >= 0.9999f;
		const bool p1OnLine = DotProduct(delta, p1Delta) >= 0.9999f;

		return p0OnLine && p1OnLine;
	}

}  // namespace CustomGeometry
