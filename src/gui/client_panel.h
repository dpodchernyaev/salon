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
class VidModel;
class Item;
class SheduleModel;
class EmailSender;
class StatisticEmailSender;

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
	void newVid();
	void newCoach();
	void newService();
	void newHall();
	void newCard();
	void newShedule();

	void newPrivate();

	void clientReport();
	void coachReport();

private:
	ItemListWidget* view;
	ClientInfoPanel* infoWidget;

	ClientModel* clientModel;
	CardModel* cardModel;
	HallModel* hallModel;
	CoachModel* coachModel;
	ServiceModel* serviceModel;
	SheduleModel* sheduleModel;
	VidModel* vidModel;
	EmailSender* email_sender;
	StatisticEmailSender* email_st;
};

#endif // CLIENTPANEL_H
