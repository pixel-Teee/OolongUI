#pragma once

#include <memory>

namespace Oolong
{
	class WidgetBase
	{
	public:
		WidgetBase();

		virtual ~WidgetBase();

	protected:
		//weak to parent widget
		std::weak_ptr<WidgetBase> m_parentWidget;
	};
}