
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QSortFilterProxyModel>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <model/card_item.h>
#include <model/card_model.h>

#include "card_widget.h"

CardWidget::CardWidget()
{
	QGridLayout* grid = new QGridLayout;
	int row = 0;

	QLabel* label = new QLabel("Имя: ");
	nameEdit = new QLineEdit;
	nameEdit->setMinimumWidth(300);
	grid->addWidget(label, row, 0);
	grid->addWidget(nameEdit, row++, 1);

	label = new QLabel("Скидка: ");
	discWidget = new QSpinBox;
	discWidget->setMaximum(100);
	grid->addWidget(label, row, 0);
	grid->addWidget(discWidget, row++, 1);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(grid);

	setLayout(vbox);
}

void CardWidget::set(Item* item)
{
	ItemWidget::set(item);
	if (item != NULL)
	{
		CardItem* i = (CardItem*)item;
		nameEdit->setText(i->getName());
		discWidget->setValue(i->getDiscont());
	}
	else
	{
		nameEdit->setText("");
		discWidget->setValue(0);
	}
}

void CardWidget::apply()
{
	if (item != NULL)
	{
		CardItem* i = (CardItem*)item;
		i->setName(nameEdit->text());
		i->setDiscont(discWidget->value());

	}
}

bool CardWidget::checkSave() const
{
	bool res = true;

	if (nameEdit->text().isEmpty())
	{
		res = false;
	}

	return res;
}
