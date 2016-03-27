
#pragma once

#include <QDialog>
#include <QModelIndex>

class ServiceModel;
class QSortFilterProxyModel;
class ServiceItem;
class QListView;
class ServiceWidget;

class NewServiceDialog : public QDialog
{
	Q_OBJECT
public:
	NewServiceDialog(ServiceModel* model);

protected Q_SLOTS:
	virtual void save();
	virtual void create();
	virtual void cancel();
	void currentChanged(QModelIndex ind);

private:
	void free();

private:
	QPushButton* saveBtn;
	QPushButton* cancelBtn;
	QPushButton* creBtn;

	ServiceWidget* widget;
	QListView* view;
	ServiceItem* item;
	ServiceModel* model;
	QSortFilterProxyModel* pmodel;
};
