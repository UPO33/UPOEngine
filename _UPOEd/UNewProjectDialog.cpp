#include "UNewProjectDialog.h"

#include "ui_NewProjectDialog.h"

namespace UPOEd
{

	NewProjectDialog::NewProjectDialog(QWidget* parent /*= nullptr*/) : QDialog(parent)
	{
		ui = new Ui_NewProjectDialog();
		ui->setupUi(this);

		this->setWindowTitle("New Project");

		connect(ui->locationBtn, &QPushButton::clicked, this, [&](bool) 
		{
			QString dir = QFileDialog::getExistingDirectory(this, "New Project", ui->location->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
			ui->location->setText(dir);
		});

		connect(ui->createBtn, &QPushButton::clicked, this, [&](bool) 
		{
			this->accept();
		});
	}


	NewProjectDialog::~NewProjectDialog()
	{
		delete ui;
	}

	QString NewProjectDialog::GetProjName()
	{
		return ui->projName->text();
	}

	QString NewProjectDialog::GetLocation()
	{
		return ui->location->text();
	}

};