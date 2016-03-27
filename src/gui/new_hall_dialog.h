
#pragma once

#include <QString>
#include <QModelIndex>

#include <QDialog>

class HallModel;
class QListView;
class HallItem;
class HallWidget;
class QSortFilterProxyModel;

class NewHallDialog : public QDialog
{
	Q_OBJECT
public:
	NewHallDialog(HallModel* model);

private Q_SLOTS:
	virtual void save();
	virtual void cancel();
	virtual void create();
	void currentChanged(QModelIndex ind);

private:
	void free();

private:
	QListView *view;
	QPushButton* newBtn;
	QPushButton* cancelBtn;
	QPushButton* saveBtn;

	QSortFilterProxyModel* pmodel;
	HallItem* item;
	HallModel* model;
	HallWidget* widget;
};
