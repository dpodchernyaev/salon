#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QListView>
#include <QMessageBox>

#include <gui/shedule_widget.h>
#include <model/shedule_model.h>
#include <model/shedule_item.h>

#include <gui/item_list_widget.h>
#include <gui/item_list_view.h>

#include "new_shedule_dialog.h"

NewSheduleDialog::NewSheduleDialog(SheduleModel* model) : NewItemDialog(model)
{
	item = NULL;

	setWindowTitle("Новое расписание");

	view->getView()->setModelColumn(5);
	view->setMinimumWidth(400);

	widget = new SheduleWidget;
	rightBox->addWidget(widget);
	rightBox->addStretch(1);
}

bool NewSheduleDialog::save()
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

Item* NewSheduleDialog::createItem()
{
	return new SheduleItem;
}

void NewSheduleDialog::setItem(Item *i)
{
	NewItemDialog::setItem(i);
	widget->set(i);
}

void NewSheduleDialog::clear()
{
	widget->clear();
}
