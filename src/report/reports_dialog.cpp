
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>

#include <report/client_report.h>
#include <report/coach_report.h>

#include "reports_dialog.h"

CoachReportDialog::CoachReportDialog()
{
	setWindowTitle("Отчет по частным занятиям тренеров");

	QLabel* label = new QLabel("Начало:");
	t1 = new QDateEdit(QDate::currentDate().addMonths(-1));
	widgetBox->addWidget(label);
	widgetBox->addWidget(t1);

	label = new QLabel("Конец:");
	t2 = new QDateEdit(QDate::currentDate());
	widgetBox->addWidget(label);
	widgetBox->addWidget(t2);
}

CoachReportDialog::~CoachReportDialog()
{

}

IReport* CoachReportDialog::createReport() const
{
	CoachReport* res = new CoachReport;
	CoachReport::Param p;
	p.d1 = t1->date();
	p.d2 = t2->date();
	res->setParam(p);
	return res;
}

// ======================


ClientReportDialog::ClientReportDialog(int clId) : clId(clId)
{
	setWindowTitle("Отчет по посещениям клиента");

	QLabel* label = new QLabel("Начало:");
	t1 = new QDateEdit(QDate::currentDate().addMonths(-1));
	widgetBox->addWidget(label);
	widgetBox->addWidget(t1);

	label = new QLabel("Конец:");
	t2 = new QDateEdit(QDate::currentDate());
	widgetBox->addWidget(label);
	widgetBox->addWidget(t2);
}

ClientReportDialog::~ClientReportDialog()
{

}

IReport* ClientReportDialog::createReport() const
{
	ClientReport* res = new ClientReport;
	ClientReport::Param p;
	p.clId = clId;
	p.d1 = t1->date();
	p.d2 = t2->date();
	res->setParam(p);
	return res;
}
