#ifndef CLIENTINFOPANEL_H
#define CLIENTINFOPANEL_H

#include <QWidget>
#include <QTimer>
#include <QPushButton>

class ClientWidget;
class ClientItem;

class CsModel;
class VisitModel;
class GroupModel;

class ItemTableView;
class Item;

class QLineEdit;
class QMessageBox;

class DoublePushButton : public QPushButton
{
	Q_OBJECT
protected:
	void mouseDoubleClickEvent(QMouseEvent *e)
	{
		QPushButton::mouseDoubleClickEvent(e);
		Q_EMIT doubleClicked();
	}

Q_SIGNALS:
	void doubleClicked();
};

class ClientInfoPanel : public QWidget
{
	Q_OBJECT
public:
	ClientInfoPanel();
	virtual ~ClientInfoPanel();

	void setItem(ClientItem* item, bool reload = true);

private:
	void edit(bool);

private Q_SLOTS:
	void buyService();
	void delService();
	void delVisit();
	void useService();
	void serviceLocked(bool);
	void visitLocked(bool);
	void serviceSelected(Item*);
	void visitSelected(Item*);

	void fetch();

private:

	QMessageBox* waitDialog;

	QPushButton* delVisBtn;

	QPushButton* delBtn;
	QPushButton* buyBtn;
	QPushButton* useBtn;

	ItemTableView* visitView;
	ItemTableView* serviceView;
	ClientWidget* clientWidget;
	QLineEdit* summWidget;
	CsModel* csModel;
	GroupModel* gModel;
	VisitModel* visModel;

	QTimer upTimer;
};

#endif // CLIENTINFOPANEL_H
