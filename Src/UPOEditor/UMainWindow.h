#pragma once

#include "UClassViewer.h"
#include "UPropertyBrowser.h"


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

		QWidget* mCentralWidget = nullptr;
		QStatusBar* mStatusBar = nullptr;
		
		QVBoxLayout* mVLayout = nullptr;

		QAction*	mActionClassViewer = nullptr;
		QMenuBar*	mMenuBar = nullptr;

		ClassViewerWindow*	mClassViewer;

		PropertyBrowserDW* mPropertyBrowser = nullptr;
		TestObject* mTestObject = nullptr;

	public:
		MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags())
			: QMainWindow(parent, flags) 
		{
			mCentralWidget = new QWidget(this);
			setCentralWidget(mCentralWidget);
			mStatusBar = new QStatusBar(this);
			setStatusBar(mStatusBar);
			setMinimumSize(QSize(500, 500));

			mVLayout = new QVBoxLayout(mCentralWidget);
			QLabel* lbl = new QLabel(this);
			lbl->setText("teeeeeeeeeeeeeeeext");
			mVLayout->addWidget(lbl);
			QPushButton* btn = new QPushButton(this);
			btn->setText("click");
			mVLayout->addWidget(btn);

			mTestObject = NewObject<TestObject>();
			mPropertyBrowser = new PropertyBrowserDW(this);
			this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, mPropertyBrowser);
			mPropertyBrowser->AttachObject(mTestObject);

			menuBar()->addMenu("File");
			QMenu* menuView = menuBar()->addMenu("View");


			mActionClassViewer = new QAction("Class Viewer", this);
			mActionClassViewer->setStatusTip("show class viewer window");
			connect(mActionClassViewer, &QAction::triggered, this, &MainWindow::ClassViewerTriggered);

			menuView->addAction(mActionClassViewer);
			
// 			mMenuBar = new QMenuBar(this);
// 			setMenuBar(mMenuBar);
			
			
		};
		float mTickCounter = 0;
		void Tick()
		{
			mPropertyBrowser->Tick();
			mTickCounter++;
			if (mTickCounter == 100)
			{
				mTestObject->Tick();
				mTickCounter = 0;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		void ClassViewerTriggered(bool checked)
		{
			if (mClassViewer)
			{

			}
			else
			{
				mClassViewer = new ClassViewerWindow(this);
				ULOG_MESSAGE("class viewr created");
			}

			mClassViewer->show();
		}
		
		
	};
};