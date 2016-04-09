
#pragma once

#include <QDate>

#include <report/ireport.h>

class CoachReport : public IReport
{
public:
	CoachReport();
	virtual ~CoachReport();

	struct Param
	{
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
