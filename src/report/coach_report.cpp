
#include <QStringList>
#include <QVariantList>
#include <QSqlQuery>

#include "coach_report.h"

CoachReport::CoachReport()
{

}

CoachReport::~CoachReport()
{

}

// ==== IReport =====
QString CoachReport::getSql() const
{
	return "SELECT"
				" coach.name"
				", vgroup.bdtime"
				", vid.name"
				", hall.name"
			" FROM"
				" coach"
					" INNER JOIN"
						" vgroup"
							" INNER JOIN hall"
								" ON vgroup.hall_id = hall.id"
							" INNER JOIN vid"
								" ON vgroup.vid_id = vid.id"
							" INNER JOIN private_group"
								" ON vgroup.id = private_group.vgroup_id"
						" ON coach.id = vgroup.coach_id"
			" WHERE (vgroup.bdtime BETWEEN ? AND ?) OR vgroup.bdtime IS NULL"
			" ORDER BY coach.name, vgroup.bdtime";
}

void CoachReport::setParam(const Param &param)
{
	p = param;
}

QVariantList CoachReport::getSqlParam() const
{
	return QVariantList() << QDateTime(p.d1) << QDateTime(p.d2);
}

QList<QString> CoachReport::getResult() const
{
	return result;
}

bool CoachReport::create(QSqlQuery* q)
{
	bool res = true;

	if (q == NULL || !q->isActive())
	{
		res = false;
		return res;
	}

	result.clear();

	QString currName = "";
	while (q->next())
	{
		if (q->value(0).isNull() || q->value(1).isNull())
		{
			continue;
		}

		QString str;
		QString name = q->value(0).toString();
		if (currName.isEmpty() || currName != name)
		{
			currName = name;
			str = name + "\n";
		}
		str += "\t" + q->value(1).toDateTime().toString("dd.MM.yyyy hh.mm")
			+ " - " + q->value(2).toString() + " - (" + q->value(3).toString() + ")";
		result.append(str);
	}
	return res;
}
