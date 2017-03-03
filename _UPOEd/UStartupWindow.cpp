#include "UStartupWindow.h"

#include "ui_StartupWindow.h"

namespace UPOEd
{
	static const char* RecentProjectsRegistryKey = "RecentProjects";
	

// 	void WriteProjectFileContent(QFile& projectFile, const QString& projectName)
// 	{
// 
// 	}
	QString CreateDefaulProjectIn(const QString& path, const QString& projectName)
	{
		QDir(path).mkdir(projectName);
		QDir dirProject = QDir(path);
		dirProject.mkdir(projectName);
		dirProject.cd(projectName);
		dirProject.mkdir("Bin");
		dirProject.mkdir("Content");
		dirProject.mkdir("Src");

		//creating project file
		{
			QString projectFilePath = dirProject.absoluteFilePath(projectName + ".uproj");
			QFile projectFile(projectFilePath);

			projectFile.open(QIODevice::OpenModeFlag::WriteOnly);
			UASSERT(projectFile.isOpen());
			//WriteProjectFileContent(projectFile, projectName);
			projectFile.close();

			return projectFilePath;
		}
	}
	StartupWindow::StartupWindow(QWidget* parent /*= nullptr*/)
	{
		ui = new Ui_StartupWindow();
		ui->setupUi(this);

		setWindowTitle("UPOEd");

		mResultProjectFilePath = QString();

		{
			QSettings setting;
			QVariant vRecentProjects = setting.value(RecentProjectsRegistryKey);
			if (vRecentProjects.isValid())
			{
				QStringList strProjects = vRecentProjects.toStringList();
				for (auto iter : strProjects)
				{
					if (QFileInfo::exists(iter))
					{
						mRecentProjects << iter;
					}
				}
				setting.setValue(RecentProjectsRegistryKey, QVariant(mRecentProjects));
				ui->recentProjects->addItems(mRecentProjects);
			}
		}

		

		connect(ui->btnBrowse, &QPushButton::clicked, this, [&]() 
		{
			QString selsectedFile = QFileDialog::getOpenFileName(this, "Open Project", QString(), QString("*.uproj"));
			if (!selsectedFile.isEmpty())
			{
				Finish(selsectedFile);
			}
		});

		connect(ui->recentProjects, &QListWidget::itemClicked, this, [&](QListWidgetItem* item)
		{
			Finish(item->text());
		});
		connect(ui->btnLocation, &QPushButton::clicked, this, [&](bool)
		{
			QString dir = QFileDialog::getExistingDirectory(this, "New Project", ui->location->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
			ui->location->setText(dir);
		});

		connect(ui->btnCreate, &QPushButton::clicked, this, [&](bool)
		{
			//TODO: add checking validation of project's name and path
			QString projectFilePath = CreateDefaulProjectIn(ui->location->text(), ui->projName->text());
			Finish(projectFilePath);
		});
	}

	StartupWindow::~StartupWindow()
	{
		delete ui;
	}


	void StartupWindow::Finish(const QString& projectFile)
	{
		mResultProjectFilePath = projectFile;
		if (!projectFile.isEmpty() && !mRecentProjects.contains(projectFile))
		{
			mRecentProjects << projectFile;
			QSettings setting;
			setting.setValue(RecentProjectsRegistryKey, QVariant(mRecentProjects));
		}

		this->close();
		
	}
};