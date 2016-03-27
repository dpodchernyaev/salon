#ifndef CLIENTINFOPANEL_H
#define CLIENTINFOPANEL_H

#include <QWidget>

class ClientWidget;
class ClientItem;
class QPushButton;
class QPushButton;

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
	void save();
	void edit();
	void cancel();
	void clean();

private:
	ClientWidget* clientWidget;
	QPushButton* saveClientBtn;
	QPushButton* editClientBtn;
	QPushButton* cancelClientBtn;
};

#endif // CLIENTINFOPANEL_H
