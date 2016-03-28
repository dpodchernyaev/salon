
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

	setWindowTitle("Новый карта");

	widget = new CardWidget;
	rightBox->addWidget(widget);
	rightBox->addStretch(1);
}

void NewCardDialog::save()
{
	if (widget->checkSave())
	{
		widget->apply();
		widget->save();
		widget->clear();
		NewItemDialog::save();
	}
	else
	{
		QMessageBox::warning(NULL, "Предупреждение", "Не все поля введены верно");
	}
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
