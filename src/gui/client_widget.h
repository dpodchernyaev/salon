#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <gui/item_widget.h>

#include <QWidget>

class PhotoWidget;
class ClientItem;
class QLineEdit;
class QDateEdit;
class QCheckBox;
class QSpinBox;
class QComboBox;

class ClientWidget : public ItemWidget
{
public:
	ClientWidget();
	virtual ~ClientWidget() {}

	void set(Item* item);
	void apply();
	void save();
	void cancel();
	bool checkSave() const;
	void edit(bool flag);

private:
	void setClient(ClientItem* item);

private:
	PhotoWidget* photo;
	QLineEdit* nameWidget;
	QLineEdit* surnameWidget;
	QLineEdit* patronymicWidget;
	QLineEdit* emailWidget;
	QLineEdit* phoneWidget;
	QCheckBox* notifyWidget;

	QSpinBox* cardNumberWidget;
	QComboBox* cardType;

	QDateEdit* birthdayWidget;
	QLineEdit* birthdayWidgetEmpty;
};

#endif // CLIENTWIDGET_H
