
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

NewClientDialog::NewClientDialog(ClientModel* model) : NewItemDialog(model)
{
	item = NULL;

	setWindowTitle("Клиенты");

	widget = new ClientWidget;
	rightBox->addWidget(widget);
	rightBox->addStretch(1);
}

bool NewClientDialog::save()
{
	if (widget->checkSave())
	{
		widget->apply();
		widget->save();
		NewItemDialog::save();
		return true;
	}
	else
	{
		QMessageBox::warning(NULL, "Предупреждение", "Не все поля введены верно");
		return false;
	}
}

Item* NewClientDialog::createItem()
{
	ClientItem* i = new ClientItem;
	ClientParam p = i->get();
	p.birthday = QDateTime(MIN_DATE);
	i->set(p);

	return i;
}

void NewClientDialog::setItem(Item *i)
{
	NewItemDialog::setItem(i);
	widget->set(i);
}

void NewClientDialog::clear()
{
	widget->clear();
}








