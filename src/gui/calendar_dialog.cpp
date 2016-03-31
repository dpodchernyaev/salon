
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <model/item_proxy_model.h>
#include <model/model_factory.h>
#include <model/shedule_model.h>
#include <gui/item_list_view.h>

#include <gui/calendar_widget.h>

#include "calendar_dialog.h"

CalendarDialog::CalendarDialog()
{
	SheduleModel* sModel = (SheduleModel*)ModelFactory::getInstance()->getModel(SHEDULE);
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

	QHBoxLayout* btnbox = new QHBoxLayout;
	btnbox->addStretch(1);
	btnbox->addWidget(okBtn);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(calendar);
	hbox->addWidget(dayWidget);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(hbox);
	vbox->addLayout(btnbox);
	setLayout(vbox);

	connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(activated(QDate)));
	connect(okBtn, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
}

CalendarDialog::~CalendarDialog()
{

}

void CalendarDialog::okClicked()
{
	close();
}

void CalendarDialog::activated(QDate date)
{
	sProxy->setFilterDate(date);
	sProxy->invalidate();
	dayWidget->setColumnHidden(0, true);
	dayWidget->setColumnHidden(5, true);
}
