/* Copyright (c) 2015 Forward Systems. All rights reserved */

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QLocale>
#include <QTextCodec>
#include <QHash>
#include <QDir>
#include <QDebug>
#include <QSqlQuery>

#include <login_dialog.h>
#include <db/db_service.h>
#include <gui/client_panel.h>

#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

class ClientItem;
class Item;

#include <QCleanlooksStyle>

class SalonStyle : public QCleanlooksStyle
{
public:
	int pixelMetric(PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const {
		int s = QCleanlooksStyle::pixelMetric(metric, option, widget);
		if (metric == QStyle::PM_SmallIconSize) {
			s = 40;
		}
		return s;
	}
};

/** Запускает приложение */
int
launchApp(int argc, char *argv[])
{
#ifdef Q_WS_X11
	XInitThreads();
#endif
	QApplication app(argc, argv);

#ifdef Q_OS_WIN
	QApplication::setStyle(new SalonStyle);
#endif

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);

	QLocale::setDefault(QLocale(QLocale::Russian, QLocale::RussianFederation));

	if (!DBService::getInstance()->getConnection()->isConnected())
	{
		QMessageBox::critical(NULL, "Ошибка БД", "Нет соединения с БД");
		exit(0);
	}

	LoginDialog login;
	if (login.exec() == false)
	{
		exit(0);
	}

	qRegisterMetaType<ClientItem*>("ClientItem*");
	qRegisterMetaType<Item*>("Item*");
	qRegisterMetaType< QList<ClientItem*> >("QList<ClientItem*>");
	qRegisterMetaType< QList<Item*> >("QList<Item*>");
	ClientPanel mainPanel;
	mainPanel.showMaximized();

	qDebug() << QSqlDatabase::drivers();

	return app.exec();
}

/** Точка входа. */
int main(int argc, char *argv[])
{
	return launchApp(argc, argv);
}



/*
 * ===== Доработки: (1 чч = 400 р) =====
 * + 1. Пересечение в расписании 4 чч = 1600
 * + 5. Доп информация по тренеру индивидуальные занятия 2 чч = 800 (Индивидуальные занятия тренера)
 * + кремовый цвет фона
 *
 * Сумма:	28 000 + 3600 = 31600
 * Аванс:	14 000
 * Остаток: 31600 - 14 000 = 17 600 р.
*/
