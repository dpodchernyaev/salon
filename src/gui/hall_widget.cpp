
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QSortFilterProxyModel>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <model/hall_item.h>
#include <model/hall_model.h>

#include "hall_widget.h"

HallWidget::HallWidget()
{
	QGridLayout* grid = new QGridLayout;
	int row = 0;

	QLabel* label = new QLabel("Имя: ");
	nameEdit = new QLineEdit;
	nameEdit->setMinimumWidth(300);
	grid->addWidget(label, row, 0);
	grid->addWidget(nameEdit, row++, 1);

	label = new QLabel("Мест: ");
	cntWidget = new QSpinBox;
	nameEdit->setMinimumWidth(100);
	grid->addWidget(label, row, 0);
	grid->addWidget(cntWidget, row++, 1);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(grid);

	setLayout(vbox);
}

void HallWidget::set(Item* item)
{
	ItemWidget::set(item);
	if (item != NULL)
	{
		HallItem* i = (HallItem*)item;
		nameEdit->setText(i->getName());
		cntWidget->setValue(i->getCnt());
	}
	else
	{
		nameEdit->clear();
		cntWidget->clear();
	}
}

void HallWidget::apply()
{
	if (item != NULL)
	{
		HallItem* i = (HallItem*)item;
		i->setName(nameEdit->text());
		i->setCnt(cntWidget->value());

	}
}

bool HallWidget::checkSave() const
{
	bool res = true;

	if (nameEdit->text().isEmpty())
	{
		res = false;
	}
	else if (cntWidget->value() == 0)
	{
		res = false;
	}

	return res;
}
