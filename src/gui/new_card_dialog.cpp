
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QListView>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include <gui/card_widget.h>
#include <model/card_model.h>
#include <model/card_item.h>

#include "new_card_dialog.h"

NewCardDialog::NewCardDialog(CardModel* model) : model(model)
{
	item = NULL;

	setWindowTitle("Новая карта");

	widget = new CardWidget;

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

void NewCardDialog::currentChanged(QModelIndex ind)
{
	QModelIndex sind = pmodel->mapToSource(ind);
	Item* i = model->getItem(sind);
	free();
	item = (CardItem*) i;
	widget->set(item);
}

void NewCardDialog::save()
{
	if (item == NULL)
	{
		return;
	}

	QString sname = item->getName();
	int scnt = item->getDiscont();

	widget->apply();

	QString name = item->getName();
	int cnt = item->getDiscont();

	if (name.isEmpty() || cnt == 0)
	{
		QMessageBox::warning(NULL, "Предупреждение",
					"Неободимо ввести название карты");
		item->setName(sname);
		item->setDiscont(scnt);
		return;
	}

	if (item->getId() <= 0 && Item::getItem(item->hash()) != NULL)
	{
		QMessageBox::warning(NULL, "Предупреждение",
					"Такая карта уже присутствует");
		item->setName(sname);
		item->setDiscont(scnt);
		return;
	}

	widget->save();
	close();
}

void NewCardDialog::free()
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

void NewCardDialog::cancel()
{
	widget->cancel();
	close();
}

void NewCardDialog::create()
{
	newBtn->setEnabled(false);
	view->setEnabled(false);

	free();
	item = new CardItem;
	item->setModel(model);

	widget->set(item);
}
