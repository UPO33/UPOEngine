#include "UColorSelectionDialog.h"

namespace UPOEd
{

	ColorSelectionDialog* ColorSelectionDialog::Active = nullptr;

	QRect ColorSelectionDialog::LastGemetry;

	ColorSelectionDialog* ColorSelectionDialog::OpenNow(QColor color)
	{
		if (Active == nullptr)
		{
			Active = new ColorSelectionDialog(nullptr);
			Active->setCurrentColor(color);
			Active->show();
			Active->setFocus();
			LastGemetry = Active->geometry();
		}
		else
		{
			Active->close();
			Active->setCurrentColor(color);
			Active->show();
			Active->setGeometry(LastGemetry);
			Active->setFocus();
		}
		return Active;
	}



};