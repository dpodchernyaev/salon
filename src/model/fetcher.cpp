
#include <QThread>

#include <db/db_service.h>

#include "fetcher.h"

Fetcher::Fetcher()
{
	DBService* service = DBService::getInstance();
	QThread* th = service->getThread();
	moveToThread(th);

	connect(this, SIGNAL(fetchSignal()), SLOT(fetchSlot()));
	connect(this, SIGNAL(saveSignal(Item*)), SLOT(saveSlot(Item*)));
}

Fetcher::~Fetcher()
{

}

void Fetcher::fetch()
{
	Q_EMIT fetchSignal();
}

void Fetcher::save(Item* item)
{
	Q_EMIT saveSignal(item);
}
