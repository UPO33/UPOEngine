#pragma once

#include "../UPOEngine/Core/UCore.h"
#include "../UPOEngine/Meta/UMeta.h"
#include "../UPOEngine/Engine/UEngine.h"

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

#include "UFlowLayout.h"

//#define PROJECT_FILE_EXT ".uproj"

namespace UPOEd
{
	extern const QString PROJECT_FILE_EXT;

	using namespace UPO;

	extern bool FilterCheck(const QString& filter, const QString& string);
	extern QString GetPropertyName(const PropertyInfo& prp);

	Color ToColor(QColor c);
	Color32 ToColor32(QColor c);
	QColor ToQColor(const Color& c);
	QColor ToQColor(Color32 c);
	QString ToQString(const String& str);
	QString ToQString(Name  name);
	String ToString(const QString& str);
	Name ToName(const QString& str);

	QIcon* GetIcon(const QString& iconName);
	//returns the icon of a class if defined with UATTR_Icon, null if failed
	QIcon* GetIcon(const ClassInfo*);
	
	bool CopyPath(const QString& src, const QString& dst);

	//show a file or folder in explorer
	void UShowInExplorer(const String& path);
};