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
	bool saveSlot(Item *Item, DBConn* conn);
	bool deleteSlot(Item *i, DBConn *conn);

	bool saveCard(Item *item, DBConn *conn);

	bool checkClientService(int client_id, DBConn* conn);
};

#endif // CLIENTFETCHER_H
