#include "UClassViewer.h"

namespace UPOEd
{




	void ClassViewerWindow::EVSearchTextChanged(const QString&)
	{
		FillClassTree();
	}

	void ClassViewerWindow::EVClassTreeItemEntered(QTreeWidgetItem *item, int column)
	{

	}

	void ClassViewerWindow::EVClassTreeContextMenu(const QPoint& point)
	{
		if (mClassTree->itemAt(point))
		{

		}
	}

};