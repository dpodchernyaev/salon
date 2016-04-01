#ifndef EMAILSENDER_H
#define EMAILSENDER_H

#include <QStringList>
#include <QObject>
#include <QTimer>

class ClientFetcher;
class Item;
class SmtpClient;
class ClientItem;

class EmailSender : public QObject
{
	Q_OBJECT
public:
	EmailSender();
	virtual ~EmailSender();

	void start();

private Q_SLOTS:
	void fetched(QList<Item*>);
	void send();

private:
	bool check(Item* item) const;
	bool sendMessage(ClientItem *ci) const;

private:
	QString theme;
	QStringList text;
	ClientFetcher* fetcher;
	QList<Item*> items;
	QTimer timer;
};

#endif // EMAILSENDER_H
