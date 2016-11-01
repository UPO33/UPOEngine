#pragma once

#include "UClassViewer.h"
#include "UPropertyBrowser.h"
#include "ULogWindow.h"
#include "UAssetBrowser.h"
#include "UAssetViewer.h"
#include "UMainViewport.h"
#include "UEntityBrowser.h"

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
		
	public:
		MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
		float mTickCounter = 0;
		void Tick();
		
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
		void InitWorld();
	};
};