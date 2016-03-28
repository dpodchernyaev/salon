
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

void NewCoachDialog::currentChanged(QModelIndex ind)
{
	QModelIndex sind = pmodel->mapToSource(ind);
	Item* i = model->getItem(sind);
	free();
	item = (CoachItem*) i;
	widget->set(item);
}

void NewCoachDialog::save()
{
	NewItemDialog::save();
	widget->apply();
	widget->save();
	close();
}

void NewCoachDialog::free()
{
	if (item != NULL)
	{
		int id = item->getId();
		if (id <= 0)
		{
			delete item;
		}
	}
}

void NewCoachDialog::exit()
{
	NewItemDialog::exit();
	widget->cancel();
	close();
}

void NewCoachDialog::add()
{
	NewItemDialog::add();

	free();
	item = new CoachItem;
	item->setModel(model);

	widget->set(item);
}
