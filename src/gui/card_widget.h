
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
	virtual void cancel();
	virtual void set(Item* item);

private:
	QLineEdit* nameEdit;
	QSpinBox* discWidget;
};
