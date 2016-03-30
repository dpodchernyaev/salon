
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

NewCardDialog::NewCardDialog(CardModel* model) : NewItemDialog(model)
{
	item = NULL;

	setWindowTitle("Карты");

	widget = new CardWidget;
	rightBox->addWidget(widget);
	rightBox->addStretch(1);
}

bool NewCardDialog::save()
{
	bool res = false;
	if (widget->checkSave())
	{
		res = NewItemDialog::save();
		if (res == true)
		{
			widget->apply();
			widget->save();
			widget->clear();
		}
	}
	else
	{
		QMessageBox::warning(NULL, "Предупреждение", "Не все поля введены верно");
	}
	return res;
}

Item* NewCardDialog::createItem()
{
	return new CardItem;
}

void NewCardDialog::setItem(Item *i)
{
	NewItemDialog::setItem(i);
	widget->set(i);
}

void NewCardDialog::clear()
{
	widget->clear();
}
