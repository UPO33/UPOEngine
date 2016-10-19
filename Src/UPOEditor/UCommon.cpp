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
	//////////////////////////////////////////////////////////////////////////
	Color ToColor(QColor c)
	{
		return Color(c.redF(), c.greenF(), c.blueF(), c.alphaF());
	}
	//////////////////////////////////////////////////////////////////////////
	Color32 ToColor32(QColor c)
	{
		return Color32(c.red(), c.green(), c.blue(), c.alpha());
	}

	QColor ToQColor(const Color& c)
	{
		auto cc = Clamp(c, 0, 1);
		return QColor::fromRgbF(cc.mR, cc.mG, cc.mB, cc.mA);
	}

	QColor ToQColor(Color32 c)
	{
		return QColor::fromRgb(c.GetR(), c.GetG(), c.GetB(), c.GetA());
	}

	QString ToQString(const String& str)
	{
		if (str.IsEmpty()) return QString();
		return QString(str.CStr());
	}

	QString ToQString(Name name)
	{
		if (name.IsEmpty()) return QString();
		return QString(name.CStr());
	}

	String ToString(const QString& str)
	{
		return String(str.toStdString().c_str());
	}

	UPO::Name ToName(const QString& str)
	{
		return Name(str.toStdString().c_str());
	}

};