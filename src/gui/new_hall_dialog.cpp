
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QListView>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include <gui/hall_widget.h>
#include <model/hall_model.h>
#include <model/hall_item.h>

#include "new_hall_dialog.h"











NewHallDialog::NewHallDialog(HallModel* model) : NewItemDialog(model)
{
	item = NULL;

	setWindowTitle("Залы");

	widget = new HallWidget;
	rightBox->addWidget(widget);
	rightBox->addStretch(1);
}

bool NewHallDialog::save()
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

Item* NewHallDialog::createItem()
{
	return new HallItem;
}

void NewHallDialog::setItem(Item *i)
{
	NewItemDialog::setItem(i);
	widget->set(i);
}

void NewHallDialog::clear()
{
	widget->clear();
}
