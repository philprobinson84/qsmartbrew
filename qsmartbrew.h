#ifndef QSMARTBREW_H
#define QSMARTBREW_H

#include <QtWidgets/QMainWindow>
#include <qlabel.h>

class QSmartBrew : public QMainWindow
{
	Q_OBJECT

public:
	QSmartBrew(QWidget *parent = 0);

protected:
	void timerEvent(QTimerEvent *);

private:
	void layoutWindow();

	int m_timeout;
	QLabel *m_timerLabel;
};

#endif // QSMARTBREW_H
