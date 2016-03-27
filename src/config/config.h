
#pragma once

#include <QSettings>

static QString DB_NAME = "db_name";
static QString DB_HOST = "db_host";
static QString DB_USER_NAME = "db_user_name";
static QString DB_PASSWD = "db_passwd";
static QString DB_PORT = "db_port";

static QString PHOTO_PATH = "photo_path";

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
