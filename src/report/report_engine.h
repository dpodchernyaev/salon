#ifndef REPORTENGINE_H
#define REPORTENGINE_H

#include <QObject>
#include <QVariant>

class IReport;


class ReportEngine : public QObject
{
	Q_OBJECT
public:
	ReportEngine();
	virtual ~ReportEngine();

	void createReport(IReport* report);

private Q_SLOTS:
	void createReportSlot(IReport* report);

Q_SIGNALS:
	void createReportSignal(IReport* report);
	void reportCreated(IReport* report, bool res);
};

#endif // REPORTENGINE_H
