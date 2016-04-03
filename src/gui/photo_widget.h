#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QLabel>

#include <gui/background_animation.h>

class PhotoLoader : public QObject
{
	Q_OBJECT
public:
	PhotoLoader(QWidget* view);
	virtual ~PhotoLoader();

	void load(const QString &name);

private Q_SLOTS:
	void loadSlot();

Q_SIGNALS:
	void loaded(QImage*);
	void loadSignal();

private:
	QWidget* view;
	QMutex mtx;
	QStringList queue;
};

class PhotoWidget : public QLabel, public BackgroundAnimation
{
	Q_OBJECT
public:
	PhotoWidget();
	virtual ~PhotoWidget();
	void setName(const QString &name);
	QString getName() const;

	void setReadOnly(bool flag);

protected:
	void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *);

private Q_SLOTS:
	void loaded(QImage* img);

private:
	PhotoLoader* loader;
	QString name; ///< сохраняемое имя изображения
	bool readOnly;
};

#endif // PHOTOWIDGET_H
