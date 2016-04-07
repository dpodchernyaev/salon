

#include <model/card_model.h>
#include <model/coach_model.h>
#include <model/hall_model.h>
#include <model/service_model.h>
#include <model/client_model.h>
#include <model/cs_model.h>
#include <model/vid_model.h>
#include <model/shedule_model.h>
#include <model/group_model.h>
#include <model/visit_model.h>

#include <model/client_service_item.h>
#include <model/hall_item.h>
#include <model/coach_item.h>
#include <model/vid_item.h>
#include <model/group_item.h>

#include "model_factory.h"

ModelFactory* ModelFactory::inst = NULL;

ModelFactory::ModelFactory()
{
}

void ModelFactory::reload()
{
	models.insert(SERVICE, new ServiceModel);
	models.insert(CARD, new CardModel);
	models.insert(COACH, new CoachModel);
	models.insert(HALL, new HallModel);
	models.insert(CLIENT, new ClientModel);
	models.insert(CS, new CsModel);
	models.insert(SHEDULE, new SheduleModel);
	models.insert(GROUP, new GroupModel);
	models.insert(VISIT, new VisitModel);
	models.insert(VID, new VidModel);

	getModel(SERVICE)->fetch();
	getModel(CARD)->fetch();
	getModel(COACH)->fetch();
	getModel(HALL)->fetch();
	getModel(CLIENT)->fetch();
	getModel(SHEDULE)->fetch();
	getModel(GROUP)->fetch();
	getModel(VID)->fetch();
}

ModelFactory *ModelFactory::getInstance()
{
	if (inst == NULL)
	{
		inst = new ModelFactory;
		inst->reload();
	}
	return inst;
}

ItemModel *ModelFactory::getModel(ModelType type) const
{
	return models.value(type);
}

bool ModelFactory::isFull(const QDate &date, int vidId)
{
	bool res = true;
	SheduleModel* smodel = (SheduleModel*)getInstance()->getModel(SHEDULE);
	GroupModel* gmodel = (GroupModel*)getInstance()->getModel(GROUP);

	QList<GroupItem*> gitems = gmodel->getItems(date, vidId);
	QList<SheduleItem*> sitems = smodel->getItems(date.dayOfWeek(), vidId);

	if (gitems.size() != sitems.size())
	{
		res = false;
	}
	else
	{
		Q_FOREACH (GroupItem* i, gitems)
		{
			if (gmodel->isFull(i) == false)
			{
				res = false;
			}
		}
	}

	return res;
}

QString ModelFactory::getVid(int id)
{
	QString res;
	VidModel* model = (VidModel*)ModelFactory::getInstance()->getModel(VID);
	Item* i = model->getItem(id);
	if (i == NULL)
	{
		res = "Нет";
	}
	else
	{
		VidItem* csi = (VidItem*)i;
		res = csi->getParam().name;
	}
	return res;
}

QString ModelFactory::getService(int id)
{
	QString res;
	CsModel* model = (CsModel*)ModelFactory::getInstance()->getModel(CS);
	Item* i = model->getItem(id);
	if (i == NULL)
	{
		res = "Нет";
	}
	else
	{
		CsItem* csi = (CsItem*)i;
		res = csi->getParam().name;
	}
	return res;
}

QString ModelFactory::getDay(int id)
{
	QString res = "Ошибка";
	if (id == 1)
	{
		res = "Понедельник";
	}
	else if (id == 2)
	{
		res = "Вторник";
	}
	else if (id == 3)
	{
		res = "Среда";
	}
	else if (id == 4)
	{
		res = "Четверг";
	}
	else if (id == 5)
	{
		res = "Пятница";
	}
	else if (id == 6)
	{
		res = "Суббота";
	}
	else if (id == 7)
	{
		res = "Воскресение";
	}
	return res;
}

QString ModelFactory::getHallByGroup(int id)
{
	QString res = "Нет";
	ItemModel* model = ModelFactory::getInstance()->getModel(GROUP);
	Item* i = model->getItem(id);
	if (i != NULL)
	{
		GroupItem* g = (GroupItem*)i;
		res = getHall(g->getParam().hall_id);
	}
	return res;
}

int ModelFactory::getHallCnt(int id)
{
	Q_ASSERT (id > 0);
	if (id <= 0)
	{
		return 0;
	}

	HallModel* model = (HallModel*)ModelFactory::getInstance()->getModel(HALL);
	Item* item = model->getItem(id);
	if (item == NULL)
	{
		return 0;
	}
	else
	{
		HallItem* i = (HallItem*)item;
		return i->getCnt();
	}
}

QString ModelFactory::getHall(int id)
{
	QString res;
	if (id == 0)
	{
		res = "Удалено";
		return res;
	}

	HallModel* model = (HallModel*)ModelFactory::getInstance()->getModel(HALL);
	Item* item = model->getItem(id);
	if (item == NULL)
	{
		res = "Удалено";
	}
	else
	{
		HallItem* i = (HallItem*)item;
		res = i->getName();
	}
	return res;
}

QString ModelFactory::getCoach(int id)
{
	QString res;
	if (id == 0)
	{
		res = "Удалено";
		return res;
	}

	CoachModel* model = (CoachModel*)ModelFactory::getInstance()->getModel(COACH);
	Item* item = model->getItem(id);
	if (item == NULL)
	{
		res = "Удалено";
	}
	else
	{
		CoachItem* i = (CoachItem*)item;
		res = i->getName();
	}
	return res;
}
