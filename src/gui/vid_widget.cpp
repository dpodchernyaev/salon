
#include <QLineEdit>
#include <QLabel>
#include <QSortFilterProxyModel>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <model/vid_item.h>
#include <model/vid_model.h>

#include "vid_widget.h"

VidWidget::VidWidget()
{
	QLabel* label = new QLabel("Название: ");
	nameEdit = new QLineEdit;
	nameEdit->setMinimumWidth(300);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(label);
	hbox->addWidget(nameEdit);
	hbox->addStretch(1);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(hbox);

	setLayout(vbox);

	item = NULL;
}

void VidWidget::set(Item* item)
{
	ItemWidget::set(item);
	if (item != NULL)
	{
		VidItem* i = (VidItem*)item;
		nameEdit->setText(i->getParam().name);
	}
	else
	{
		nameEdit->clear();
	}
}

void VidWidget::apply()
{
	if (item != NULL)
	{
		VidItem* i = (VidItem*)item;
		VidParam p = i->getParam();
		p.name = nameEdit->text();
		i->setParam(p);
	}
}

bool VidWidget::checkSave() const
{
	bool res = true;

	if (nameEdit->text().isEmpty())
	{
		res = false;
	}

	return res;
}
