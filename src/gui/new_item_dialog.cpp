
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QListView>

#include <gui/item_list_widget.h>
#include <gui/item_list_view.h>
#include <model/item_model.h>
#include <model/item_proxy_model.h>
#include <model/item.h>

#include "new_item_dialog.h"

NewItemDialog::NewItemDialog(ItemModel *model) : model(model)
{
	item = NULL;
	view = new ItemListWidget(model);
	view->setMaximumWidth(300);

	saveBtn = new QPushButton;
	saveBtn->setIcon(QIcon("pics/save.png"));
	saveBtn->setToolTip("Сохранить");

	exitBtn = new QPushButton;
	exitBtn->setIcon(QIcon("pics/exit.png"));
	exitBtn->setToolTip("Выйти");

	addBtn = new QPushButton;
	addBtn->setIcon(QIcon("pics/add.png"));
	addBtn->setToolTip("Добавить");

	editBtn = new QPushButton;
	editBtn->setIcon(QIcon("pics/edit.png"));
	editBtn->setToolTip("Редактировать");

	delBtn = new QPushButton;
	delBtn->setIcon(QIcon("pics/del.png"));
	delBtn->setToolTip("Удалить");

	cancelBtn = new QPushButton;
	cancelBtn->setIcon(QIcon("pics/stop_edit.png"));
	cancelBtn->setToolTip("Отмена");

	btnBox = new QHBoxLayout;
	btnBox->addStretch(1);
	btnBox->addWidget(editBtn);
	btnBox->addWidget(cancelBtn);
	btnBox->addWidget(addBtn);
	btnBox->addWidget(delBtn);
	btnBox->addWidget(saveBtn);
	btnBox->addWidget(exitBtn);

	QHBoxLayout* hbox = new QHBoxLayout;

	leftBox = new QVBoxLayout;
	leftBox->addWidget(view);

	QVBoxLayout* privateRightBox = new QVBoxLayout;
	rightBox = new QVBoxLayout;
	rightWidget = new QWidget;
	rightWidget->setLayout(rightBox);

	privateRightBox->addWidget(rightWidget);
	privateRightBox->addLayout(btnBox);

	hbox->addLayout(leftBox);
	hbox->addLayout(privateRightBox);
	setLayout(hbox);

	QItemSelectionModel* smodel = view->getView()->selectionModel();
	connect(smodel,
			SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
			this,
			SLOT(selectionChanged(const QItemSelection &, const QItemSelection &)));

	connect(addBtn, SIGNAL(clicked(bool)), this, SLOT(add()));
	connect(delBtn, SIGNAL(clicked(bool)), this, SLOT(del()));
	connect(saveBtn, SIGNAL(clicked(bool)), this, SLOT(save()));
	connect(editBtn, SIGNAL(clicked(bool)), this, SLOT(edit()));
	connect(exitBtn, SIGNAL(clicked(bool)), this, SLOT(exit()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(cancel()));

	connect(model, SIGNAL(lock(bool)), this, SLOT(modelLocked(bool)));

	editBtn->setEnabled(false);
	saveBtn->setEnabled(false);
	addBtn->setEnabled(true);
	delBtn->setEnabled(false);
	exitBtn->setEnabled(true);
	cancelBtn->setEnabled(false);
	rightWidget->setEnabled(false);
}

void NewItemDialog::modelLocked(bool locked)
{
	setEnabled(!locked);

	if (locked == true)
	{
		if (item != NULL)
		{
			QModelIndex ind = model->getIndex(item);
			view->setCurrentIndex(ind);
		}
	}
}

void NewItemDialog::selectionChanged(const QItemSelection &selected,
										const QItemSelection &deselected)
{
	Q_UNUSED (deselected);
	QModelIndexList lst = selected.indexes();
	QModelIndex ind;
	if (lst.isEmpty())
	{
		editBtn->setEnabled(false);
		saveBtn->setEnabled(false);
		addBtn->setEnabled(true);
		delBtn->setEnabled(false);
		exitBtn->setEnabled(true);
		cancelBtn->setEnabled(false);
		rightWidget->setEnabled(false);
	}
	else
	{
		ind = lst.first();
		editBtn->setEnabled(true);
		saveBtn->setEnabled(false);
		addBtn->setEnabled(true);
		delBtn->setEnabled(true);
		exitBtn->setEnabled(true);
		cancelBtn->setEnabled(false);
		rightWidget->setEnabled(false);
	}

	QModelIndex sind = view->getView()->getProxyModel()->mapToSource(ind);
	Item* i = model->getItem(sind);
	setItem(i);
}

void NewItemDialog::add()
{
	editBtn->setEnabled(false);
	saveBtn->setEnabled(true);
	addBtn->setEnabled(false);
	delBtn->setEnabled(true);
	exitBtn->setEnabled(false);
	cancelBtn->setEnabled(true);
	rightWidget->setEnabled(true);
	view->setEnabled(false);

	view->getView()->selectionModel()->blockSignals(true);
	view->getView()->selectionModel()->clearSelection();
	view->getView()->selectionModel()->blockSignals(false);

	Item* i = createItem();
	i->setModel(model);
	setItem(i);
}

void NewItemDialog::del()
{
	editBtn->setEnabled(false);
	saveBtn->setEnabled(false);
	addBtn->setEnabled(true);
	delBtn->setEnabled(false);
	exitBtn->setEnabled(true);
	cancelBtn->setEnabled(false);
	rightWidget->setEnabled(false);
	view->setEnabled(true);

//	clear();
	deleteItem();
}

void NewItemDialog::deleteItem()
{
	if (model->indexOf(item) == -1)
	{
		delete item;
		item = NULL;
		view->getView()->clearSelection();
	}
	else
	{
		Item* i = item;
		item = NULL;
		model->deleteItem(i);
	}
}

void NewItemDialog::exit()
{
	clear();
	close();
}

void NewItemDialog::edit()
{
	editBtn->setEnabled(false);
	saveBtn->setEnabled(true);
	addBtn->setEnabled(false);
	delBtn->setEnabled(false);
	exitBtn->setEnabled(false);
	cancelBtn->setEnabled(true);
	rightWidget->setEnabled(true);
	view->setEnabled(false);
}

bool NewItemDialog::save()
{
	editBtn->setEnabled(true);
	saveBtn->setEnabled(false);
	addBtn->setEnabled(true);
	delBtn->setEnabled(true);
	exitBtn->setEnabled(true);
	cancelBtn->setEnabled(false);
	rightWidget->setEnabled(false);
	view->setEnabled(true);

	QModelIndex ind = model->getIndex(item);
	QModelIndex pind = view->getView()->getProxyModel()->mapFromSource(ind);
	view->setCurrentIndex(ind);
	view->getView()->scrollTo(pind);
	return true;
}

void NewItemDialog::cancel()
{
	editBtn->setEnabled(true);
	saveBtn->setEnabled(false);
	addBtn->setEnabled(true);
	exitBtn->setEnabled(true);
	cancelBtn->setEnabled(false);
	rightWidget->setEnabled(false);
	view->setEnabled(true);
	delBtn->setEnabled(!view->getView()->selectionModel()->selectedIndexes().isEmpty());

	if (item != NULL)
	{
		if (item->getId() == 0)
		{
			editBtn->setEnabled(false);
			del();
		}
	}

	setItem(item);
}

void NewItemDialog::setItem(Item* i)
{
	this->item = i;
}
