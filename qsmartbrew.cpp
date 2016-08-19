#include <qboxlayout.h>
#include <qlabel.h>
#include <qtimer.h>

#include "qsmartbrew.h"

#define DEFAULT_TIMEOUT_SECONDS 60

QSmartBrew::QSmartBrew(QWidget *parent)
	: QMainWindow(parent)
{
	m_timeout = DEFAULT_TIMEOUT_SECONDS;

	layoutWindow();

	startTimer(1000);
}

void QSmartBrew::getTemperatures()
{
	// FV...
	QFile inputFile("/sys/bus/w1/devices/28-0216011c53ff/w1_slave");
	if (inputFile.open(QIODevice::ReadOnly))
	{
	   QTextStream in(&inputFile);
	   while (!in.atEnd())
	   {
	      QString line = in.readLine();
				QString isValid = line.right(4); // line contains "YES " if next line is valid temp

				if (isValid.compare("YES ") == 0)
				{
					line = in.readLine();
					QString tempFVraw = line.right(6);
					QString tempFVformatted = tempFVraw.left(2) + "." + tempFVraw.right(4);
					m_tempFV = tempFVformatted.toFloat();
				}
	   }
	   inputFile.close();
	}

	// Amb...
	QFile inputFile("/sys/bus/w1/devices/28-021571c028ff/w1_slave");
	if (inputFile.open(QIODevice::ReadOnly))
	{
		 QTextStream in(&inputFile);
		 while (!in.atEnd())
		 {
				QString line = in.readLine();
				QString isValid = line.right(4); // line contains "YES " if next line is valid temp

				if (isValid.compare("YES ") == 0)
				{
					line = in.readLine();
					QString tempFVraw = line.right(6);
					QString tempFVformatted = tempFVraw.left(2) + "." + tempFVraw.right(4);
					m_tempFV = tempFVformatted.toFloat();
				}
		 }
		 inputFile.close();
	}
}

void QSmartBrew::timerEvent(QTimerEvent *)
{
	m_timeout--;
	getTemperatures();
	sendTemperatures();

	if (m_timeout < 1)
		close();
	else
		m_timerLabel->setText(QString::number(m_timeout));
		m_tempAmbLabel->setText(QString::number(m_tempAmb, 'f', 3));
		m_tempFVLabel->setText(QString::number(m_tempFV, 'f', 3));
}

void QSmartBrew::layoutWindow()
{
	QVBoxLayout *layout = new QVBoxLayout;

	m_timerLabel = new QLabel(QString::number(m_timeout));
	m_timerLabel->setAlignment(Qt::AlignCenter);
	m_timerLabel->setStyleSheet("background-color: white; color: black");
	layout->addWidget(m_timerLabel);

	m_tempFVLabel = new QLabel(QString::number(m_tempFV, 'f', 3));
	m_tempFVLabel->setAlignment(Qt::AlignCenter);
	m_tempFVLabel->setStyleSheet("background-color: white; color: black");
	layout->addWidget(m_tempFVLabel);

	m_tempAmbLabel = new QLabel(QString::number(m_tempAmb, 'f', 3));
	m_tempAmbLabel->setAlignment(Qt::AlignCenter);
	m_tempAmbLabel->setStyleSheet("background-color: white; color: black");
	layout->addWidget(m_tempAmb);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);
	widget->setStyleSheet("background-color: black");

	setCentralWidget(widget);
}
