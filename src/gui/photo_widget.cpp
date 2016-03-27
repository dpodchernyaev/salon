
#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QDir>
#include <QVariant>
#include <QString>

#include <config/config.h>

#include "photo_widget.h"

PhotoWidget::PhotoWidget()
{
	setStyleSheet("border: 5px solid grey");
	setAlignment(Qt::AlignCenter);
	readOnly = false;
}

void PhotoWidget::setName(const QString &name)
{
	this->name = name;
	Config* cfg = Config::getInstance();
	QString path = cfg->getValue(PHOTO_PATH).toString();
	path.append(QDir::separator()).append(name).append(".png");

	setPhoto(path);
}

void PhotoWidget::setPhoto(const QString &path)
{
	this->path = path;
	QString localPath;
	QFileInfo fi(path);
	if (!fi.exists())
	{
		qWarning() << "Файла с фотографией не существует " << path;
		localPath = "pics/no_photo.png";
	}
	else
	{
		localPath = fi.absoluteFilePath();
	}

	QImage img(localPath, "PNG");

	QSize s = img.size();
	if (s.width() > width() || s.height() > height())
	{
		img = img.scaled(width() - 10, height() - 10, Qt::KeepAspectRatio);
	}
	setPixmap(QPixmap::fromImage(img));
}

QString PhotoWidget::getName() const
{
	return name;
}

void PhotoWidget::mouseReleaseEvent(QMouseEvent *ev)
{
	QLabel::mouseReleaseEvent(ev);
	if (readOnly == false)
	{
		QString filePath =
				QFileDialog::getOpenFileName
					(NULL, "11", QDir::currentPath(), "PNG(*.png)");
		if (!filePath.isEmpty())
		{
			setPhoto(filePath);
		}
	}
}

void PhotoWidget::save()
{
	QFileInfo info(path);
	if (!info.exists())
	{
		return;
	}

	QImage img(path);
	Config* cfg = Config::getInstance();
	QString path = cfg->getValue(PHOTO_PATH).toString();
	if (name.isEmpty())
	{
		path.append(QDir::separator()).append(info.baseName()).append(".png");
	}
	else
	{
		path.append(QDir::separator()).append(name).append(".png");
	}

	if (img.save(path) == false)
	{
		qCritical() << "Не удалось сохранить изображение";
	}
}

void PhotoWidget::setReadOnly(bool flag)
{
	readOnly = flag;
}
