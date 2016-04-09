
#include <QSqlQuery>

#include <report/ireport.h>
#include <db/dbconnection.h>
#include <db/db_service.h>

#include "report_engine.h"

ReportEngine::ReportEngine()
{
	QThread* th = DBService::getInstance()->getThread();
	moveToThread(th);

	connect(this, SIGNAL(createReportSignal(IReport*)),
			this, SLOT(createReportSlot(IReport*)));
}

ReportEngine::~ReportEngine()
{
}

void ReportEngine::createReport(IReport *report)
{
	Q_EMIT createReportSignal(report);
}

void ReportEngine::createReportSlot(IReport *report)
{
	if (report == NULL)
	{
		Q_EMIT reportCreated(report, false);
	}

	QVariantList vars = report->getSqlParam();

	DBConn* conn = DBService::getInstance()->getConnection();

	conn->beginTransaction();

	QSqlQuery q(conn->qtDatabase());
	q.prepare(report->getSql());
	for (int i = 0; i < vars.size(); i++)
	{
		q.bindValue(i, vars.value(i));
	}
	bool res = conn->executeQuery(q);

	if (res == true)
	{
		res = report->create(&q);
	}

	conn->commit();

	Q_EMIT reportCreated(report, res);
}

