#include <QtCore/QCoreApplication>

#include <qdebug.h>
#include <qfile.h>
#include <qdir.h>
#include <qlockfile.h>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	qInfo() << "Attempting to lock a file ...";

	QString path = QDir::currentPath() + QDir::separator() + "test.txt";
	QFile file(path);
	QLockFile lock(file.fileName() + "l");
	lock.setStaleLockTime(30000);

	if (lock.tryLock()) {
		qInfo() << "Putting into file...";
		if (file.open(QIODevice::WriteOnly)) {
			QByteArray data;
			file.write(data);
			file.close();
			qInfo() << "Finished with file...";
		}
		qInfo() << "Unlocking file";
		lock.unlock();
	}
	else {
		qInfo() << "Could not lock the file!";
		qint64 pid;
		QString host;
		QString application;

		if (lock.getLockInfo(&pid, &host, &application)) {
			qInfo() << "The file is locked by: ";
			qInfo() << "Pid: " << pid;
			qInfo() << "Host: " << host;
			qInfo() << "Application: " << application;
		}
		else {
			qInfo() << "File is locked, but we can't get the info!";
		}
	}

	return a.exec();
}
