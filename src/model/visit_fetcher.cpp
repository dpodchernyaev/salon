#include <QDebug>


#include <unistd.h>
#include <model/group_item.h>
#include <model/visit_item.h>
#include <model/client_service_item.h>

#include <db/db_service.h>

#include <model/cs_model.h>
#include <model/group_model.h>
#include <model/model_factory.h>

#include "visit_fetcher.h"

VisitFetcher::VisitFetcher()
{
	gModel = (GroupModel*)ModelFactory::getInstance()->getModel(GROUP);
	csModel = (CsModel*)ModelFactory::getInstance()->getModel(CS);

	minDate = QDateTime::currentDateTime();
}

VisitFetcher::~VisitFetcher()
{
}

void VisitFetcher::fetchForClient(int id, const QDateTime &dtime)
{
	minDate = dtime;
	fetch(id);
}

bool VisitFetcher::deleteSlot(Item *i, DBConn *conn)
{
	QString sql = "DELETE FROM visit WHERE id = " + QString::number(i->getId());
	QSqlQuery q = conn->executeQuery(sql);

	bool res = q.isActive();

	if (res == true)
	{
		VisitItem* vis = (VisitItem*)i;
		GroupItem* g = vis->getGroup();
		CsItem* cs = vis->getCs();

		Q_ASSERT (g != NULL);
		// вычитаем из группы
		if (g != NULL)
		{
			GroupParam gp = g->getParam();
			if (gp.cnt > 0)
			{
				gp.cnt -= 1;
				g->setParam(gp);
				res = gModel->getFetcher()->saveItem(g, conn);

				if (res == false)
				{
					gp.cnt += 1;
					g->setParam(gp);
				}
			}
		}

		// вычитаем посещение из купленной услуги
		if (res == true)
		{
			if (cs != NULL)
			{
				CsParam csp = cs->getParam();
				if (csp.limit_type != LT_DATE)
				{
					csp.limit_value += 1;
					cs->setParam(csp);
					res = csModel->getFetcher()->saveItem(cs, conn);

					if (res == false)
					{
						csp.limit_value -= 1;
						cs->setParam(csp);
					}
				}
			}
		}
		vis->setCs(NULL);
		vis->setGroup(NULL);
	}
	return res;
}

bool VisitFetcher::saveSlot(Item* item, DBConn *conn)
{
	bool res = false;

//	usleep(1000000 * 1);
	VisitItem* vItem = (VisitItem*)item;
	VisitParam p = vItem->getParam();
	QString sql = "";
	GroupItem* gItem = vItem->getGroup();

	if (gItem != NULL)
	{
		GroupParam gp = gItem->getParam();
		// если нет группы, то сохранить
		gp.cnt += 1;
		gItem->setParam(gp);

		res = gModel->getFetcher()->saveSlot(gItem, conn);
		if (res == false)
		{
			gp.cnt -= 1;
			gItem->setParam(gp);
			return res;
		}

		gModel->add(gItem);
		int gid = gItem->getId();
		p.vgroup_id = gid;
		vItem->setGroup(NULL);
	}

	QSqlQuery q(conn->qtDatabase());

	bool ins = false;
	int i = 0;
	if (p.id != 0)
	{
		sql =
			"UPDATE visit"
				" SET"
					" client_service_id = ?"
					", vgroup_id = ?"
					", info = ?"
					", dtime = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, p.cs_id);
		q.bindValue(i++, p.vgroup_id);
		q.bindValue(i++, p.info);
		q.bindValue(i++, p.dtime);
		q.bindValue(i++, p.id);
	}
	else
	{
		ins = true;
		sql = "SELECT nextval('visit_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			p.id = seq.value(0).toInt();
		}
		sql =
			"INSERT INTO visit("
					" id, client_service_id, vgroup_id, info, dtime)"
				" VALUES(?, ?, ?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, p.id);
		q.bindValue(i++, p.cs_id);
		q.bindValue(i++, p.vgroup_id);
		q.bindValue(i++, p.info);
		q.bindValue(i++, p.dtime);
	}

	res = conn->executeQuery(q);
	if (res == true && ins == true)
	{
		CsItem* cs = vItem->getCs();
		if ( (res == true) && (cs != NULL) )
		{
			CsParam csp = cs->getParam();
			if (csp.limit_type != LT_DATE)
			{
				csp.limit_value -= 1;
				cs->setParam(csp);
				res = csModel->getFetcher()->saveItem(cs, conn);

				if (res == false)
				{
					csp.limit_value += 1;
					cs->setParam(csp);
				}
			}
		}
	}

	if (res == true)
	{
		vItem->setParam(p);
	}

	return res;
}

QList<Item*> VisitFetcher::fetchSlot(DBConn* conn)
{
	QList<Item*> items;

	QSqlQuery q(conn->qtDatabase());
	QString sql =
			"SELECT"
				" visit.id"
				", visit.client_service_id"
				", visit.vgroup_id"
				", visit.info"
				", visit.dtime"
			" FROM visit, client_service, client"
			" WHERE dtime >= ?"
				" AND client_service.id = visit.client_service_id"
				" AND client_service.client_id = client.id"
				" AND client.id = ?";

	q.prepare(sql);
	qDebug() << "PARAM: " << minDate << id;

	int i = 0;
	q.bindValue(i++, minDate);
	q.bindValue(i++, id);

	bool res = conn->executeQuery(q);
	while ( (res == true) && q.next())
	{
		VisitParam param;

		int i = 0;
		param.id = q.value(i++).toInt();
		param.cs_id = q.value(i++).toInt();
		param.vgroup_id = q.value(i++).toInt();
		param.info = q.value(i++).toString();
		param.dtime = q.value(i++).toDateTime();

		VisitItem* item = new VisitItem;
		item->setParam(param);
		items.append(item);
	}
	return items;
}
