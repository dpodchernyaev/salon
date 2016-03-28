
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <model/model_factory.h>
#include <model/client_model.h>
#include <model/card_model.h>
#include <model/hall_model.h>
#include <model/coach_model.h>
#include <model/service_model.h>
#include <model/item_proxy_model.h>
#include <gui/item_list_widget.h>
#include <gui/new_client_dialog.h>
#include <gui/new_service_dialog.h>
#include <gui/new_card_dialog.h>
#include <gui/new_coach_dialog.h>
#include <gui/new_hall_dialog.h>
#include <gui/client_info_panel.h>

#include "client_panel.h"

ClientPanel::ClientPanel()
{
	serviceModel = (ServiceModel*)ModelFactory::getInstance()->getModel(SERVICE);
	cardModel = (CardModel*)ModelFactory::getInstance()->getModel(CARD);
	coachModel = (CoachModel*)ModelFactory::getInstance()->getModel(COACH);
	hallModel = (HallModel*)ModelFactory::getInstance()->getModel(HALL);
	clientModel = (ClientModel*)ModelFactory::getInstance()->getModel(CLIENT);

	view = new ItemListWidget(clientModel);
	view->setMaximumWidth(400);
	view->setMinimumWidth(300);

	infoWidget = new ClientInfoPanel;

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addWidget(view);

	QHBoxLayout* hbox = new QHBoxLayout(this);
	hbox->addLayout(vbox);
	hbox->addWidget(infoWidget);
	hbox->addStretch(1);

	QWidget* mainWidget = new QWidget;
	mainWidget->setLayout(hbox);
	setCentralWidget(mainWidget);

	connect(clientModel, SIGNAL(lock(bool)),
			this, SLOT(clientLocked(bool)));
	clientLocked(true);

	clientModel->fetch();

	connect(view, SIGNAL(currentChanged(Item*)),
			this, SLOT(selected(Item*)));

	QMenu* menu = new QMenu("Меню");
	QMenuBar* bar = menuBar();
	bar->addMenu(menu);

	QAction* action = new QAction("Новый клиент", menu);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(newClient()));
	menu->addAction(action);


	action = new QAction("Новый тренер", menu);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(newCoach()));
	menu->addAction(action);

	action = new QAction("Новая услуга", menu);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(newService()));
	menu->addAction(action);

	action = new QAction("Новый зал", menu);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(newHall()));
	menu->addAction(action);

	action = new QAction("Новая карта", menu);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(newCard()));
	menu->addAction(action);


	connect(clientModel, SIGNAL(modelRestored()),
			this, SLOT(modelRestored()));
}

ClientPanel::~ClientPanel()
{

}

void ClientPanel::modelRestored()
{
	QModelIndex first = clientModel->index(0, 0);
	view->setCurrentIndex(first);
}

void ClientPanel::newClient()
{
	NewClientDialog dlg(clientModel);
	dlg.exec();
}

void ClientPanel::newService()
{
	NewServiceDialog dlg(serviceModel);
	dlg.exec();
}

void ClientPanel::newHall()
{
	NewHallDialog dlg(hallModel);
	dlg.exec();
}

void ClientPanel::newCard()
{
	NewCardDialog dlg(cardModel);
	dlg.exec();
}

void ClientPanel::newCoach()
{
	NewCoachDialog dlg(coachModel);
	dlg.exec();
}

void ClientPanel::clientLocked(bool locked)
{
	infoWidget->setEnabled(!locked);
	view->setEnabled(!locked);
	view->showAnimation(locked);
}

void ClientPanel::selected(Item* item)
{
	ClientItem* c = (ClientItem*)item;
	infoWidget->setItem(c);
}
