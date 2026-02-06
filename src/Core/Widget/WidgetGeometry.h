#pragma once

#include <Math/myMath.h>

namespace Oolong
{
	class WidgetGeometry
	{
	public:
		WidgetGeometry();

		~WidgetGeometry();

		//static WidgetGeometry makeRoot(const float2& inLocalSize);

		float2 m_localPosition;
		float2 m_localSize;

		float m_absoluteScale;
		float2 m_absolutePosition;
	};
}