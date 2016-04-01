#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;

class LoginDialog : public QDialog
{
	Q_OBJECT
public:
	LoginDialog();
	virtual ~LoginDialog() {}

private Q_SLOTS:
	void ok();

private:
	QLineEdit* name;
	QLineEdit* passwd;
};

#endif // LOGINDIALOG_H
