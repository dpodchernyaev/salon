#include "queue_fetcher.h"

QueueFetcher::QueueFetcher()
{
	id = 0;
	connect(this, SIGNAL(queueFetch()), SLOT(queueFetchSlot()));
}


void QueueFetcher::fetch(int id)
{
	mtx.lock();
	queue.append(id);
	mtx.unlock();

	Q_EMIT queueFetch();
}

void QueueFetcher::queueFetchSlot()
{
	mtx.lock();
	if (queue.isEmpty())
	{
		mtx.unlock();
		return;
	}

	id = queue.takeLast();
	queue.clear();
	mtx.unlock();

	fetchPrivate();
}
