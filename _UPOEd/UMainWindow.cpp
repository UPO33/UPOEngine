#include "UMainWindow.h"

#include "../UPOEngine/UPOEngine.h"


#include "UNewProjectDialog.h"

namespace UPOEd
{

	static const char* UPO_PROJECT_FILE_EXT = ".uproj";

	MainWindow* gMainWindow = nullptr;


	UCLASS_BEGIN_IMPL(TestObject)
		UPROPERTY(mMetaClass)
		UPROPERTY(mPropertyType)
		UPROPERTY(mIntNoEdit, UATTR_Uneditable(), UATTR_Comment("this cant be edited"))
		UPROPERTY(mFloat01, UATTR_Range(0,1))
		UPROPERTY(mTArrayMS)
		UPROPERTY(mTestObj, UATTR_ShowProperties())
		UPROPERTY(mMetaClass.mVec2)
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
		static bool Initilized = false;
		mTestObj = nullptr;
		if (!Initilized)
		{
			Initilized = true;
			mTestObj = NewObject<TestObject>();
		}
	}

	TestObject::~TestObject()
	{

	}
	
	void TestObject::MetaAfterPropertyChange(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("% prp changed", prp->GetName());
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
		ULOG_WARN("%", this);
	}

	StructStr::~StructStr()
	{
		ULOG_WARN("");
	}

	void StructStr::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%", prp->GetName().CStr());
	}
	//////////////////////////////////////////////////////////////////////////
	void MainWindow::EVOpenProject(bool)
	{
		if (mProject && mProject->NeedsSave())
		{
			auto result = QMessageBox::warning(this, "saving", "do you want to save current project ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			if (result == QMessageBox::Yes)
			{
				EVSaveProject(true);
			}
		}
		QString selsectedFile = QFileDialog::getOpenFileName(this, "Open Project", QString(), QString("*") + UPO_PROJECT_FILE_EXT);
		if (!selsectedFile.isEmpty())
		{
			OpenProject(selsectedFile);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MainWindow::EVNewProject(bool)
	{
		if (mProject && mProject->NeedsSave())
		{
			auto result = QMessageBox::warning(this, "saving", "do you want to save current project ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			if (result == QMessageBox::Yes)
			{
				EVSaveProject(true);
			}
		}
		NewProjectDialog* npd = new NewProjectDialog();
		if (npd->exec() == QDialog::Accepted)
		{	
			QString projFilePath = CreateDefaultProjectIn(npd->GetLocation(), npd->GetProjName());
			if (!projFilePath.isEmpty())
			{
				OpenProject(projFilePath);
			}
			else
			{
				ULOG_ERROR("creating new project failed");
			}
		}
		delete npd;
	}

	void MainWindow::EVSaveProject(bool)
	{
		//TODO saving all assets and settings
	}

	void MainWindow::InitWidgets()
	{

	}

	void MainWindow::InitActions()
	{
		//file menu
		{
			mActionNewProject = new QAction("&New Project", this);
			connect(mActionNewProject, &QAction::triggered, this, &MainWindow::EVNewProject);
			mActionOpenProject = new QAction("&Open Project", this);
			connect(mActionOpenProject, &QAction::triggered, this, &MainWindow::EVOpenProject);
			mActionSaveProject = new QAction("&Save Project", this);
			connect(mActionSaveProject, &QAction::triggered, this, &MainWindow::EVSaveProject);

			QMenu* fileMenu = menuBar()->addMenu("File");
			fileMenu->addAction(mActionNewProject);
			fileMenu->addAction(mActionOpenProject);
			fileMenu->addAction(mActionSaveProject);
		}

		//view menu
		{
			QMenu* viewMenu = menuBar()->addMenu("View");


			mActionClassViewer = new QAction("Class Viewer", this);
			mActionClassViewer->setStatusTip("show class viewer window");
			connect(mActionClassViewer, &QAction::triggered, this, [&](bool) {
				mClassViewer = new ClassViewerWindow(this);
				mClassViewer->show();
			});

			viewMenu->addAction(mActionClassViewer);
		}

		// 			mMenuBar = new QMenuBar(this);
		// 			setMenuBar(mMenuBar);
	}
	void MainWindow::InitWorld()
	{
		//////////////////////////////////////////////////////////////////////////TEST World
		{
			WorldInitParam wip;
			wip.mStartPlaying = true;
			mActiveWorld = IEngineInterface::Get()->CreateWorld(wip);
			mMainViewport->SetWorld(mActiveWorld);
			mActiveWorld->CreateEntity<Entity>(nullptr);
			Entity* parent = mActiveWorld->CreateEntity<Entity>(nullptr);
			for (size_t i = 0; i < 4; i++)
			{
				mActiveWorld->CreateEntity<Entity>(parent);
			}
			mActiveWorld->CreateEntity<EntityTest>(nullptr);

			auto smesh = mActiveWorld->CreateEntity<EntityStaticMesh>(nullptr);
			auto buildingAsset = GAssetSys()->LoadAsset("Engine/meshes/building1", smesh);
			ULOG_MESSAGE("%", buildingAsset);
			smesh->SetMesh(buildingAsset ? buildingAsset->Cast<AStaticMesh>() : nullptr);
			auto smesh2 = mActiveWorld->CreateEntity<EntityStaticMesh>(smesh);
			smesh2->SetWorldTransform(Vec3(100), Vec3::ZERO, Vec3(0.5f));
			smesh2->SetMesh(buildingAsset ? buildingAsset->Cast<AStaticMesh>() : nullptr);

			mEntityBrowser->AttachWorld(mActiveWorld);
		};

	}
	QString GetTempalteProjectPath()
	{



		QDir dir = QDir::current();
		dir.cdUp();
		return dir.absoluteFilePath("TemplateProject");
	}
	void WriteProjectFileContent(QFile& file, const QString& projectName)
	{
		file.write("UPOProject");
		//we can write required properties to project's file
	}
	//////////////////////////////////////////////////////////////////////////
	QString MainWindow::CreateDefaultProjectIn(const QString& path, const QString& projectName)
	{
// 		{
// 			QDir(path).mkdir(projectName);
// 			QString projRoot = path + QDir::separator() + projectName;
// 			CopyPath(GetTempalteProjectPath(), projRoot);
// 			QFile::rename(projRoot + QDir::separator() + "TemplateProject.uproj", );
// 
// 		}

		QDir(path).mkdir(projectName);
		QDir dirProject = QDir(path);
		dirProject.mkdir(projectName);
		dirProject.cd(projectName);
		dirProject.mkdir("Bin");
		dirProject.mkdir("Content");
		dirProject.mkdir("Src");

		//creating project file
		{
			QString projectFilePath = dirProject.absoluteFilePath(projectName + UPO_PROJECT_FILE_EXT);
			QFile projectFile(projectFilePath);

			projectFile.open(QIODevice::OpenModeFlag::WriteOnly);
			UASSERT(projectFile.isOpen());
			WriteProjectFileContent(projectFile, projectName);
			projectFile.close();

			return projectFilePath;
		}
	}

	bool MainWindow::OpenProject(const QString& projectFilePath)
	{
		if (mProject)
		{
			CloseCurrentProject();
		}
		else
		{

		}
		return true;
	}

	void MainWindow::CloseCurrentProject()
	{
		if (mProject)
		{
			if (mPropertyBrowser) mPropertyBrowser->AttachObject(nullptr);
			if (mEntityBrowser) mEntityBrowser->AttachWorld(nullptr);

			

			if (mLog) mLog->Clean();

		}
	}

	MainWindow::MainWindow(QWidget *parent /*= nullptr*/, Qt::WindowFlags flags /*= Qt::WindowFlags()*/) : QMainWindow(parent, flags)
	{	
// 		setAcceptDrops(true);

		mStatusBar = new QStatusBar(this);
		setStatusBar(mStatusBar);
		setMinimumSize(QSize(500, 500));

		mMainViewport = new MainViewport(nullptr);
		setCentralWidget(mMainViewport);
		GameWindowCreationParam gwcp;
		gwcp.mVSyncEnable = false;
		mMainViewport->GetViewport()->InitAndReg(gwcp);

		mTestObject = NewObject<TestObject>();
		
		
		/////////////////property browser
		mPropertyBrowser = new PropertyBrowserDW(this);
		this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, mPropertyBrowser);
		mPropertyBrowser->AttachObject(mTestObject);

		/////////////log window
		mLog = new LogDW(this);
		this->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, mLog);
		//////////entity browser
		mEntityBrowser = new EntityBrowserDW(this);
		this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mEntityBrowser);

		//////////////asset browser
		mAssetBrowser = new AssetBrowserDW(this);
		this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mAssetBrowser);
		
		//bottom space belong to left and right doc
		this->setCorner(Qt::Corner::BottomLeftCorner, Qt::LeftDockWidgetArea);
		this->setCorner(Qt::Corner::BottomRightCorner, Qt::RightDockWidgetArea);

		//this->tabifyDockWidget(mAssetBrowser, mEntityBrowser);



		InitActions();
		InitWorld();

		QToolBar* toolbar=  this->addToolBar("tiilbar");
		toolbar->addAction("Play");
		toolbar->addAction("Save");
	}

	MainWindow::~MainWindow()
	{
		ULOG_MESSAGE("");
	}

	void MainWindow::Tick()
	{
		if (mPropertyBrowser) mPropertyBrowser->Tick();
		if (mAssetBrowser)	mAssetBrowser->Tick();
		if (mEntityBrowser) mEntityBrowser->Tick();
		if (mMainViewport) mMainViewport->Tick();

		AssetViewer::Tick();

		if (mLog) mLog->Tick();
	}



	void MainWindow::closeEvent(QCloseEvent *event)
	{
		if (mMainViewport)
		{
			mMainViewport->GetViewport()->ReleaseAndUnreg();
			delete mMainViewport;
			mMainViewport = nullptr;
		}

		event->accept();
	}


};