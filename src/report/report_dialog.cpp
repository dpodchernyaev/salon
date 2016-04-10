
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <report/ireport.h>
#include <config/config.h>

#include <report/report_engine.h>

#include "report_dialog.h"

ReportDialog::ReportDialog()
{
	setStyleSheet("QDialog { background:rgb(255,253,208); }");

	engine = new ReportEngine;

	QPushButton* createBtn = new QPushButton;
	createBtn->setToolTip("Создать отчет");
	createBtn->setIcon(QIcon("pics/use.png"));

	QPushButton* exitBtn = new QPushButton;
	exitBtn->setToolTip("Закрыть окно");
	exitBtn->setIcon(QIcon("pics/exit.png"));

	widgetBox = new QVBoxLayout;

	QHBoxLayout* btnbox = new QHBoxLayout;
	btnbox->addStretch(1);
	btnbox->addWidget(createBtn);
	btnbox->addWidget(exitBtn);

	QVBoxLayout* mainBox = new QVBoxLayout;
	mainBox->addLayout(widgetBox);
	mainBox->addLayout(btnbox);

	setLayout(mainBox);

	connect(engine, SIGNAL(reportCreated(IReport*, bool)),
			this, SLOT(reportCreated(IReport*,bool)));

	connect(createBtn, SIGNAL(clicked(bool)), this, SLOT(createClicked()));
	connect(exitBtn, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

ReportDialog::~ReportDialog()
{
	engine->deleteLater();
}

void ReportDialog::createClicked()
{
	IReport* report = createReport();
	engine->createReport(report);
	setEnabled(false);
}

void ReportDialog::reportCreated(IReport* report, bool res)
{
	setEnabled(true);
	if (res == false)
	{
		QMessageBox::critical(this, "Ошибка", "Ошибка при формировании отчета");
		return;
	}

	QString dirStr = Config::getInstance()->getValue(REPORT_DIR).toString();
	dirStr = dirStr.replace("\\", "/");

	QString path = QFileDialog::getSaveFileName(this, "Сохранение отчета",
								 dirStr, "Текстовые файлы (*.txt)");

	if (!path.isEmpty())
	{
		QFile file(path);
		if (file.open(QFile::WriteOnly | QFile::Truncate) == false)
		{
			QMessageBox::critical(this, "Ошибка", "Ошибка открытия файла отчета для записи");
			return;
		}

		QTextStream stream(&file);

		Q_FOREACH (QString str, report->getResult())
		{
			stream << str << endl;
		}

		file.close();
	}

	delete report;
}

