
#include <QMessageBox>
#include <QVBoxLayout>

#include <gui/vid_widget.h>
#include <model/vid_model.h>
#include <model/vid_item.h>

#include "new_vid_dialog.h"

NewVidDialog::NewVidDialog(VidModel* model) : NewItemDialog(model)
{
	item = NULL;

	setWindowTitle("Виды занятий");

	widget = new VidWidget;
	rightBox->addWidget(widget);
	rightBox->addStretch(1);
}

bool NewVidDialog::save()
{
	if (widget->checkSave())
	{
		widget->apply();
		widget->save();
		NewItemDialog::save();
		return true;
	}
	else
	{
		QMessageBox::warning(NULL, "Предупреждение", "Не все поля введены верно");
		return false;
	}
}

Item* NewVidDialog::createItem()
{
	return new VidItem;
}

void NewVidDialog::setItem(Item *i)
{
	NewItemDialog::setItem(i);
	widget->set(i);
}

void NewVidDialog::clear()
{
	widget->clear();
}
