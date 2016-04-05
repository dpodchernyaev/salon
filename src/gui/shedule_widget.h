#ifndef SHEDULEWIDGET_H
#define SHEDULEWIDGET_H

#include <gui/item_widget.h>

class QComboBox;
class QTimeEdit;

class SheduleWidget : public ItemWidget
{
public:
	SheduleWidget();
	virtual ~SheduleWidget();

	virtual void set(Item* item);
	virtual bool checkSave() const;
	virtual void apply();
	virtual void clear();

private:
	QComboBox* vidWidget;
	QComboBox* dayWidget;
	QComboBox* coachWidget;
	QComboBox* hallWidget;
	QTimeEdit* bTimeWidget;
	QTimeEdit* eTimeWidget;
};

#endif // SHEDULEWIDGET_H
