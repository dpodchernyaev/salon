#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QLabel>

#include <gui/background_animation.h>

class PhotoWidget : public QLabel, public BackgroundAnimation
{
	Q_OBJECT
public:
	PhotoWidget();
	virtual ~PhotoWidget() {}
	void setName(const QString &name);
	QString getName() const;

	void setReadOnly(bool flag);

protected:
	void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *);

private Q_SLOTS:
	void loaded(QImage* img);
	void load();

Q_SIGNALS:
	void loadSignal();
	void loadedSignal(QImage*);

private:
	QStringList queue;
	QMutex mtx;
	QString name; ///< сохраняемое имя изображения
	bool readOnly;
};

#endif // PHOTOWIDGET_H
