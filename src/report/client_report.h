#ifndef CLIENTREPORT_H
#define CLIENTREPORT_H

#include <QDate>

#include <report/ireport.h>

class ClientReport : public IReport
{
public:
	ClientReport();
	virtual ~ClientReport();

	struct Param
	{
		int clId;
		QDate d1;
		QDate d2;
	};

	// ==== IReport =====
	virtual QString getSql() const;
	virtual QVariantList getSqlParam() const;
	virtual QList<QString> getResult() const;
	virtual bool create(QSqlQuery *q);

	void setParam(const Param &param);

private:
	Param p;
	QList<QString> result;
};

#endif // CLIENTREPORT_H
