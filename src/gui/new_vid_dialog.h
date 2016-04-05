
#pragma once

#include <QString>

#include <gui/new_item_dialog.h>

class VidModel;
class VidWidget;

class NewVidDialog : public NewItemDialog
{
public:
	NewVidDialog(VidModel* model);

protected:
	virtual void clear();
	virtual Item* createItem();
	virtual void setItem(Item *i);
	virtual bool save();

private:
	void free();


private:
	VidWidget* widget;
};
