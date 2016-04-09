
#pragma once

#include <QDateEdit>
#include <report/report_dialog.h>

class CoachReportDialog : public ReportDialog
{
public:
	CoachReportDialog();
	virtual ~CoachReportDialog();

protected:
	IReport* createReport() const;

private:
	QDateEdit* t1;
	QDateEdit* t2;
};

class ClientReportDialog : public ReportDialog
{
public:
	ClientReportDialog(int clId);
	virtual ~ClientReportDialog();

protected:
	IReport* createReport() const;

private:
	QDateEdit* t1;
	QDateEdit* t2;
	int clId;
};
