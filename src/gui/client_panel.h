#ifndef CLIENTPANEL_H
#define CLIENTPANEL_H

#include <QMainWindow>

class ClientListView;
class ClientInfoPanel;
class ClientSearchWidget;
class ClientItem;
class CoachModel;
class ServiceModel;
class HallModel;
class CardModel;

class ClientPanel : public QMainWindow
{
	Q_OBJECT
public:
	ClientPanel();
	virtual ~ClientPanel();

private Q_SLOTS:
	void clientLocked(bool);
	void selected(ClientItem*);
	void modelRestored();

	void newClient();
	void newCoach();
	void newService();
	void newHall();
	void newCard();

private:
	ClientListView* view;
	ClientSearchWidget* searchWidget;
	ClientInfoPanel* infoWidget;

	CardModel* cardModel;
	HallModel* hallModel;
	CoachModel* coachModel;
	ServiceModel* serviceModel;
};

#endif // CLIENTPANEL_H
