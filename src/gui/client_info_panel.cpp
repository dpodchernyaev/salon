
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>

#include <gui/item_list_view.h>
#include <gui/buy_dialog.h>
#include <gui/calendar_dialog.h>

#include <model/item_proxy_model.h>
#include <model/cs_model.h>
#include <model/card_model.h>
#include <model/model_factory.h>
#include <model/group_model.h>
#include <model/visit_model.h>

#include <model/group_item.h>
#include <model/visit_item.h>
#include <model/card_item.h>
#include <model/service_item.h>
#include <model/shedule_item.h>
#include <model/client_service_item.h>
#include <model/client_item.h>

#include <gui/client_widget.h>

#include "client_info_panel.h"

ClientInfoPanel::ClientInfoPanel()
{	
	waitDialog = new QMessageBox("Ожидайте", "Идет обработка данных...",
									QMessageBox::Information, 0, 0, 0, this);
	waitDialog->setStandardButtons(QMessageBox::NoButton);
	clientWidget = new ClientWidget;

	delBtn = new QPushButton;
	delBtn->setIcon(QIcon("pics/del.png"));
	delBtn->setToolTip("Отмена покупки");
	buyBtn = new QPushButton;
	buyBtn->setIcon(QIcon("pics/buy.png"));
	buyBtn->setToolTip("Купить новую услугу");
	useBtn = new QPushButton;
	useBtn->setIcon(QIcon("pics/use.png"));
	useBtn->setToolTip("Оформить посещение");
	delVisBtn = new QPushButton;
	delVisBtn->setIcon(QIcon("pics/del.png"));
	delVisBtn->setToolTip("Отменить бронь");

	useBtn->setEnabled(false);

	summWidget = new QLineEdit;
	summWidget->setReadOnly(true);
	QLabel* sLabel = new QLabel("Потрачено: ");

	QHBoxLayout* btnbox = new QHBoxLayout;
	btnbox->addStretch(1);
	btnbox->addWidget(delBtn);
	btnbox->addStretch(1);
	btnbox->addWidget(sLabel);
	btnbox->addWidget(summWidget);
	btnbox->addWidget(buyBtn);
	btnbox->addWidget(useBtn);

	gModel = (GroupModel*)ModelFactory::getInstance()->getModel(GROUP);
	visModel = (VisitModel*)ModelFactory::getInstance()->getModel(VISIT);
	csModel = (CsModel*)ModelFactory::getInstance()->getModel(CS);
	CsProxyModel* csProxy = new CsProxyModel(csModel);
	csProxy->setDynamicSortFilter(true);
	csProxy->setSortRole(SortRole);
	csProxy->sort(0, Qt::DescendingOrder);

	ItemProxyModel* visitProxy = new ItemProxyModel(visModel);
	visitProxy->setDynamicSortFilter(true);
	visitProxy->setSortRole(SortRole);
	visitProxy->sort(0);

	visitView = new ItemTableView(visModel);
	visitView->setProxyModel(visitProxy);
	visitView->verticalHeader()->hide();
	visitView->horizontalHeader()->setStretchLastSection(true);
	visitView->setSelectionMode(QAbstractItemView::SingleSelection);
	visitView->setSelectionBehavior(QAbstractItemView::SelectRows);
	visitView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	serviceView = new ItemTableView(csModel);
	serviceView->horizontalHeader()->setStretchLastSection(true);
	serviceView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	serviceView->resizeColumnsToContents();
	serviceView->setSelectionMode(QAbstractItemView::SingleSelection);
	serviceView->setSelectionBehavior(QAbstractItemView::SelectRows);
	serviceView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	serviceView->setProxyModel(csProxy);

	QFont tmpFont;
	tmpFont.setBold(true);

	QLabel* tmpLabel = new QLabel("Бронь: ");
	tmpLabel->setFont(tmpFont);

	QVBoxLayout* visitBox = new QVBoxLayout;
	visitBox->addWidget(tmpLabel);
	visitBox->addWidget(visitView);
	visitBox->addWidget(delVisBtn, 0, Qt::AlignHCenter);
	visitBox->addStretch(1);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(clientWidget);
	hbox->addLayout(visitBox);
	//hbox->addStretch(1);

	QFont f;
	f.setBold(true);
	QLabel* label = new QLabel("Купленные услуги:");
	label->setFont(f);

	QVBoxLayout* serviceBox = new QVBoxLayout;
	serviceBox->addWidget(serviceView);
	serviceBox->addLayout(btnbox);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(hbox);
	vbox->addWidget(label);
	vbox->addLayout(serviceBox);

	vbox->addStretch(1);

	setLayout(vbox);

	clientWidget->edit(false);
	serviceView->hideAnimation();
	visitView->hideAnimation();
	delVisBtn->setEnabled(false);

	connect(csModel, SIGNAL(lock(bool)), this, SLOT(serviceLocked(bool)));
	connect(visModel, SIGNAL(lock(bool)), this, SLOT(visitLocked(bool)));
	connect(visModel, SIGNAL(lock(bool)), waitDialog, SLOT(accept()));

	connect(delVisBtn, SIGNAL(clicked(bool)), this, SLOT(delVisit()));
	connect(delBtn, SIGNAL(clicked(bool)), this, SLOT(delService()));
	connect(useBtn, SIGNAL(clicked(bool)), this, SLOT(useService()));
	connect(buyBtn, SIGNAL(clicked(bool)), this, SLOT(buyService()));

	connect(serviceView, SIGNAL(currentChanged(Item*)),
			this, SLOT(serviceSelected(Item*)));

	connect(visitView, SIGNAL(currentChanged(Item*)),
			this, SLOT(visitSelected(Item*)));

	upTimer.setInterval(500);

	connect(&upTimer, SIGNAL(timeout()), this, SLOT(fetch()));
}

