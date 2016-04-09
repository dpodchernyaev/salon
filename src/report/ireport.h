#ifndef IREPORT
#define IREPORT

#include <QVariant>
#include <QString>

class QSqlQuery;

class IReport
{
public:
	virtual ~IReport() {}
	virtual QString getSql() const = 0;
	virtual QVariantList getSqlParam() const = 0;
	virtual QList<QString> getResult() const = 0;
	virtual bool create(QSqlQuery *q) = 0;
};

#endif // IREPORT

