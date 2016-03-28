
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QListView>

#include <model/item_model.h>
#include <model/item.h>

#include "new_item_dialog.h"

NewItemDialog::NewItemDialog(ItemModel *model)
{
	pmodel = new QSortFilterProxyModel;
	pmodel->setSourceModel(model);
	pmodel->setDynamicSortFilter(true);
	pmodel->sort(0);

	view = new QListView;
	view->setModel(pmodel);
	view->setMaximumWidth(200);

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

	QItemSelectionModel* smodel = view->selectionModel();
	connect(smodel, SIGNAL(currentChanged(QModelIndex, QModelIndex)),
			this, SLOT(currentChanged(QModelIndex)));

	connect(addBtn, SIGNAL(clicked(bool)), this, SLOT(add()));
	connect(delBtn, SIGNAL(clicked(bool)), this, SLOT(del()));
	connect(saveBtn, SIGNAL(clicked(bool)), this, SLOT(save()));
	connect(editBtn, SIGNAL(clicked(bool)), this, SLOT(edit()));
	connect(exitBtn, SIGNAL(clicked(bool)), this, SLOT(exit()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(cancel()));

	currentChanged(QModelIndex());
}

void NewItemDialog::currentChanged(QModelIndex index)
{
	QModelIndex sind = pmodel->mapToSource(index);
	if (sind.isValid())
	{
		editBtn->setEnabled(true);
		saveBtn->setEnabled(false);
		addBtn->setEnabled(true);
		delBtn->setEnabled(true);
		exitBtn->setEnabled(true);
		cancelBtn->setEnabled(false);
		rightWidget->setEnabled(false);
	}
	else
	{
		editBtn->setEnabled(false);
		saveBtn->setEnabled(false);
		addBtn->setEnabled(true);
		delBtn->setEnabled(false);
		exitBtn->setEnabled(true);
		cancelBtn->setEnabled(false);
		rightWidget->setEnabled(false);
	}
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
}

void NewItemDialog::exit()
{
	close();
}

void NewItemDialog::edit()
{
	editBtn->setEnabled(false);
	saveBtn->setEnabled(true);
	addBtn->setEnabled(false);
	delBtn->setEnabled(false);
	exitBtn->setEnabled(true);
	cancelBtn->setEnabled(true);
	rightWidget->setEnabled(true);
}

void NewItemDialog::save()
{
	editBtn->setEnabled(false);
	saveBtn->setEnabled(false);
	addBtn->setEnabled(true);
	delBtn->setEnabled(true);
	exitBtn->setEnabled(true);
	cancelBtn->setEnabled(false);
	rightWidget->setEnabled(false);
}

void NewItemDialog::cancel()
{
	editBtn->setEnabled(false);
	saveBtn->setEnabled(false);
	addBtn->setEnabled(true);
	delBtn->setEnabled(true);
	exitBtn->setEnabled(true);
	cancelBtn->setEnabled(false);
	rightWidget->setEnabled(false);
}
