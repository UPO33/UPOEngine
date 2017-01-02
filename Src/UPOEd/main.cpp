#include "../UPOEngine/UPOEngine.h"
#include "UMainWindow.h"
#include "UEditorConfig.h"
#include "UD3DRenderWidget.h"
#include "UStartupWindow.h"

#include <qfilesystemwatcher.h>



#include "../UPOEngine/GFXCore/UGFXCore.h"

namespace UPOEd
{
	using namespace UPO;
	QFileSystemWatcher* fsw = new QFileSystemWatcher(nullptr);


	QSplashScreen* gSplash = nullptr;
	QApplication* gQApp = nullptr;
	ProjectInfo* gProject = nullptr;
	TArray<Module*>	mLoadedModules;

	class EngineEd : public IEngineInterface
	{
// 		QApplication* mQApp;
// 		UPOEd::MainWindow* mMainWindow;

		//////////////////////////////////////////////////////////////////////////
		virtual bool OnInit() override
		{
// 			mQApp = new QApplication(__argc, __argv);
// 
// 			QLocale::setDefault(QLocale::c());
// 			QApplication::setStyle("Fusion");
// 			/////loading style sheet
// 			{
// 				UPO::Buffer styleSheet;
// 				if (UPO::File::OpenReadFull(UPOEd::GEditorConfig()->AsName("Style").CStr(), styleSheet))
// 				{
// 					QString strSheet = QString::fromLatin1((const char*)styleSheet.Data(), (int)styleSheet.Size());
// 					mQApp->setStyleSheet(strSheet);
// 				}
// 			}
// 
// 			mMainWindow = new UPOEd::MainWindow();
// 			mMainWindow->showMaximized();

			gMainWindow = new MainWindow();
			gMainWindow->showMaximized();
			return true;
		}

		virtual bool OnTick() override
		{
			QApplication::processEvents();

			gMainWindow->Tick();

			return gMainWindow->isVisible();
		}

		virtual bool OnRelease() override
		{
			QApplication::exit();
			delete gMainWindow;
			return true;
		}

		virtual GameWindow* OnCreateGameWindow() override
		{

			return gMainWindow->mMainViewport;
		}

		virtual void OnReleaseGameWindow() override
		{
		}

	};
	void ParseCmdLine(int argc, const char** argv)
	{
		if (argc > 1)
		{
			
		}
	}
	void SplashSetMessage(const QString& msg)
	{
		if (gSplash)gSplash->showMessage(msg, Qt::AlignLeft | Qt::AlignBottom);
	}
	void ClearTmpFolder()
	{
		QDir("tmp").removeRecursively();
		QDir::current().mkdir("tmp");
	}
	void LoadProjectModules()
	{
#ifdef UPLATFORM_WIN
		String projModulesPath = GApp()->mProjectPath + "\\Bin\\Win32";
		QStringList moduleFilters;
		moduleFilters << "*.dll";
#else 
#endif
		QDir dir = QDir(ToQString(projModulesPath));

		dir.setNameFilters(moduleFilters);
		auto filesList = dir.entryInfoList(QDir::Filter::Files | QDir::Filter::NoDotAndDotDot);

		for (auto& file : filesList)
		{
			QString moduleFile = "tmp/" + file.fileName();
			QFile::copy(file.absoluteFilePath(), moduleFile);
		}
		for (auto& file : filesList)
		{
			QString moduleFile = "tmp/" + file.fileName();
			GModuleSys()->LoadModule(ToString(moduleFile));
		}
	}
	void UnloadProjectModules()
	{
		GModuleSys()->UnloadAll();
	}

	void ShaderCompiled(const ShaderUniqueParam& param)
	{
		QString msg = QString::asprintf("compiling %s shader %s %s", EnumToStr(param.mType), param.mFileName, param.mEntryPoint);
		SplashSetMessage(msg);
	}

	//////////////////////////////////////////////////////////////////////////
	int Main(int argc, char** argv)
	{


		gQApp = new QApplication(argc, argv);

		QCoreApplication::setOrganizationName("UPOSoft");
		QCoreApplication::setOrganizationDomain("UPO33.com");
		QCoreApplication::setApplicationName("UPOEd");
		QCoreApplication::setApplicationVersion("1.0");

		QLocale::setDefault(QLocale::c());


		ClearTmpFolder();

		QApplication::setStyle("Fusion");
		
		/////loading style sheet
		{
			Buffer styleSheet;
			if (File::OpenReadFull(GEditorConfig()->AsName("Style").CStr(), styleSheet))
			{
				QString strSheet = QString::fromLatin1((const char*)styleSheet.Data(), (int)styleSheet.Size());
				gQApp->setStyleSheet(strSheet);
			}
		}

		QString strProjectToOpen = QString();
#if 1 //open test project?
		strProjectToOpen = "C:/UPOEngine/TestProj/TestProj.uproj";
#else
		if (argc > 1)
		{
			strProjectToOpen = argv[1];
		}
		else
		{
			StartupWindow* startupWin = new StartupWindow(nullptr);
			startupWin->show();
			while (startupWin->isVisible())
			{
				QApplication::processEvents();
				QThread::msleep(10);
			}
			strProjectToOpen = startupWin->mResultProjectFilePath;
			delete startupWin;
		}
#endif


		if (strProjectToOpen.isEmpty()) //no project ?
		{
			ULOG_ERROR("no project to open");
			return 0;
		}

		//setting log file
		{
// 			String logFile = ToString(strProjectToOpen);
// 			GLog()->SetOutFile(logFile.SubStr(0, logFile.FindRN(PATH_SEPARATOR_CHAR)));
		}

		{
			String enginePath = argv[0];
			enginePath.Replace('\\', '/');
			enginePath = enginePath.SubStr(0, enginePath.FindRN('/', 1));

			String projectPath = ToString(strProjectToOpen);
			projectPath.Replace('\\', '/');
			projectPath = projectPath.SubStr(0, projectPath.FindRN('/'));

			GApp()->mIsEditor = true;
			GApp()->mProjectPath = projectPath;
			GApp()->mProjectAssetsPath = projectPath + '/'/*PATH_SEPARATOR_CHAR*/ + "Assets";
			GApp()->mEnginePath = enginePath;
			GApp()->mEngineAssetsPath = enginePath + '/'/*PATH_SEPARATOR_CHAR*/ + "Assets";
		
		}

		/////////splash screen
		{
			//initialize splash
			QPixmap pixmap("uposplash.png");
			gSplash = new QSplashScreen(pixmap);
			gSplash->show();


			LoadProjectModules();
			//UGlobalShader_CompileAll(&ShaderCompiled);


			//end of splash
			gSplash->close();
			delete gSplash;
			gSplash = nullptr;
		}

		

		UPO::LaunchEngine(new UPOEd::EngineEd);
		return 0;
	}
}



int main(int argc, char** argv)
{
	return UPOEd::Main(argc, argv);
}
