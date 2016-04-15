#ifndef QUEUEFETCHER_H
#define QUEUEFETCHER_H

#include <QMutex>
#include <model/fetcher.h>

class QueueFetcher : public Fetcher
{
	Q_OBJECT
public:
	QueueFetcher();
	virtual ~QueueFetcher() {}

	void fetch(int id);

private Q_SLOTS:
	void queueFetchSlot();

Q_SIGNALS:
	void queueFetch();

protected:
	int id;

private:
	QList<int> queue;
	QMutex mtx;
};

#endif // QUEUEFETCHER_H
