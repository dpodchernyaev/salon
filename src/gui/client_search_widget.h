#ifndef CLIENTSEARCHWIDGET_H
#define CLIENTSEARCHWIDGET_H

#include <QWidget>

class QLineEdit;
class ClientModel;
class QCompleter;
class ClientListView;
class QPushButton;

class ClientSearchWidget : public QWidget
{
	Q_OBJECT
public:
	ClientSearchWidget(ClientListView *view);
	virtual ~ClientSearchWidget();

public Q_SLOTS:
	void clearEditor();

private Q_SLOTS:
	void editorChanged();
	void refresh();

private:
	QLineEdit* editor;
	QCompleter* completer;
	ClientModel* model;
	ClientListView* view;
	QPushButton* clearBtn;
};

#endif // CLIENTSEARCHWIDGET_H
