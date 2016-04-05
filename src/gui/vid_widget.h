
#pragma once

#include <gui/item_widget.h>

class QLineEdit;
class CoachModel;

class VidWidget : public ItemWidget
{
public:
	VidWidget();
	virtual void apply();
	virtual bool checkSave() const;
	virtual void set(Item* item);

private:
	QLineEdit* nameEdit;
};

