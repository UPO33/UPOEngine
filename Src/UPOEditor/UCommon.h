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
};