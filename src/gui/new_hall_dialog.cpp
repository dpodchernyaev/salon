
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

NewHallDialog::NewHallDialog(HallModel* model) : model(model)
{
	item = NULL;

	setWindowTitle("Новый зал");

	widget = new HallWidget;

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

void NewHallDialog::currentChanged(QModelIndex ind)
{
	QModelIndex sind = pmodel->mapToSource(ind);
	Item* i = model->getItem(sind);
	free();
	item = (HallItem*) i;
	widget->set(item);
}

void NewHallDialog::save()
{
	if (item == NULL)
	{
		return;
	}

	QString sname = item->getName();
	int scnt = item->getCnt();

	widget->apply();

	QString name = item->getName();
	int cnt = item->getCnt();

	if (name.isEmpty() || cnt == 0)
	{
		QMessageBox::warning(NULL, "Предупреждение",
					"Неободимо ввести название и количество мест в зале");
		item->setName(sname);
		item->setCnt(scnt);
		return;
	}

	if (item->getId() <= 0 && Item::getItem(item->hash()) != NULL)
	{
		QMessageBox::warning(NULL, "Предупреждение",
					"Такой зал уже присутствует");
		item->setName(sname);
		item->setCnt(scnt);
		return;
	}

	widget->save();
	close();
}

void NewHallDialog::free()
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

void NewHallDialog::cancel()
{
	widget->cancel();
	close();
}

void NewHallDialog::create()
{
	newBtn->setEnabled(false);
	view->setEnabled(false);

	free();
	item = new HallItem;
	item->setModel(model);

	widget->set(item);
}
