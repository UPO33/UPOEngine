#pragma once

#include "UClassViewer.h"
#include "UPropertyBrowser.h"


namespace UPOEd
{
	class MainWindow : public QMainWindow
	{

		QWidget* mCentralWidget = nullptr;
		QStatusBar* mStatusBar = nullptr;
		
		QVBoxLayout* mVLayout = nullptr;

		QAction*	mActionClassViewer = nullptr;
		QMenuBar*	mMenuBar = nullptr;

		ClassViewerWindow*	mClassViewer;

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

			QDockWidget* dock = new QDockWidget(this);
			dock->setWidget(new PropertyBrowserWidget(TestMetaClass::GetClassInfoStatic(), new TestMetaClass));
			this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dock);
			
			menuBar()->addMenu("File");
			QMenu* menuView = menuBar()->addMenu("View");


			mActionClassViewer = new QAction("Class Viewer", this);
			mActionClassViewer->setStatusTip("show class viewer window");
			connect(mActionClassViewer, &QAction::triggered, this, &MainWindow::ClassViewerTriggered);

			menuView->addAction(mActionClassViewer);
			
// 			mMenuBar = new QMenuBar(this);
// 			setMenuBar(mMenuBar);
			
			
		};
		 
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