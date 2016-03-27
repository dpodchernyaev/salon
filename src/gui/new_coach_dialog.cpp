
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

NewCoachDialog::NewCoachDialog(CoachModel* model) : model(model)
{
	item = NULL;

	setWindowTitle("Новый тренер");

	widget = new CoachWidget;

	pmodel = new QSortFilterProxyModel;
	pmodel->setSourceModel(model);
	pmodel->setDynamicSortFilter(true);
	pmodel->sort(0);

	view = new QListView;
	view->setModel(pmodel);
	view->setMaximumWidth(200);

	saveBtn = new QPushButton("Сохранить");
	cancelBtn = new QPushButton("Отмена");
	newBtn = new QPushButton("Создать");

	QHBoxLayout* btnBox = new QHBoxLayout;
	QHBoxLayout* hbox = new QHBoxLayout;
	QVBoxLayout* vbox = new QVBoxLayout;

	btnBox->addStretch(1);
	btnBox->addWidget(newBtn);
	btnBox->addWidget(cancelBtn);
	btnBox->addWidget(saveBtn);

	vbox->addWidget(widget);
	vbox->addStretch(1);
	vbox->addLayout(btnBox);

	hbox->addWidget(view);
	hbox->addLayout(vbox);

	setLayout(hbox);

	QItemSelectionModel* smodel = view->selectionModel();
	connect(smodel, SIGNAL(currentChanged(QModelIndex, QModelIndex)),
			this, SLOT(currentChanged(QModelIndex)));
	connect(newBtn, SIGNAL(clicked(bool)), this, SLOT(create()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(cancel()));
	connect(saveBtn, SIGNAL(clicked(bool)), this, SLOT(save()));
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

void NewCoachDialog::cancel()
{
	widget->cancel();
	close();
}

void NewCoachDialog::create()
{
	newBtn->setEnabled(false);
	view->setEnabled(false);

	free();
	item = new CoachItem;
	item->setModel(model);

	widget->set(item);
}