ClientInfoPanel::~ClientInfoPanel()
{

}

void ClientInfoPanel::visitSelected(Item* item)
{
	delVisBtn->setEnabled(item != NULL);
}

void ClientInfoPanel::serviceSelected(Item* item)
{
	delBtn->setEnabled(item != NULL);
	useBtn->setEnabled(item != NULL);
}

void ClientInfoPanel::visitLocked(bool f)
{
	visitView->setEnabled(!f);
	if (f == true)
	{
		delVisBtn->setEnabled(false);
		visitView->showAnimation();
	}
	else
	{
		delVisBtn->setEnabled(visitView->getSelected() != NULL);
		visitView->resizeColumnsToContents();
		visitView->hideAnimation();
	}
}

void ClientInfoPanel::serviceLocked(bool f)
{
	serviceView->setEnabled(!f);
	if (f == true)
	{
		summWidget->setText("");
		serviceView->showAnimation();
	}
	else
	{
		CsModel* cs = (CsModel*)ModelFactory::getInstance()->getModel(CS);
		summWidget->setText(QString::number(cs->getSumm(), 'f', 2) + " руб.");
		serviceView->hideAnimation();
		serviceView->resizeColumnsToContents();
		buyBtn->setEnabled(true);
	}
}

void ClientInfoPanel::buyService()
{
	CardModel* cardModel = (CardModel*)ModelFactory::getInstance()->getModel(CARD);
	ItemModel* smodel = ModelFactory::getInstance()->getModel(SERVICE);

	ClientItem* clientItem = (ClientItem*)clientWidget->get();

	int disc = 0;
	int cardId = clientItem->get().card_id;
	if (cardId > 0)
	{
		CardItem* ca = (CardItem*)cardModel->getItem(cardId);
		disc = ca->getDiscont();
	}

	BuyDialog dlg(smodel, disc);
	dlg.exec();

	if (dlg.getStatus() == true)
	{
		if (dlg.getSelected() != NULL)
		{
			ServiceItem* si = (ServiceItem*)dlg.getSelected();
			CsItem* i = new CsItem;
			CsParam p = i->getParam();
			p.client_id = clientItem->getId();
			p.limit_value = si->get().value;
			p.limit_type = si->get().limitType;
			p.limit_days = si->get().limitDays;
			p.vid_id = si->get().vid_id;
			p.name = si->get().name;
			p.date = QDateTime::currentDateTime();
			p.summ = si->get().price * (1 - (double)disc / 100);
			i->setParam(p);
			csModel->save(i);
		}
	}
}

