#pragma once

#include "../UPOEngine/Core/UCore.h"
#include "../UPOEngine/Meta/UMeta.h"

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

namespace UPOEd
{
	using namespace UPO;

	extern bool SearchCheck(const QString& string, const QString& searchString);
	extern QString GetPropertyName(const PropertyInfo& prp);

	Color ToColor(QColor c);
	Color32 ToColor32(QColor c);
	QColor ToQColor(const Color& c);
	QColor ToQColor(Color32 c);
	QString ToQString(const String& str);
	QString ToQString(Name  name);
	String ToString(const QString& str);
	Name ToName(const QString& str);
};