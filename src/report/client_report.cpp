
#include <QStringList>
#include <QVariantList>
#include <QSqlQuery>

#include "client_report.h"

ClientReport::ClientReport()
{

}

ClientReport::~ClientReport()
{

}

// ==== IReport =====
QString ClientReport::getSql() const
{
	return "SELECT"
				" client.name"
				", client.surname"
				", client.patronymic"
				", client.birthday"
				", client_service.name"
				", visit.dtime"
				", visit.info"
			" FROM client LEFT JOIN client_service INNER JOIN visit"
					" ON client_service.id = visit.client_service_id"
				" ON client.id = client_service.client_id"
			" WHERE client.id = ?"
				" AND ( (visit.dtime BETWEEN ? AND ?) OR visit.dtime IS NULL)"
			" ORDER BY visit.dtime";
}

void ClientReport::setParam(const Param &param)
{
	p = param;
}

QVariantList ClientReport::getSqlParam() const
{
	return QVariantList() << p.clId << p.d1 << p.d2;
}

QList<QString> ClientReport::getResult() const
{
	return result;
}

bool ClientReport::create(QSqlQuery* q)
{
	bool res = true;

	if (q == NULL || !q->isActive())
	{
		res = false;
		return res;
	}

	result.clear();

	bool first = true;
	while (q->next())
	{
		QString str = "";
		if (first == true)
		{
			str = q->value(1).toString() + " " + q->value(0).toString()
				  + " " + q->value(2).toString()
				  + " (" + q->value(3).toString() + ")\n";
			first = false;
		}
		if (!q->value(5).isNull())
		{
			str += "\t" + q->value(5).toDateTime().toString("dd.MM.yyyy hh.mm")
				+ " - " + q->value(6).toString() + " - (" + q->value(4).toString() + ")";
		}
		result.append(str);
	}
	return res;
}
