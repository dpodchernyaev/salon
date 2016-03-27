#ifndef NEWCLIENTDIALOG_H
#define NEWCLIENTDIALOG_H

class ClientModel;
class ClientWidget;
class ClientItem;

class CoachModel;
class CoachWidget;
class CoachItem;

class ServiceModel;

class QLineEdit;
class QHBoxLayout;
class ItemModel;
class ItemWidget;
class QVBoxLayout;

#include <QDialog>

class NewClientDialog : public QDialog
{
	Q_OBJECT
public:
	NewClientDialog(ClientModel* model);
	virtual ~NewClientDialog();

protected:
	QString  getErrorStr() const;
	virtual QVBoxLayout* construct();

protected Q_SLOTS:
	virtual void save();
	virtual void cancel();

protected:
	QPushButton* saveBtn;
	QPushButton* cancelBtn;
	QHBoxLayout* btnBox;
	ItemWidget* widget;
	ItemModel* model;
	ClientItem* item;
};

#endif // NEWCLIENTDIALOG_H
