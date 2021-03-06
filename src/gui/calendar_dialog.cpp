
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

#include <model/item_proxy_model.h>
#include <model/model_factory.h>
#include <model/shedule_model.h>
#include <model/group_model.h>
#include <model/vid_model.h>

#include <model/group_item.h>

#include <model/client_service_item.h>

#include <gui/item_list_view.h>

#include <gui/calendar_widget.h>

#include "calendar_dialog.h"

CalendarDialog::CalendarDialog()
{
	setWindowTitle("Запись на занятие");
	setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
	setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setStyleSheet("QDialog { background:rgb(255,253,208); }");

	vModel = (VidModel*)ModelFactory::getInstance()->getModel(VID);
	gModel = (GroupModel*)ModelFactory::getInstance()->getModel(GROUP);

	QLabel* label = new QLabel("Вид: ");
	vidWidget = new QComboBox;
	vidWidget->setModel(vModel);

	QHBoxLayout* vidBox = new QHBoxLayout;
	vidBox->addWidget(label);
	vidBox->addWidget(vidWidget);
	vidBox->addStretch(1);

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
	vbox->addLayout(vidBox);
	vbox->addLayout(hbox);
	vbox->addLayout(btnbox);
	setLayout(vbox);

	vidWidget->setCurrentIndex(-1);

	connect(vidWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(vidChanged(int)));
	connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(activated(QDate)));
	connect(okBtn, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
	connect(exitBtn, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

CalendarDialog::~CalendarDialog()
{
}

void CalendarDialog::vidChanged(int ind)
{
	int vidId = vidWidget->itemData(ind, KeyRole).toInt();
	sProxy->setFilterVid(vidId);
	sProxy->invalidate();

	calendar->setVidFilter(vidId);
}

QDate CalendarDialog::getDate() const
{
	return calendar->selectedDate();
}

void CalendarDialog::setFilterItem(CsItem *item)
{
	int indx = vidWidget->findData(item->getParam().vid_id, KeyRole);
	vidWidget->setCurrentIndex(indx);
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
			gModel->getItems(date,
				vidWidget->itemData(vidWidget->currentIndex(), KeyRole).toInt());

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
		sp.disabled = gModel->isFull(item);

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
