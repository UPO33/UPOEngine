#include "UCommon.h"

namespace UPOEd
{

	bool SearchCheck(const QString& string, const QString& searchString)
	{
		QStringList searchWords = searchString.split(' ', QString::SkipEmptyParts);

		if (searchWords.length() == 0) return true;

		for (auto str : searchWords)
		{
			if (string.contains(str, Qt::CaseInsensitive))
				return true;
		}
		return false;
	}

	QString GetPropertyName(const PropertyInfo& prp)
	{
		const char* cstr = prp.GetName().CStr();
		if (cstr[0] == 'm') return QString(cstr + 1);
		return QString(cstr);
	}

};