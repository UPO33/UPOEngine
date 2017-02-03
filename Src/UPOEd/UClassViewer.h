#pragma once

#include "UCommonEd.h"
#include "../UPOEngine/Meta/UMeta.h"


namespace UPOEd
{
	using namespace UPO;

	//////////////////////////////////////////////////////////////////////////
	class ClassViewerWidget : public QWidget
	{
		QTreeView*		mTreeView = nullptr;
		QVBoxLayout*	mLayout = nullptr;

	public:
		ClassViewerWidget(QWidget* parent) : QWidget(parent)
		{
			mLayout = new QVBoxLayout(this);
			mTreeView = new QTreeView(this);
			mLayout->addWidget(mTreeView);
		}
		~ClassViewerWidget()
		{

		}
		void Init()
		{
			

		}
	};
	//////////////////////////////////////////////////////////////////////////
	class ClassViewerWindow : public QMainWindow
	{
		QLineEdit*		mSearch;
		QWidget*		mCentralWidget;
		QTreeWidget*	mClassTree;

	public:
		//////////////////////////////////////////////////////////////////////////
		ClassViewerWindow(QWidget* parent) : QMainWindow(parent)
		{
			setMinimumSize(QSize(500, 400));
			
			mCentralWidget = new QWidget(this);
			mCentralWidget->setLayout(new QVBoxLayout());
			setCentralWidget(mCentralWidget);
			
			mSearch = new QLineEdit(mCentralWidget);
			connect(mSearch, &QLineEdit::textChanged, this, &ClassViewerWindow::EVSearchTextChanged);
			mSearch->setPlaceholderText("Search");
			mCentralWidget->layout()->addWidget(mSearch);

			mClassTree = new QTreeWidget(mCentralWidget);
			mClassTree->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
			mClassTree->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
			mClassTree->setHeaderHidden(true);
			mClassTree->setColumnCount(1);
			connect(mClassTree, &QTreeWidget::itemEntered, this, &ClassViewerWindow::EVClassTreeItemEntered);
			mClassTree->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
			connect(mClassTree, &QTreeWidget::customContextMenuRequested, this, &ClassViewerWindow::EVClassTreeContextMenu);

			mCentralWidget->layout()->addWidget(mClassTree);

			
			FillClassTree();
		}
		void AddToTree(const ClassInfo* ci, QTreeWidgetItem* parent)
		{
			QString className = ci->GetName().CStr();
			QTreeWidgetItem* item = parent;
			
			if(FilterCheck(mSearch->text(), className) && !ci->GetAttributes().HasAttrib(EAttribID::EAT_Hidden))
			{
				item = new QTreeWidgetItem(parent);
				item->setText(0, className);
				//TODO set icon
				mClassTree->addTopLevelItem(item);
			}
			for (size_t i = 0; i < ci->GetSubClasses().Length(); i++)
			{
				AddToTree(ci->GetSubClasses()[i], item);
			}
		}
		void FillClassTree()
		{
			mClassTree->clear();
			
			auto& types = MetaSys::Get()->GetAllTypes();

			///////////////get root classes
			TArray<ClassInfo*> rootClasses;
			for (size_t i = 0; i < types.Length(); i++)
			{
				if (ClassInfo* classInfo = types[i]->Cast<ClassInfo>())
				{
					if (!classInfo->HasParent()) rootClasses.Add(classInfo);
				}
			}

			for (size_t i = 0; i < rootClasses.Length(); i++)
			{
				AddToTree(rootClasses[i], nullptr);
			}

			mClassTree->expandAll();
		}
		~ClassViewerWindow()
		{

		}

		void EVSearchTextChanged(const QString&);
		void EVClassTreeItemEntered(QTreeWidgetItem *item, int column);
		void EVClassTreeContextMenu(const QPoint& point);
	};
};