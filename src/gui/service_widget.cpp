
#include <QDebug>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>

#include <model/service_item.h>

#include "service_widget.h"

ServiceWidget::ServiceWidget()
{
	item = NULL;

	QGridLayout* grid = new QGridLayout;

	int row = 0;
	QLabel* label = new QLabel("Название: ");
	nameWidget = new QLineEdit;
	nameWidget->setMinimumWidth(300);
	grid->addWidget(label, row, 0);
	grid->addWidget(nameWidget, row++, 1);

	label = new QLabel("Цена: ");
	priceWidget = new QDoubleSpinBox;
	priceWidget->setMaximum(300000);
	grid->addWidget(label, row, 0);
	grid->addWidget(priceWidget, row++, 1);

	label = new QLabel("Тип ограничения: ");
	limitTypeWidget = new QComboBox;
	grid->addWidget(label, row, 0);
	grid->addWidget(limitTypeWidget, row++, 1);

	label = new QLabel("Занятий: ");
	limitValueWidget = new QSpinBox;
	limitValueWidget->setMaximum(1000);
	grid->addWidget(label, row, 0);
	grid->addWidget(limitValueWidget, row++, 1);

	label = new QLabel("Дней: ");
	limitDaysWidget = new QSpinBox;
	limitDaysWidget->setMaximum(1000);
	grid->addWidget(label, row, 0);
	grid->addWidget(limitDaysWidget, row++, 1);

	usedWidget = new QCheckBox("Используется");
	grid->addWidget(usedWidget, row++, 1);


	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(grid);
	vbox->addStretch(1);

	setLayout(vbox);

	limitTypeWidget->addItem(ServiceItem::toString(LT_DATE_COUNT));
	limitTypeWidget->addItem(ServiceItem::toString(LT_DATE));
	limitTypeWidget->addItem(ServiceItem::toString(LT_COUNT));

	connect(limitTypeWidget, SIGNAL(currentIndexChanged(int)),
			this, SLOT(typeChanged(int)));

	typeChanged(0);
}

void ServiceWidget::typeChanged(int type)
{
	if (type == 0)
	{
		limitDaysWidget->setEnabled(true);
		limitValueWidget->setEnabled(true);
	}
	else if (type == 1)
	{
		limitDaysWidget->setEnabled(true);
		limitValueWidget->setEnabled(false);
	}
	else if (type == 2)
	{
		limitDaysWidget->setEnabled(false);
		limitValueWidget->setEnabled(true);
	}
}

void ServiceWidget::set(Item* item)
{
	ItemWidget::set(item);

	if (item == NULL)
	{
		nameWidget->setText("");
		priceWidget->setValue(0);
		limitValueWidget->setValue(0);
		limitDaysWidget->setValue(0);
		usedWidget->setCheckState(Qt::Unchecked);
		limitTypeWidget->setCurrentIndex(0);
	}
	else
	{
		ServiceItem* i = (ServiceItem*)item;
		ServiceParam p = i->get();

		nameWidget->setText(p.name);
		priceWidget->setValue(p.price);
		limitValueWidget->setValue(p.value);
		limitDaysWidget->setValue(p.limitDays);
		usedWidget->setCheckState(p.used ? Qt::Checked : Qt::Unchecked);
		limitTypeWidget->setCurrentIndex(
					limitTypeWidget->findText(
						ServiceItem::toString(p.limitType)));
	}
}

void ServiceWidget::apply()
{
	if (item != NULL)
	{
		ServiceItem* c = (ServiceItem*)item;
		ServiceParam p = c->get();

		p.name = nameWidget->text();
		p.limitDays = limitDaysWidget->value();
		if (limitTypeWidget->currentIndex() == 0)
		{
			p.limitType = LT_DATE_COUNT;
		}
		else if (limitTypeWidget->currentIndex() == 1)
		{
			p.limitType = LT_DATE;
		}
		else
		{
			p.limitType = LT_COUNT;
		}
		p.price = priceWidget->value();
		p.used = usedWidget->checkState() == Qt::Checked ? true : false;
		p.value = limitValueWidget->value();
		c->set(p);
	}
}

void ServiceWidget::cancel()
{
	set(item);
}
