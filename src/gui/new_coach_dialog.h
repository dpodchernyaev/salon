#ifndef NEWCOACHDIALOG_H
#define NEWCOACHDIALOG_H

#include <QString>
#include <QModelIndex>

#include <QDialog>

class CoachModel;
class QLineEdit;
class QListView;
class CoachWidget;
class CoachItem;
class QSortFilterProxyModel;

class NewCoachDialog : public QDialog
{
	Q_OBJECT
public:
	NewCoachDialog(CoachModel* model);

private Q_SLOTS:
	virtual void save();
	virtual void cancel();
	virtual void create();
	void currentChanged(QModelIndex ind);

private:
	void free();

private:
	QListView *view;
	QLineEdit* editor;
	QPushButton* newBtn;
	QPushButton* cancelBtn;
	QPushButton* saveBtn;

	QSortFilterProxyModel* pmodel;
	CoachItem* item;
	CoachModel* model;
	CoachWidget* widget;
};

#endif // NEWCOACHDIALOG_H
