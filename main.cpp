#include "qsmartbrew.h"
#include <QtWidgets/QApplication>

#ifdef Q_OS_LINUX
#define SHOW_FULLSCREEN
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSmartBrew w;

#ifdef SHOW_FULLSCREEN
	w.showFullScreen();
#else
	w.show();
#endif

	return a.exec();
}
