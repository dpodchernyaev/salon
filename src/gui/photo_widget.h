#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QLabel>

class PhotoWidget : public QLabel
{
public:
	PhotoWidget();
	virtual ~PhotoWidget() {}
	void setName(const QString &name);
	void setPhoto(const QString &path);
	QString getName() const;

	void setReadOnly(bool flag);

	void save();

protected:
	void mouseReleaseEvent(QMouseEvent *ev);

private:
	QString name; ///< сохраняемое имя изображения
	QString path; ///< отображаемое изображение
	bool readOnly;
};

#endif // PHOTOWIDGET_H
