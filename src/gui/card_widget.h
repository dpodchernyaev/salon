
#pragma once

#include <gui/item_widget.h>

class QLineEdit;
class CoachModel;
class QSpinBox;

class CardWidget : public ItemWidget
{
public:
	CardWidget();
	virtual void apply();
	virtual void clear();
	virtual bool checkSave() const;
	virtual void set(Item* item);

private:
	QLineEdit* nameEdit;
	QSpinBox* discWidget;
};
