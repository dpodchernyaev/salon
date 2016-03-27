#ifndef CLIENTFETCHER_H
#define CLIENTFETCHER_H

#include <model/fetcher.h>
#include <QObject>

class ClientItem;
class DBConn;

class ClientFetcher : public Fetcher
{
public:
	virtual ~ClientFetcher();

private:
	void fetchSlot();
	void saveSlot(Item *item);

	bool saveCard(Item *item, DBConn *conn);

	bool checkClientService(int client_id, DBConn* conn);
};

#endif // CLIENTFETCHER_H
