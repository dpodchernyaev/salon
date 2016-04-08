
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QTimeEdit>
#include <QComboBox>
#include <QLabel>

#include <variables.h>

#include <model/shedule_item.h>
#include <model/model_factory.h>
#include <model/item_model.h>
#include <model/shedule_model.h>

#include <QSortFilterProxyModel>

#include "shedule_widget.h"

SheduleWidget::SheduleWidget()
{
	QGridLayout* grid = new QGridLayout;

	QLabel* label = NULL;

	int row = 0;

	label = new QLabel("Вид занятий:");
	vidWidget = new QComboBox;
	vidWidget->setModel(ModelFactory::getInstance()->getModel(VID));
	grid->addWidget(label, row, 0);
	grid->addWidget(vidWidget, row++, 1);

	label = new QLabel("День недели:");
	dayWidget = new QComboBox;
	grid->addWidget(label, row, 0);
	grid->addWidget(dayWidget, row++, 1);


	QSortFilterProxyModel* coachProxy = new QSortFilterProxyModel(this);
	coachProxy->setSourceModel(ModelFactory::getInstance()->getModel(COACH));
	coachProxy->setDynamicSortFilter(true);
	coachProxy->setSortRole(SortRole);
	coachProxy->sort(0);

	label = new QLabel("Тренер:");
	coachWidget = new QComboBox;
	coachWidget->setModel(coachProxy);
	grid->addWidget(label, row, 0);
	grid->addWidget(coachWidget, row++, 1);

	label = new QLabel("Зал:");
	hallWidget = new QComboBox;
	hallWidget->setModel(ModelFactory::getInstance()->getModel(HALL));
	grid->addWidget(label, row, 0);
	grid->addWidget(hallWidget, row++, 1);

	label = new QLabel("Время начала:");
	bTimeWidget = new QTimeEdit;
	grid->addWidget(label, row, 0);
	grid->addWidget(bTimeWidget, row++, 1);

	label = new QLabel("Время завершения:");
	eTimeWidget = new QTimeEdit;
	grid->addWidget(label, row, 0);
	grid->addWidget(eTimeWidget, row++, 1);

	setLayout(grid);

	for (int i = 1; i <= 7; i++)
	{
		dayWidget->addItem(ModelFactory::getDay(i), i);
	}

	clear();
}

SheduleWidget::~SheduleWidget()
{

}

void SheduleWidget::set(Item* item)
{
	ItemWidget::set(item);
	if (item != NULL)
	{
		SheduleItem* i = (SheduleItem*)item;
		SheduleParam p = i->getParam();
		dayWidget->setCurrentIndex(p.day - 1);

		QString str = ModelFactory::getHall(p.hall_id);
		int ind = hallWidget->findText(str);
		hallWidget->setCurrentIndex(ind);

		str = ModelFactory::getCoach(p.coach_id);
		ind = coachWidget->findText(str);
		coachWidget->setCurrentIndex(ind);

		bTimeWidget->setTime(p.bTime);
		eTimeWidget->setTime(p.eTime);

		ind = vidWidget->findData(p.vid_id, KeyRole);
		vidWidget->setCurrentIndex(ind);
	}
	else
	{
		dayWidget->setCurrentIndex(-1);
		bTimeWidget->setTime(QTime());
		eTimeWidget->setTime(QTime());
		hallWidget->setCurrentIndex(-1);
		coachWidget->setCurrentIndex(-1);
		vidWidget->setCurrentIndex(-1);
	}
}

bool SheduleWidget::checkSave() const
{
	bool res = true;

	if (bTimeWidget->time().isNull() || !bTimeWidget->time().isValid())
	{
		res = false;
	}
	else if (eTimeWidget->time().isNull() || !eTimeWidget->time().isValid())
	{
		res = false;
	}
	else if (coachWidget->currentIndex() < 0)
	{
		res = false;
	}
	else if (hallWidget->currentIndex() < 0)
	{
		res = false;
	}
	else if (dayWidget->currentIndex() < 0)
	{
		res = false;
	}
	else if (vidWidget->currentIndex() < 0)
	{
		res = false;
	}

	return res;
}

void SheduleWidget::apply()
{
	if (item == NULL)
	{
		return;
	}

	SheduleItem* i = (SheduleItem*)item;
	SheduleParam p = i->getParam();

	int cId = coachWidget->itemData(coachWidget->currentIndex(), KeyRole).toInt();
	int hId = hallWidget->itemData(hallWidget->currentIndex(), KeyRole).toInt();

	p.coach_id = cId;
	p.hall_id = hId;
	p.bTime = bTimeWidget->time();
	p.eTime = eTimeWidget->time();
	p.day = dayWidget->currentIndex() + 1;
	p.vid_id = vidWidget->itemData(vidWidget->currentIndex(), KeyRole).toInt();

	i->setParam(p);
}

void SheduleWidget::clear()
{
	set(NULL);
}

