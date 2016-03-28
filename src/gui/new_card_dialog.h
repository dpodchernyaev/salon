
#pragma once

#include <QString>

#include <gui/new_item_dialog.h>

class CardModel;
class CardWidget;

class NewCardDialog : public NewItemDialog
{
	Q_OBJECT
public:
	NewCardDialog(CardModel* model);

protected:
	virtual void clear();
	virtual Item* createItem();
	virtual void setItem(Item *i);
	virtual bool save();

private:
	void free();

private:
	CardWidget* widget;
};
