
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QListView>
#include <QSortFilterProxyModel>

#include <gui/service_widget.h>
#include <model/service_model.h>
#include <model/service_item.h>

#include "new_service_dialog.h"


NewServiceDialog::NewServiceDialog(ServiceModel* model) : NewItemDialog(model)
{
	item = NULL;

	setWindowTitle("Новая услуга");

	widget = new ServiceWidget;
	rightBox->addWidget(widget);
	rightBox->addStretch(1);
}

void NewServiceDialog::save()
{
	if (widget->checkSave())
	{
		widget->apply();
		widget->save();
		NewItemDialog::save();
	}
	else
	{
		QMessageBox::warning(NULL, "Предупреждение", "Не все поля введены верно");
	}
}

Item* NewServiceDialog::createItem()
{
	return new ServiceItem;
}

void NewServiceDialog::setItem(Item *i)
{
	NewItemDialog::setItem(i);
	widget->set(i);
}

void NewServiceDialog::clear()
{
	widget->clear();
}
