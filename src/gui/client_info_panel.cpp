
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

#include <model/client_item.h>
#include <gui/client_widget.h>

#include "client_info_panel.h"

ClientInfoPanel::ClientInfoPanel()
{
	clientWidget = new ClientWidget;

	saveClientBtn = new QPushButton("Сохранить");
	editClientBtn = new QPushButton("Редактировать");
	cancelClientBtn = new QPushButton("Отмена");

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(clientWidget);
	hbox->addStretch(1);

	QHBoxLayout* btnBox = new QHBoxLayout;
	btnBox->addStretch(1);
	btnBox->addWidget(saveClientBtn);
	btnBox->addWidget(cancelClientBtn);
	btnBox->addWidget(editClientBtn);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(hbox);
	vbox->addLayout(btnBox);

	vbox->addStretch(1);

	setLayout(vbox);

	connect(saveClientBtn, SIGNAL(clicked(bool)), this, SLOT(save()));
	connect(editClientBtn, SIGNAL(clicked(bool)), this, SLOT(edit()));
	connect(cancelClientBtn, SIGNAL(clicked(bool)), this, SLOT(cancel()));

	edit(false);
}

ClientInfoPanel::~ClientInfoPanel()
{

}

void ClientInfoPanel::setItem(ClientItem *item)
{
	clientWidget->set(item);
	edit(false);
}

void ClientInfoPanel::save()
{
	clientWidget->apply();
	clientWidget->save();

	edit(false);
}

void ClientInfoPanel::edit(bool editFlag)
{
	clientWidget->edit(editFlag);
	editClientBtn->setEnabled(!editFlag);
	saveClientBtn->setEnabled(editFlag);
	cancelClientBtn->setEnabled(editFlag);
}

void ClientInfoPanel::edit()
{
	edit(true);
}

void ClientInfoPanel::cancel()
{
	clientWidget->cancel();
	edit(false);
}

void ClientInfoPanel::clean()
{
	setItem(NULL);
}
