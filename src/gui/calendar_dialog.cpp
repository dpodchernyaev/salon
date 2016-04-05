
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <model/item_proxy_model.h>
#include <model/model_factory.h>
#include <model/shedule_model.h>
#include <model/group_model.h>

#include <model/group_item.h>

#include <model/client_service_item.h>

#include <gui/item_list_view.h>

#include <gui/calendar_widget.h>

#include "calendar_dialog.h"

CalendarDialog::CalendarDialog()
{
	setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
	setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	gModel = (GroupModel*)ModelFactory::getInstance()->getModel(GROUP);

	sModel = (SheduleModel*)ModelFactory::getInstance()->getModel(SHEDULE);
	sProxy = new SheduleProxyModel(sModel);
	sProxy->setDynamicSortFilter(true);
	sProxy->setSortRole(TimeSortRole);
	sProxy->sort(0);

	dayWidget = new ItemTableView(sModel);
	dayWidget->setProxyModel(sProxy);
	dayWidget->setColumnHidden(0, true);
	dayWidget->setColumnHidden(5, true);
	dayWidget->setMinimumWidth(500);
	dayWidget->horizontalHeader()->setStretchLastSection(true);
	dayWidget->verticalHeader()->hide();
	dayWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	dayWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	dayWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

	calendar = new CalendarWidget;
	calendar->setFixedWidth(300);
	calendar->setMinimumHeight(300);
	calendar->setMinimumDate(QDate::currentDate());
	calendar->setMaximumDate(QDate::currentDate().addMonths(2));
	calendar->setFirstDayOfWeek(Qt::Monday);

	calendar->setSelectedDate(QDate::currentDate());
	sProxy->setFilterDate(QDate::currentDate());
	sProxy->invalidate();

	okBtn = new QPushButton;
	okBtn->setIcon(QIcon("pics/use.png"));
	okBtn->setToolTip("Записать");

	QPushButton* exitBtn = new QPushButton;
	exitBtn->setIcon(QIcon("pics/exit.png"));
	exitBtn->setToolTip("Закрыть");

	QHBoxLayout* btnbox = new QHBoxLayout;
	btnbox->addStretch(1);
	btnbox->addWidget(okBtn);
	btnbox->addWidget(exitBtn);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(calendar);
	hbox->addWidget(dayWidget);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(hbox);
	vbox->addLayout(btnbox);
	setLayout(vbox);

	connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(activated(QDate)));
	connect(okBtn, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
	connect(exitBtn, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

CalendarDialog::~CalendarDialog()
{

}

QDate CalendarDialog::getDate() const
{
	return calendar->selectedDate();
}

void CalendarDialog::setFilterItem(CsItem *item)
{
	filterItem = item;
	sProxy->setFilterVid(item->getParam().vid_id);
	sProxy->invalidate();

	calendar->setVidFilter(item->getParam().vid_id);
}

SheduleItem* CalendarDialog::getSheduleItem() const
{
	SheduleItem* res = NULL;

	QItemSelectionModel* selModel = dayWidget->selectionModel();
	QModelIndexList indLst = selModel->selectedIndexes();
	if (indLst.isEmpty())
	{
		return res;
	}
	QModelIndex proxyInd = indLst.first();
	QModelIndex sInd = sProxy->mapToSource(proxyInd);
	Item* item = sModel->getItem(sInd);
	SheduleItem* sItem = (SheduleItem*)item;
	return sItem;
}

void CalendarDialog::okClicked()
{
	accept();
}

void CalendarDialog::exitClicked()
{
	reject();
}

void CalendarDialog::activated(QDate date)
{
	sProxy->flushSheduleParam();
	sProxy->setFilterDate(date);
	dayWidget->setColumnHidden(0, true);
	dayWidget->setColumnHidden(5, true);


	QList<GroupItem*> items =
			gModel->getItems(date, filterItem->getParam().vid_id);

	Q_FOREACH (GroupItem* item, items)
	{
		GroupParam p = item->getParam();
		int hallId = p.hall_id;
		int day = date.dayOfWeek();
		QTime time = p.bdtime.time();

		SheduleProxyModel::Param sp;
		sp.day = day;
		sp.time = time;
		sp.hallId = hallId;
		sp.disabled = !gModel->isActive(item);

		if (sp.disabled == true)
		{
			sp.color = Qt::red;
		}
		else
		{
			sp.color = Qt::green;
		}
		sProxy->addParam(sp);
	}

	sProxy->invalidate();
}
