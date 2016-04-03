
#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QDir>
#include <QVariant>
#include <QString>
#include <QThread>
#include <QMessageBox>

#include <config/config.h>

#include "photo_widget.h"

PhotoWidget::PhotoWidget() : BackgroundAnimation(this)
{
	QThread* th = new QThread;
	th->start();
	moveToThread(th);

	setAlignment(Qt::AlignCenter);
	readOnly = false;

	connect(this, SIGNAL(loadSignal()), this, SLOT(load()));
	connect(this, SIGNAL(loadedSignal(QImage*)), this, SLOT(loaded(QImage*)));
}

void PhotoWidget::setName(const QString &name)
{
	this->name = name;
	Config* cfg = Config::getInstance();
	QString path = cfg->getValue(PHOTO_PATH).toString();
	path.append(QDir::separator()).append(name);

	showAnimation();

	mtx.lock();
	queue.append(path);
	mtx.unlock();

	Q_EMIT load();
}

void PhotoWidget::load()
{
	mtx.lock();
	if (queue.isEmpty())
	{
		return;
	}
	QString path = queue.takeLast();
	queue.clear();
	mtx.unlock();

	QString localPath;
	QFileInfo fi(path);
	if (!fi.exists() || !fi.isFile())
	{
		qWarning() << "Файла с фотографией не существует " << path;
		localPath = "pics/no_photo.png";
	}
	else
	{
		localPath = fi.absoluteFilePath();
	}

	QImage img(localPath);

	QSize s = img.size();
	if (s.width() > width() || s.height() > height())
	{
		img = img.scaled(width() - 10, height() - 10, Qt::KeepAspectRatio);
	}

	QImage* res = NULL;
	if (!img.isNull())
	{
		res = new QImage(img);
	}
	else
	{
		res = new QImage();
	}

	mtx.lock();
	bool isEmpt = queue.isEmpty();
	mtx.unlock();

	if (isEmpt == true)
	{
		Q_EMIT loadedSignal(res);
	}
	else
	{
		delete res;
		load();
	}
}

void PhotoWidget::loaded(QImage* img)
{
	setPixmap(QPixmap::fromImage(*img));
	delete img;
	hideAnimation();
}

QString PhotoWidget::getName() const
{
	return name;
}

void PhotoWidget::paintEvent(QPaintEvent* e)
{
	QLabel::paintEvent(e);
	paintAnimation(e);
}

void PhotoWidget::mouseReleaseEvent(QMouseEvent *ev)
{
	QLabel::mouseReleaseEvent(ev);
	if (readOnly == false)
	{
		Config* cfg = Config::getInstance();
		QString path = cfg->getValue(PHOTO_PATH).toString();

		while (true)
		{
			QString filePath =
					QFileDialog::getOpenFileName
						(NULL, "Фотография клиента", path, "Images (*.png *.jpg)",
						 NULL, QFileDialog::ReadOnly);
			if (!filePath.isEmpty())
			{
				QFileInfo fi(filePath);
				QDir dir = fi.dir();
				QDir defDir(path);

				if (dir.absolutePath() != defDir.absolutePath())
				{
					QMessageBox::information(this, "Предупреждение",
											 "Необходимо выбрать файл из каталога "
												+ path);
				}
				else
				{
					setName(fi.fileName());
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
}

void PhotoWidget::setReadOnly(bool flag)
{
	readOnly = flag;
}

