
#pragma once

#include <gui/item_widget.h>

class QLineEdit;
class CoachModel;
class QSpinBox;

class HallWidget : public ItemWidget
{
public:
	HallWidget();
	virtual void apply();
	virtual void cancel();
	virtual bool checkSave() const;
	virtual void set(Item* item);

private:
	QLineEdit* nameEdit;
	QSpinBox* cntWidget;
};
