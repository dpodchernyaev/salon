
#include <QThread>

#include <unistd.h>

#include <config/config.h>

#include <QFile>

#include <model/client_fetcher.h>
#include <model/client_item.h>

#include <db/db_service.h>
#include <db/dbconnection.h>

#include <smtp/src/SmtpMime>

#include "email_sender.h"

#define DEF_INTERVAL 0.5 * 1000
#define ERR_INTERVAL 10. * 1000
#define MAX_INTERVAL 60. * 60 * 1000

EmailSender::EmailSender()
{
	QThread* th = new QThread;
	th->start();
	moveToThread(th);

	fetcher = new ClientFetcher;

	timer.moveToThread(th);
	timer.setInterval(DEF_INTERVAL);

	connect(this, SIGNAL(destroyed()), th, SLOT(deleteLater()));

	connect(fetcher, SIGNAL(fetched(QList<Item*>)),
			this, SLOT(fetched(QList<Item*>)));

	connect(&timer, SIGNAL(timeout()), this, SLOT(send()));
}

EmailSender::~EmailSender()
{
	delete fetcher;
	thread()->exit();
}

void EmailSender::start()
{
	Config* cfg = Config::getInstance();

	if (cfg->getValue(EMAIL_SENDER_FLAG).toBool() == false)
	{
		return;
	}

	QString filePath = cfg->getValue(BIRTHDAY_FILE).toString();
	QFile file(filePath);
	if (file.exists())
	{
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			bool first = true;
			while(!file.atEnd())
			{
				//читаем строку
				QString str = file.readLine();
				if (first)
				{
					theme = str;
					first = false;
				}
				else
				{
					text.append(str);
				}
			}
			file.close();
		}
	}


	if (!theme.isEmpty() && !text.isEmpty())
	{
		fetcher->fetch();
	}
}

bool EmailSender::check(Item* item) const
{
	ClientItem* ci = (ClientItem*)item;
	if (ci->get().email.isEmpty())
	{
		return false;
	}

	if (ci->get().notify == false)
	{
		return false;
	}

	bool res = false;

	QDate currDate = QDate::currentDate();
	QDate bDate = ci->get().birthday.date();
	if (currDate.day() == bDate.day()
			&& currDate.month() == bDate.month())
	{
		DBService* service = DBService::getInstance();
		DBConn* conn = service->getConnection();
		QSqlQuery q(conn->qtDatabase());
		q.prepare("SELECT count(*)"
				  " FROM email_history"
				  " WHERE client_id = ? AND ddate >= ?");
		q.bindValue(0, ci->getId());
		q.bindValue(1, currDate);

		if (q.exec() && q.next())
		{
			if (q.value(0).toInt() == 0)
			{
				res = true;
			}
		}
	}
	return res;
}

void EmailSender::fetched(QList<Item*> items)
{
	qDeleteAll(this->items);
	this->items.clear();

	Q_FOREACH (Item* i, items)
	{
		if (check(i) == true)
		{
			this->items.append(i);
		}
		else
		{
			delete i;
		}
	}
	qDebug() << "SEND MESSAGE STARTED";
	timer.start();
}

bool EmailSender::sendMessage(ClientItem* ci) const
{
	Config* cfg = Config::getInstance();
	SmtpClient smtp(
				cfg->getValue(EMAIL_SMTP_SSL).toString(),
				cfg->getValue(EMAIL_SMTP_SSL_PORT).toInt(),
				SmtpClient::SslConnection);
	smtp.setUser(cfg->getValue(EMAIL_ADDR).toString());
	smtp.setPassword(cfg->getValue(EMAIL_PASSWD).toString());


	EmailAddress sender(cfg->getValue(EMAIL_ADDR).toString(),
						cfg->getValue(EMAIL_NAME).toString());
	EmailAddress to(ci->get().email);

	QString fio = ci->get().surname + " " + ci->get().name + " " + ci->get().patronymic;
	QString localText = this->text.join("\n");
	localText.replace("#", fio);

	MimeText text;
	text.setText(localText);

	MimeMessage message;
	message.setSender(&sender);
	message.addRecipient(&to);
	message.setSubject(theme);
	message.addPart(&text);

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
		res = smtp.sendMail(message);
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

void EmailSender::send()
{
	timer.stop();
	if (items.isEmpty())
	{
		qDebug() << "SEND MESSAGE COMPLITED";
		return;
	}

	QTime time;
	time.start();
	bool res = false;

	Item* i = items.takeFirst();
	ClientItem* ci = (ClientItem*)i;

	DBService* service = DBService::getInstance();
	DBConn* conn = service->getConnection();
	conn->beginTransaction();

	QSqlQuery q(conn->qtDatabase());
	q.prepare("INSERT INTO email_history(client_id, email, ddate)"
			  " VALUES(?, ?, ?)");
	q.bindValue(0, ci->getId());
	q.bindValue(1, ci->get().email);
	q.bindValue(2, QDate::currentDate());

	if (q.exec() && q.isActive())
	{
		res = true;
	}

	if (res == true)
	{
		res = sendMessage(ci);
	}

	if (res == true)
	{
		conn->commit();
		qDebug() << "MESSAGE TO EMAIL SENDING (" << time.elapsed() << " msec)"
				 << ci->get().surname << ci->get().name;
		timer.setInterval(DEF_INTERVAL);
		qDebug() << "NEW SEND MESSAGE INTERVAL " << DEF_INTERVAL / 1000 << " sec";
		delete i;
	}
	else
	{
		conn->rollback();
		int interval = timer.interval();
		if (interval < MAX_INTERVAL)
		{
			interval += ERR_INTERVAL;
		}
		timer.setInterval(interval);
		qDebug() << "NEW SEND MESSAGE INTERVAL " << interval / 1000 << " sec";
		items.append(i);
	}

	timer.start();
}
