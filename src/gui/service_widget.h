
#pragma once

#include <QWidget>

class PhotoWidget;
class ClientItem;
class QLineEdit;
class QDateEdit;
class QCheckBox;
class QDoubleSpinBox;
class QSpinBox;
class QComboBox;

#include <gui/item_widget.h>

class ServiceWidget : public ItemWidget
{
	Q_OBJECT
public:
	ServiceWidget();
	virtual ~ServiceWidget() {}

	void set(Item* item);
	void apply();
	bool checkSave() const;

private Q_SLOTS:
	void typeChanged(int);

private:

	QLineEdit* nameWidget;
	QDoubleSpinBox* priceWidget;
	QComboBox* limitTypeWidget;
	QSpinBox* limitValueWidget;
	QSpinBox* limitDaysWidget;
	QCheckBox* usedWidget;
};
