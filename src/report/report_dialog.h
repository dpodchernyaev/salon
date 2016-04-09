#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>

class ReportEngine;

class QVBoxLayout;

class IReport;

class ReportDialog : public QDialog
{
	Q_OBJECT
public:
	ReportDialog();
	virtual ~ReportDialog();

protected:
	virtual IReport* createReport() const = 0;

private Q_SLOTS:
	void createClicked();
	void reportCreated(IReport* report, bool res);

protected:
	QVBoxLayout* widgetBox;

private:
	ReportEngine *engine;
};

#endif // REPORTDIALOG_H
