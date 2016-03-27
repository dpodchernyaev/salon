
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStringList>

#include "config/config.h"

#define SALON_CONFIG "salon.ini"

Config* Config::inst = NULL;

Config::Config()
{

}

QVariant Config::getValue(const QString &type) const
{
	return values.value(type, "");
}

void Config::read()
{
	QString path = QDir::currentPath();
	path.append("/").append(SALON_CONFIG);

	QFile file(path);
	if (!file.exists())
	{
		qCritical() << "Нет файла конфигурации" << path;
		return;
	}

	if (!file.open(QIODevice::ReadOnly))
	{
		qCritical() << "Файл с настройками не открывается" << path;
		return;
	}
	file.close();

	QSettings s(path, QSettings::IniFormat);
	QStringList keys = s.allKeys();
	Q_FOREACH (QString key, keys)
	{
		values.insert(key, s.value(key));
	}
}

Config *Config::getInstance()
{
	if (inst == NULL)
	{
		inst = new Config;
		inst->read();
	}
	return inst;
}
