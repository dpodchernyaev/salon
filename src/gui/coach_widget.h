#ifndef COACHWIDGET_H
#define COACHWIDGET_H

#include <gui/item_widget.h>

class QLineEdit;
class CoachModel;

class CoachWidget : public ItemWidget
{
public:
	CoachWidget();
	virtual void apply();
	virtual void clear();
	virtual bool checkSave() const;
	virtual void set(Item* item);

private:
	QLineEdit* nameEdit;
};

#endif // COACHWIDGET_H
