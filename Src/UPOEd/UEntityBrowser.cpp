#include "UEntityBrowser.h"

namespace UPOEd
{

	EntityBrowserDW::EntityBrowserDW(QWidget* parent) : QDockWidget(parent)
	{
		this->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);
		this->setWindowTitle("Entities");
		this->setContentsMargins(0, 0, 0, 0);
		this->setWidget(new EntityBrowserWidget);
	}

	void EntityBrowserDW::AttachWorld(World* world)
	{

	}

	EntityBrowserWidget::EntityBrowserWidget(QWidget* parent ) : QWidget(parent)
	{
		mAttachedWorld = nullptr;

		setLayout(new QVBoxLayout);
		mFilter = new QLineEdit(this);
		mFilter->setClearButtonEnabled(true);
		layout()->addWidget(mFilter);

		mTree = new QTreeWidget(this);
		layout()->addWidget(mTree);
		mTree->setHeaderHidden(true);
		mTree->setAlternatingRowColors(true);

		setMinimumSize(QSize(300, 300));
	}

	EntityBrowserWidget::~EntityBrowserWidget()
	{

	}

	void EntityBrowserWidget::ReFillTree()
	{
		mTree->clear();

	}

};