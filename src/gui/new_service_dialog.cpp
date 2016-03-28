
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


NewServiceDialog::NewServiceDialog(ServiceModel* model) : model(model)
{
	item = NULL;

	pmodel = new QSortFilterProxyModel;
	pmodel->setSourceModel(model);
	pmodel->setDynamicSortFilter(true);
	pmodel->sort(0);

	widget = new ServiceWidget;
	view = new QListView;
	view->setModel(pmodel);
	view->setSelectionMode(QAbstractItemView::SingleSelection);

	saveBtn = new QPushButton("Сохранить");
	cancelBtn = new QPushButton("Отмена");
	creBtn = new QPushButton("Создать");

	QHBoxLayout* btnbox = new QHBoxLayout;
	btnbox->addStretch(1);
	btnbox->addWidget(creBtn);
	btnbox->addWidget(cancelBtn);
	btnbox->addWidget(saveBtn);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addWidget(widget);
	vbox->addLayout(btnbox);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(view);
	hbox->addLayout(vbox);
	setLayout(hbox);

	QItemSelectionModel* smodel = view->selectionModel();
	connect(smodel, SIGNAL(currentChanged(QModelIndex, QModelIndex)),
			this, SLOT(currentChanged(QModelIndex)));

	connect(creBtn, SIGNAL(clicked(bool)), this, SLOT(create()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(cancel()));
	connect(saveBtn, SIGNAL(clicked(bool)), this, SLOT(save()));
}

void NewServiceDialog::save()
{
	ServiceItem* i = (ServiceItem*)item;

	ServiceParam saved = i->get();
	widget->apply();
	ServiceParam p = i->get();


	if (p.name.isEmpty())
	{
		QMessageBox::warning(NULL, "Ошибка", "Не введено имя тренера");
		i->set(saved);
		return;
	}

	if (p.limitDays == 0 && p.limitType == LT_DATE)
	{
		QMessageBox::warning(NULL, "Ошибка", "Для этого типа необходимо"
							 "\nввести ограничения по дням");
		i->set(saved);
		return;
	}

	if (p.value == 0 && p.limitType == LT_COUNT)
	{
		QMessageBox::warning(NULL, "Ошибка", "Для этого типа необходимо"
							 "\nввести ограничения по количеству занятий");
		i->set(saved);
		return;
	}

	if ( (p.value == 0 || p.limitDays) && p.limitType == LT_DATE_COUNT)
	{
		QMessageBox::warning(NULL, "Ошибка", "Для этого типа необходимо\n"
							"ввести ограничения:\n"
							" - по количеству занятий\n"
							" - по времени");
		i->set(saved);
		return;
	}

	if (Item::getItem(i->hash()) != NULL && i->getId() <= 0)
	{
		QMessageBox::warning(NULL, "Ошибка", "Такая услуга уже есть");
		i->set(saved);
		return;
	}

	widget->save();

	view->setEnabled(true);
	creBtn->setEnabled(true);
	close();
}

void NewServiceDialog::free()
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

void NewServiceDialog::create()
{
	free();

	ServiceItem* i = new ServiceItem;
	i->setModel(model);
	widget->set(i);
	item = i;

	view->setEnabled(false);
	creBtn->setEnabled(false);
}

void NewServiceDialog::cancel()
{
	widget->clear();
	close();
}

void NewServiceDialog::currentChanged(QModelIndex ind)
{
	free();
	QModelIndex sind = pmodel->mapToSource(ind);
	ServiceItem* i = (ServiceItem*)model->getItem(sind);
	widget->set(i);
	item = i;
}
