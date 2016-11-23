#include "UCommonEd.h"

class Ui_StartupWindow;

namespace UPOEd
{
	class StartupWindow : public QWidget
	{
		Ui_StartupWindow* ui;

	public:
		StartupWindow(QWidget* parent = nullptr);
		~StartupWindow();

		QString mResultProjectFilePath;


	private:
		QStringList mRecentProjects;
		void Finish(const QString& projectFile = QString());

	};
};