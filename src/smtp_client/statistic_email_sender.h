#ifndef STATISTICEMAILSENDER_H
#define STATISTICEMAILSENDER_H

#include <QObject>

class DBConn;

class StatisticEmailSender : public QObject
{
	Q_OBJECT

public:
	StatisticEmailSender();
	virtual ~StatisticEmailSender();

	void init();

private:
	bool send(const QString &message) const;
	bool checkSend(DBConn* conn) const;
	bool updateHistory(DBConn* conn) const;

private Q_SLOTS:
	void initSlot() const;

Q_SIGNALS:
	void initSignal();
};

#endif // STATISTICEMAILSENDER_H
