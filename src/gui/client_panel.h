#ifndef CLIENTPANEL_H
#define CLIENTPANEL_H

#include <QMainWindow>

class ItemListWidget;
class ClientInfoPanel;
class ClientSearchWidget;
class ClientItem;
class CoachModel;
class ServiceModel;
class HallModel;
class CardModel;
class ClientModel;
class Item;
class SheduleModel;

class ClientPanel : public QMainWindow
{
	Q_OBJECT
public:
	ClientPanel();
	virtual ~ClientPanel();

private Q_SLOTS:
	void clientLocked(bool);
	void selected(Item*);
	void modelRestored();

	void newClient();
	void newCoach();
	void newService();
	void newHall();
	void newCard();
	void newShedule();

private:
	ItemListWidget* view;
	ClientInfoPanel* infoWidget;

	ClientModel* clientModel;
	CardModel* cardModel;
	HallModel* hallModel;
	CoachModel* coachModel;
	ServiceModel* serviceModel;
	SheduleModel* sheduleModel;
};

#endif // CLIENTPANEL_H
