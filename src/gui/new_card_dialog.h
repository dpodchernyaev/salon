
#pragma once

#include <QString>
#include <QModelIndex>

#include <QDialog>

class CardModel;
class QListView;
class CardItem;
class CardWidget;
class QSortFilterProxyModel;

class NewCardDialog : public QDialog
{
	Q_OBJECT
public:
	NewCardDialog(CardModel* model);

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
	CardItem* item;
	CardModel* model;
	CardWidget* widget;
};