void ClientInfoPanel::delVisit()
{
	QMessageBox msgBox(
				QMessageBox::Question,
				"Подтверждение удаления",
				"Вы действительно хотите удалить запись?",
				QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, trUtf8("Да"));
	msgBox.setButtonText(QMessageBox::No, trUtf8("Нет"));

	int btn = msgBox.exec();

	if (btn == QMessageBox::Yes)
	{
		bool ok = false;
		Item* item = visitView->getSelected();
		VisitItem* vi = (VisitItem*)item;

		Item* i = ((ItemModel*)(gModel))->getItem(vi->getParam().vgroup_id);
		if (i > 0)
		{
			GroupItem* gi = (GroupItem*)i;
			Item* csitem = csModel->getItem(vi->getParam().cs_id);
			if (csitem != NULL)
			{
				CsItem* cs = (CsItem*)csitem;
				vi->setCs(cs);
				vi->setGroup(gi);
				visModel->deleteItem(vi);
				ok = true;
			}
		}

		if (ok == false)
		{
			QMessageBox::critical(NULL, "Ошибка", "Ошибка удаления посещения");
		}
	}
}

void ClientInfoPanel::delService()
{
	QMessageBox msgBox(
				QMessageBox::Question,
				"Подтверждение удаления",
				"Вы действительно хотите удалить запись?",
				QMessageBox::Yes | QMessageBox::No);
	msgBox.setButtonText(QMessageBox::Yes, trUtf8("Да"));
	msgBox.setButtonText(QMessageBox::No, trUtf8("Нет"));

	int btn = msgBox.exec();

	if (btn == QMessageBox::Yes)
	{
		Item* i = serviceView->getSelected();
		if (i != NULL)
		{
			csModel->deleteItem(i);
			waitDialog->exec();
		}
	}
}

void ClientInfoPanel::useService()
{
	Item* i = serviceView->getSelected();
	if (i == NULL)
	{
		return;
	}
	CsItem* csi = (CsItem*)i;
	CsParam pcsi = csi->getParam();

	bool isA = csModel->isActive(pcsi);
	if (isA == false)
	{
		return;
	}


	CalendarDialog wgt;
	wgt.setFilterItem(csi);
	bool res = wgt.exec();

	if (res == true)
	{
		QDate date = wgt.getDate();
		SheduleItem* si = wgt.getSheduleItem();
		if (si == NULL)
		{
			return;
		}
		SheduleParam psi = si->getParam();
		QDateTime dt(date, psi.bTime);

		GroupItem* gItem = gModel->getItem(dt, psi.hall_id);
		VisitItem* vi = new VisitItem;
		VisitParam vp;
		if (gItem == NULL)
		{
			gItem = new GroupItem;
			GroupParam gParam;
			gParam.coach_id = psi.coach_id;
			gParam.hall_id = psi.hall_id;
			gParam.bdtime = dt;
			gParam.etime = psi.eTime;
			gParam.vid_id = psi.vid_id;
			gItem->setParam(gParam);
		}
		else
		{
			if (visModel->getItemInGroup(gItem->getParam().id) != NULL)
			{
				delete vi;
				QMessageBox::information(NULL, "Предупреждение",
										 "Клиент уже записан на занятие");
				// запись уже присутствует
				return;
			}
			vp.vgroup_id = gItem->getId();
		}

		vp.cs_id = csi->getParam().id;
		vp.dtime = dt;
		vp.info = ModelFactory::getVid(psi.vid_id);
		vi->setParam(vp);
		vi->setGroup(gItem);
		vi->setCs(csi);

		visModel->save(vi);

		waitDialog->exec();
	}
	else
	{
		qDebug() << "Отменено";
	}
}

void ClientInfoPanel::setItem(ClientItem *item, bool reload)
{
	clientWidget->set(item);

	if (reload == true)
	{
		delBtn->setEnabled(false);
		useBtn->setEnabled(false);
		serviceView->setEnabled(false);
		buyBtn->setEnabled(false);
		serviceView->showAnimation();
		visitView->showAnimation();
		visitView->setEnabled(false);
		delVisBtn->setEnabled(false);
		summWidget->setText("");
		upTimer.start();
	}
}

void ClientInfoPanel::fetch()
{
	if (clientWidget->get() == NULL)
	{
		return;
	}

	csModel->fetchForClient(clientWidget->get()->getId());
	visModel->fetch(clientWidget->get()->getId());

	upTimer.stop();
}
