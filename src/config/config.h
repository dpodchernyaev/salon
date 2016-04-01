
#pragma once

#include <QSettings>

static QString DB_NAME = "db_name";
static QString DB_HOST = "db_host";
static QString DB_USER_NAME = "db_user_name";
static QString DB_PASSWD = "db_passwd";
static QString DB_PORT = "db_port";
static QString PHOTO_PATH = "photo_path";
static QString EMAIL_ADDR = "email_addr";
static QString EMAIL_NAME = "email_name";
static QString EMAIL_PASSWD = "email_passwd";
static QString EMAIL_SMTP_SSL = "email_smtp_ssl";
static QString EMAIL_SMTP_SSL_PORT = "email_smtp_ssl_port";
static QString BIRTHDAY_FILE = "birthday_file";

class Config
{
private:
	Config();

public:
	static Config* getInstance();

	QVariant getValue(const QString &type) const;
	void read();

private:
	static Config* inst;
	QMap<QString, QVariant> values;
};
