#include "UMainWindow.h"

#include "../UPOEngine/UPOEngine.h"

namespace UPOEd
{









	UCLASS_BEGIN_IMPL(TestObject)
		UPROPERTY(mMetaClass)
		UPROPERTY(mPropertyType)
		UPROPERTY(mIntNoEdit, UATTR_Uneditable(), UATTR_Comment("this cant be edited"))
		UPROPERTY(mFloat01, UATTR_Range(0,1))
		UPROPERTY(mTArrayMS)
	UCLASS_END_IMPL(TestObject)

	UCLASS_BEGIN_IMPL(StructStr)
		UPROPERTY(mStr)
	UCLASS_END_IMPL(StructStr)

	TestObject::TestObject()
	{
		mTArrayMS.AddDefault();
		mTArrayMS.AddDefault();
// 		mTArrayMS[0].mStr = 10;
// 		mTArrayMS[1].mStr = 20;
	}

	TestObject::~TestObject()
	{

	}
	
	void TestObject::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%s", prp->GetName().CStr());
	}

	void TestObject::Tick()
	{
		mFloat01 += 1;
		mMetaClass.mColor = Color(RandFloat01());

		
	}

	StructStr::StructStr()
	{
		mStr.mPosition = Vec3(1, 1, 1);
		mStr.mStringHello = " hello";
		ULOG_WARN("%d", this);
	}

	StructStr::~StructStr()
	{
		ULOG_WARN("");
	}

	void StructStr::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%s", prp->GetName().CStr());
	}

	MainWindow::MainWindow(QWidget *parent /*= nullptr*/, Qt::WindowFlags flags /*= Qt::WindowFlags()*/) : QMainWindow(parent, flags)
	{	
// 		setAcceptDrops(true);

		mStatusBar = new QStatusBar(this);
		setStatusBar(mStatusBar);
		setMinimumSize(QSize(500, 500));

		mMainViewport = new MainViewport(this);
		setCentralWidget(mMainViewport);

		mTestObject = NewObject<TestObject>();
		
		/////////////////property browser
		mPropertyBrowser = new PropertyBrowserDW(this);
		this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, mPropertyBrowser);
		mPropertyBrowser->AttachObject(mTestObject);

		/////////////log window
		mLog = new LogDW(this);
		this->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, mLog);

		//////////////asset browser
		mAssetBrowser = new AssetBrowserDW(this);
		this->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, mAssetBrowser);
		
		//////////entity browser
		mEntityBrowser = new EntityBrowserDW(this);
		this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mEntityBrowser);

		
		menuBar()->addMenu("File");
		QMenu* menuView = menuBar()->addMenu("View");


		mActionClassViewer = new QAction("Class Viewer", this);
		mActionClassViewer->setStatusTip("show class viewer window");
		connect(mActionClassViewer, &QAction::triggered, this, [&](bool) {
			mClassViewer = new ClassViewerWindow(this);
			mClassViewer->show();
		});

		menuView->addAction(mActionClassViewer);

		// 			mMenuBar = new QMenuBar(this);
		// 			setMenuBar(mMenuBar);
	}

	void MainWindow::Tick()
	{
		if(mPropertyBrowser) mPropertyBrowser->Tick();
		if(mAssetBrowser) mAssetBrowser->Tick();
		if(mLog) mLog->Tick();
		if (mMainViewport) mMainViewport->Tick();

		if (AssetViewer::Current) AssetViewer::Current->Tick();
	}

	void MainWindow::InitWorld()
	{
		mActiveWorld = new UPO::World();
	}

};