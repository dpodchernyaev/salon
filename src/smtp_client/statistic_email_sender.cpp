
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QDateTime>

#include <db/db_service.h>
#include <db/dbconnection.h>

#include <smtp/src/SmtpMime>

#include "statistic_email_sender.h"

StatisticEmailSender::StatisticEmailSender()
{
	QThread* th = new QThread;
	th->start();
	moveToThread(th);

	connect(this, SIGNAL(initSignal()), SLOT(initSlot()));
	connect(this, SIGNAL(destroyed()), th, SLOT(deleteLater()));
}

StatisticEmailSender::~StatisticEmailSender()
{
	thread()->exit();
}

void StatisticEmailSender::init()
{
	Q_EMIT initSignal();
}

bool StatisticEmailSender::send(const QString &message) const
{
	SmtpClient smtp("smtp.mail.ru", 465, SmtpClient::SslConnection);

	smtp.setUser("sheridan13avg@mail.ru");
	smtp.setPassword("ArchangelA");


	EmailAddress sender("sheridan13avg@mail.ru", "Monella");
	EmailAddress to("sheridan13avg@mail.ru");

	MimeText mimeText;
	mimeText.setText(message);

	MimeMessage mimeMessage;
	mimeMessage.setSender(&sender);
	mimeMessage.addRecipient(&to);
	mimeMessage.setSubject("Monella statistic");
	mimeMessage.addPart(&mimeText);

	bool res = smtp.connectToHost();
	if (res == false)
	{
		qWarning() << "Failed to connect to host!" << endl;
	}

	if (res == true)
	{
		res = smtp.login();
		if (res == false)
		{
			qWarning() << "Failed to login!" << endl;
		}
	}

	if (res == true)
	{
		res = smtp.sendMail(mimeMessage);
		if (res == false)
		{
			qWarning() << "Failed to send mail!" << endl;
		}
		else
		{
			smtp.quit();
		}
	}

	return res;
}

bool StatisticEmailSender::checkSend(DBConn *conn) const
{
	QDateTime time = QDateTime::currentDateTime();
	time = time.addDays(-2);

	QString sql =
			"SELECT count(*)"
				" FROM email_history"
				" WHERE type = ? AND ddate > ?";

	QSqlQuery q(conn->qtDatabase());
	q.prepare(sql);
	q.bindValue(0, 1);
	q.bindValue(1, time);

	bool res = conn->executeQuery(q);
	if ( (res == true) && (q.isActive()) )
	{
		if (q.next() == true)
		{
			res = q.value(0).toInt() == 0;
		}
	}
	return res;
}

bool StatisticEmailSender::updateHistory(DBConn *conn) const
{
	QDateTime time = QDateTime::currentDateTime();

	QString sql =
			"INSERT INTO"
				" email_history(client_id, email, ddate, type)"
			"VALUES(?, ?, ?, ?)";
	QSqlQuery q(conn->qtDatabase());
	q.prepare(sql);
	q.bindValue(0, 0);
	q.bindValue(1, "");
	q.bindValue(2, time);
	q.bindValue(3, 1);

	bool res = conn->executeQuery(q);
	if (res == true)
	{
		res = q.isActive();
	}
	return res;
}

void StatisticEmailSender::initSlot() const
{
	bool res = true;

	DBConn* conn = DBService::getInstance()->getConnection();
	if (!conn->isConnected())
	{
		qCritical() << Q_FUNC_INFO << "Нет соединения с БД";
		res = false;
	}


	QStringList message;

	if (res == true)
	{
		QString sql = "SELECT relname,n_live_tup"
					  " FROM pg_stat_user_tables"
					  " ORDER BY n_live_tup DESC";

		conn->beginTransaction();

		bool check = checkSend(conn);
		if (check == true)
		{
			QSqlQuery q = conn->executeQuery(sql);
			if (!q.isActive())
			{
				res = false;
				qCritical() << Q_FUNC_INFO << "Ошибка выполнения запроса статистики";
			}
			else
			{
				bool first = true;
				while (q.next())
				{
					if (first == true)
					{
						message.append("Статистика по записям в таблицах: ");
						first = false;
					}
					message.append(q.value(0).toString() + ": " + q.value(1).toString());
				}
			}

			res = updateHistory(conn);
		}

		conn->commit();
	}

	if ( (res == true) && !message.isEmpty())
	{
		res = send(message.join("\n"));
	}

	if (res == false)
	{
		QTimer::singleShot(30 * 60 * 1000,
				const_cast<StatisticEmailSender*>(this), SLOT(initSlot()));
	}
}
