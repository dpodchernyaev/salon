
#include <QLineEdit>
#include <QLabel>
#include <QSortFilterProxyModel>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <model/coach_item.h>
#include <model/coach_model.h>

#include "coach_widget.h"

CoachWidget::CoachWidget()
{
	QLabel* label = new QLabel("Имя: ");
	nameEdit = new QLineEdit;
	nameEdit->setMinimumWidth(300);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(label);
	hbox->addWidget(nameEdit);
	hbox->addStretch(1);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(hbox);

	setLayout(vbox);
}

void CoachWidget::set(Item* item)
{
	ItemWidget::set(item);
	if (item != NULL)
	{
		CoachItem* i = (CoachItem*)item;
		nameEdit->setText(i->getName());
	}
	else
	{
		nameEdit->setText("");
	}
}

void CoachWidget::apply()
{
	if (item != NULL)
	{
		CoachItem* i = (CoachItem*)item;
		i->setName(nameEdit->text());
	}
}

void CoachWidget::cancel()
{
	nameEdit->clear();
}

bool CoachWidget::checkSave() const
{
	bool res = true;

	if (nameEdit->text().isEmpty())
	{
		res = false;
	}

	return res;
}
