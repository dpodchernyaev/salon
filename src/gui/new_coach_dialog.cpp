
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QListView>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include <gui/coach_widget.h>
#include <model/coach_model.h>
#include <model/coach_item.h>

#include "new_coach_dialog.h"

NewCoachDialog::NewCoachDialog(CoachModel* model) : NewItemDialog(model)
{
	item = NULL;

	setWindowTitle("Новый тренер");

	widget = new CoachWidget;
	rightBox->addWidget(widget);
	rightBox->addStretch(1);
}

bool NewCoachDialog::save()
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

Item* NewCoachDialog::createItem()
{
	return new CoachItem;
}

void NewCoachDialog::setItem(Item *i)
{
	NewItemDialog::setItem(i);
	widget->set(i);
}

void NewCoachDialog::clear()
{
	widget->clear();
}
