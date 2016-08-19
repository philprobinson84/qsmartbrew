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
	void getTemperatures();
	void sendTemperatures();

	int m_timeout;
	float m_tempAmb;
	float m_tempFV;
	QLabel *m_timerLabel;
	QLabel *m_tempAmbLabel;
	QLabel *m_tempFVLabel;
};

#endif // QSMARTBREW_H
