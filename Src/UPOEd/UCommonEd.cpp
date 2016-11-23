#include "UCommonEd.h"

namespace UPOEd
{

	const QString PROJECT_FILE_EXT = ".uproj";

	extern bool FilterCheck(const QString& filter, const QString& string)
	{
		QStringList searchWords = filter.split(' ', QString::SkipEmptyParts);

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

	QIcon* GetIcon(const QString& iconName)
	{
		struct Naked : public QMap<QString, QIcon*>
		{
			~Naked()
			{
				for (auto item : *this)
				{
					delete item;
				}
			}
		};
		static Naked IconsMap;

		QIcon* icon = nullptr;
		if (icon = IconsMap.value(iconName, nullptr))
			return icon;

		IconsMap[iconName] = icon = new QIcon(QString("../Icons/") + iconName);
		return icon;
	}

	QIcon* GetIcon(const ClassInfo* ci)
	{
		Attrib iconAttrib;
		if (ci && ci->GetAttrib(EAT_Icon, iconAttrib))
			return GetIcon(ToQString(iconAttrib.GetString()));
		return nullptr;
	}

	bool CopyPath(const QString& src, const QString& dst)
	{
		QDir dir(src);
		if (!dir.exists())
			return false;

		foreach(QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
			QString dst_path = dst + QDir::separator() + d;
			dir.mkpath(dst_path);
			CopyPath(src + QDir::separator() + d, dst_path);
		}

		foreach(QString f, dir.entryList(QDir::Files)) {
			QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void UShowInExplorer(const String& path)
	{
		QString pathIn = QDir::toNativeSeparators(ToQString(path));
#if defined(Q_OS_WIN)
		QString param;
		if (!QFileInfo(pathIn).isDir())
			param = QLatin1String("/select,");
		param += QDir::toNativeSeparators(pathIn);
		QString command = QString("explorer.exe") + " " + param;
		QProcess::startDetached(command);
#elif defined(Q_OS_MAC)
		QStringList scriptArgs;
		scriptArgs << QLatin1String("-e")
			<< QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
			.arg(pathIn);
		QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
		scriptArgs.clear();
		scriptArgs << QLatin1String("-e")
			<< QLatin1String("tell application \"Finder\" to activate");
		QProcess::execute("/usr/bin/osascript", scriptArgs);
#else
		// we cannot select a file here, because no file browser really supports it...
		const QFileInfo fileInfo(pathIn);
		const QString folder = fileInfo.absoluteFilePath();
		const QString app = Utils::UnixUtils::fileBrowser(Core::ICore::instance()->settings());
		QProcess browserProc;
		const QString browserArgs = Utils::UnixUtils::substituteFileBrowserParameters(app, folder);
		if (debug)
			qDebug() << browserArgs;
		bool success = browserProc.startDetached(browserArgs);
		const QString error = QString::fromLocal8Bit(browserProc.readAllStandardError());
		success = success && error.isEmpty();
		if (!success)
			showGraphicalShellError(parent, app, error);
#endif
	}

};