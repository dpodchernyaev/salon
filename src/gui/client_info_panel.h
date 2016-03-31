#ifndef CLIENTINFOPANEL_H
#define CLIENTINFOPANEL_H

#include <QWidget>
#include <QPushButton>

class ClientWidget;
class ClientItem;

class CsModel;
class ItemTableView;
class Item;

class QLineEdit;

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

	void setItem(ClientItem* item);

private:
	void edit(bool);

private Q_SLOTS:
	void buyService();
	void delService();
	void useService();
	void serviceLocked(bool);
	void serviceSelected(Item*);

private:

	QPushButton* delBtn;
	QPushButton* buyBtn;
	QPushButton* useBtn;

	ItemTableView* serviceView;
	ClientWidget* clientWidget;
	QLineEdit* summWidget;
	CsModel* csModel;
};

#endif // CLIENTINFOPANEL_H
