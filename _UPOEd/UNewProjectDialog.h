#pragma once

#include "UCommonEd.h"

class Ui_NewProjectDialog;

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class NewProjectDialog : public QDialog
	{
		Ui_NewProjectDialog* ui;

	public:
		NewProjectDialog(QWidget* parent = nullptr);
		~NewProjectDialog();
		
		QString GetProjName();
		QString GetLocation();
	};
};