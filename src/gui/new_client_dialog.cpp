
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include <model/client_item.h>
#include <gui/client_widget.h>
#include <model/client_model.h>

#include <model/coach_item.h>
#include <gui/coach_widget.h>
#include <model/coach_model.h>

#include <model/service_item.h>
#include <gui/service_widget.h>
#include <model/service_model.h>

#include "new_client_dialog.h"

QString NewClientDialog::getErrorStr() const
{
	return "Такой клиент уже существует";
}


NewClientDialog::NewClientDialog(ClientModel* model) : model(model)
{
	btnBox = new QHBoxLayout;

	item = new ClientItem;
	item->setModel(model);
	widget = new ClientWidget;
	widget->set(item);
	setLayout(construct());
}

NewClientDialog::~NewClientDialog()
{
}

QVBoxLayout* NewClientDialog::construct()
{
	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addWidget(widget);

	saveBtn = new QPushButton("Сохранить");
	cancelBtn = new QPushButton("Отмена");
	btnBox->addStretch(1);
	btnBox->addWidget(saveBtn);
	btnBox->addWidget(cancelBtn);

	vbox->addLayout(btnBox);

	connect(saveBtn, SIGNAL(clicked(bool)), this, SLOT(save()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(cancel()));

	return vbox;
}

void NewClientDialog::save()
{
	widget->apply();
	if (Item::getItem(item->hash()) == NULL)
	{
		widget->save();
		close();
	}
	else
	{
		QMessageBox::warning(this, "Ошибка", getErrorStr());
	}
}

void NewClientDialog::cancel()
{
	delete item;
	close();
}













