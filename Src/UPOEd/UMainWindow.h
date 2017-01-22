#pragma once

#include "UClassViewer.h"
#include "UPropertyBrowser.h"
#include "ULogWindow.h"
#include "UAssetBrowser.h"
#include "UAssetViewer.h"
#include "UMainViewport.h"
#include "UEntityBrowser.h"
#include "UProject.h"

namespace UPOEd
{
	struct StructStr
	{
		UCLASS(StructStr, void)

		StructStr();
		~StructStr();
		TestMyStruct mStr;
		void MetaPropertyChanged(const PropertyInfo* prp);
	};
	class TestObject : public Object
	{
		UCLASS(TestObject, Object)
	public:
		TestMetaClass mMetaClass;
		EPropertyType mPropertyType = EPropertyType::EPT_double;
		int mIntNoEdit = 99;
		float mFloat01 = 0.5;
		TArray<StructStr> mTArrayMS;

		TestObject();
		~TestObject();
		void MetaPropertyChanged(const PropertyInfo* prp);

		void Tick();
	};

	//////////////////////////////////////////////////////////////////////////
	class MainWindow : public QMainWindow
	{
	public:
		QWidget* mCentralWidget = nullptr;
		QStatusBar* mStatusBar = nullptr;
		
		QVBoxLayout* mVLayout = nullptr;

		QAction*	mActionClassViewer = nullptr;
		QMenuBar*	mMenuBar = nullptr;

		ClassViewerWindow*	mClassViewer = nullptr;

		PropertyBrowserDW* mPropertyBrowser = nullptr;
		TestObject* mTestObject = nullptr;

		LogDW* mLog = nullptr;
		AssetBrowserDW* mAssetBrowser = nullptr;

		MainViewport* mMainViewport = nullptr;

		EntityBrowserDW* mEntityBrowser = nullptr;

		World*	mActiveWorld = nullptr;
		World*  mPlayingWorld = nullptr;

		QAction* mActionOpenProject = nullptr;
		QAction* mActionNewProject = nullptr;
		QAction* mActionSaveProject = nullptr;

		ProjectInfo* mProject = nullptr;

		void EVOpenProject(bool);
		void EVNewProject(bool);
		void EVSaveProject(bool);

		void InitWidgets();
		void InitActions();
		void InitWorld();

		//create a default project in 'path' with specified 'projectName' and return the absolute path of the project file
		QString CreateDefaultProjectIn(const QString& path, const QString& projectName);

		bool OpenProject(const QString& projectFilePath);
		void CloseCurrentProject();
		
	public:
		MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
		~MainWindow();
		float mTickCounter = 0;
		void Tick();

		virtual void closeEvent(QCloseEvent *event) override;


// 		void dragEnterEvent(QDragEnterEvent *e) override
// 		{
// 			if (e->mimeData()->hasUrls()) {
// 				e->acceptProposedAction();
// 			}
// 		}
// 		void dropEvent(QDropEvent *e)
// 		{
// 			foreach(const QUrl &url, e->mimeData()->urls()) {
// 				QString fileName = url.toLocalFile();
// 				qDebug() << "Dropped file:" << fileName;
// 				ULOG_MESSAGE("dropped file %s", fileName);
// 			}
// 		}
	};

	extern MainWindow* gMainWindow;
};