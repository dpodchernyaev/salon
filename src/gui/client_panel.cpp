
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <model/model_factory.h>
#include <model/client_model.h>
#include <model/card_model.h>
#include <model/hall_model.h>
#include <model/coach_model.h>
#include <model/service_model.h>
#include <model/client_proxy_model.h>
#include <gui/client_list_view.h>
#include <gui/new_client_dialog.h>
#include <gui/new_service_dialog.h>
#include <gui/new_card_dialog.h>
#include <gui/new_coach_dialog.h>
#include <gui/new_hall_dialog.h>
#include <gui/client_info_panel.h>
#include <gui/client_search_widget.h>

#include "client_panel.h"

ClientPanel::ClientPanel()
{
	serviceModel = (ServiceModel*)ModelFactory::getInstance()->getModel(SERVICE);
	cardModel = (CardModel*)ModelFactory::getInstance()->getModel(CARD);
	coachModel = (CoachModel*)ModelFactory::getInstance()->getModel(COACH);
	hallModel = (HallModel*)ModelFactory::getInstance()->getModel(HALL);

	view = new ClientListView;
	view->setMaximumWidth(400);
	view->setMinimumWidth(300);

	searchWidget = new ClientSearchWidget(view);
	infoWidget = new ClientInfoPanel;

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addWidget(view);
	vbox->addWidget(searchWidget);

	QHBoxLayout* hbox = new QHBoxLayout(this);
	hbox->addLayout(vbox);
	hbox->addWidget(infoWidget);
	hbox->addStretch(1);

	QWidget* mainWidget = new QWidget;
	mainWidget->setLayout(hbox);
	setCentralWidget(mainWidget);

	connect(view->getSourceModel(), SIGNAL(lock(bool)),
			this, SLOT(clientLocked(bool)));
	clientLocked(true);

	view->getSourceModel()->fetch();

	connect(view, SIGNAL(currentChanged(ClientItem*)),
			this, SLOT(selected(ClientItem*)));

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


	connect(view->getSourceModel(), SIGNAL(modelRestored()),
			this, SLOT(modelRestored()));
}

ClientPanel::~ClientPanel()
{

}

void ClientPanel::modelRestored()
{
	QModelIndex first = view->getProxyModel()->index(0, 0);
	view->setCurrentIndex(first);
}

void ClientPanel::newClient()
{
	NewClientDialog dlg(view->getSourceModel());
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
	searchWidget->setEnabled(!locked);
	searchWidget->clearEditor();

	if (locked == true)
	{
		view->showAnimation();
	}
	else
	{
		view->hideAnimation();
	}
}

void ClientPanel::selected(ClientItem* item)
{
	infoWidget->setItem(item);
}
