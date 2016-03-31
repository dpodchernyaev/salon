
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

#include <model/card_item.h>
#include <model/service_item.h>
#include <model/cs_model.h>
#include <model/client_service_item.h>
#include <model/card_model.h>
#include <model/model_factory.h>
#include <model/client_item.h>
#include <gui/client_widget.h>

#include "client_info_panel.h"

ClientInfoPanel::ClientInfoPanel()
{	
	clientWidget = new ClientWidget;

	delBtn = new DoublePushButton;
	delBtn->setIcon(QIcon("pics/del.png"));
	delBtn->setToolTip("Удалить купленное (двойной клик)");
	buyBtn = new QPushButton;
	buyBtn->setIcon(QIcon("pics/buy.png"));
	buyBtn->setToolTip("Купить новую услугу");
	useBtn = new QPushButton;
	useBtn->setIcon(QIcon("pics/use.png"));
	useBtn->setToolTip("Оформить посещение");

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

	csModel = (CsModel*)ModelFactory::getInstance()->getModel(CS);
	serviceView = new ItemTableView(csModel);
	serviceView->horizontalHeader()->setStretchLastSection(true);
	serviceView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	serviceView->resizeColumnsToContents();
	serviceView->setSelectionMode(QAbstractItemView::SingleSelection);
	serviceView->setSelectionBehavior(QAbstractItemView::SelectRows);
	serviceView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(clientWidget);
	hbox->addStretch(1);

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

	connect(csModel, SIGNAL(lock(bool)), this, SLOT(serviceLocked(bool)));

	connect(delBtn, SIGNAL(doubleClicked()), this, SLOT(delService()));
	connect(useBtn, SIGNAL(clicked(bool)), this, SLOT(useService()));
	connect(buyBtn, SIGNAL(clicked(bool)), this, SLOT(buyService()));

	connect(serviceView, SIGNAL(currentChanged(Item*)),
			this, SLOT(serviceSelected(Item*)));
}

void ClientInfoPanel::serviceSelected(Item* item)
{
	delBtn->setEnabled(item != NULL);
	useBtn->setEnabled(item != NULL);
}

void ClientInfoPanel::serviceLocked(bool f)
{
	serviceView->setEnabled(!f);
	if (f == true)
	{
		serviceView->showAnimation();
	}
	else
	{
		CsModel* cs = (CsModel*)ModelFactory::getInstance()->getModel(CS);
		summWidget->setText(QString::number(cs->getSumm(), 'f', 2) + " руб.");
		serviceView->hideAnimation();
	}
}

ClientInfoPanel::~ClientInfoPanel()
{

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
			p.service_id = si->getId();
			p.date = QDateTime::currentDateTime();
			p.summ = si->get().price * (1 - (double)disc / 100);
			i->setParam(p);
			csModel->save(i);
		}
	}
}

void ClientInfoPanel::delService()
{
	Item* i = serviceView->getSelected();
	if (i != NULL)
	{
		csModel->deleteItem(i);
	}
	qDebug() << "DEL";
}

void ClientInfoPanel::useService()
{
	qDebug() << "USE";
}

void ClientInfoPanel::setItem(ClientItem *item)
{
	csModel->clean();
	clientWidget->set(item);
	int cId = 0;
	if (item != NULL)
	{
		cId = item->getId();
		csModel->fetchForClient(cId);

		serviceView->setEnabled(true);
		delBtn->setEnabled(serviceView->getSelected() != NULL);
		buyBtn->setEnabled(true);
		useBtn->setEnabled(serviceView->getSelected() != NULL);
	}
	else
	{
		serviceView->setEnabled(false);
		delBtn->setEnabled(false);
		buyBtn->setEnabled(false);
		useBtn->setEnabled(false);
	}
}
