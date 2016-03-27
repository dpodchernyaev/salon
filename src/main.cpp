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

#include <db/db_service.h>
#include <gui/client_panel.h>

#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

class ClientItem;
class Item;

/** Запускает приложение */
int
launchApp(int argc, char *argv[])
{
#ifdef Q_WS_X11
	XInitThreads();
#endif
	QApplication app(argc, argv);

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);

	QLocale::setDefault(QLocale(QLocale::Russian, QLocale::RussianFederation));

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
