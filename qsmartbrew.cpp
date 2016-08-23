#include <qboxlayout.h>
#include <qlabel.h>
#include <qtimer.h>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QUrlQuery>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "qsmartbrew.h"

#define DEFAULT_TIMEOUT_SECONDS 60

QSmartBrew::QSmartBrew(QWidget *parent)
	: QMainWindow(parent)
{
	m_timeout = DEFAULT_TIMEOUT_SECONDS;

	layoutWindow();

	startTimer(1000);
}

void QSmartBrew::sendTemperatures()
{
	// Setup the webservice url
	QUrl serviceUrl = QUrl("http://philr.biz/smartbrew/record.php");
	QByteArray postData;

	QUrl params;
	QUrlQuery query;
	quint64 timeInt = QDateTime::currentMSecsSinceEpoch();
	QString deviceID = "smartfv_pi1";
	query.addQueryItem("deviceID","string1");
	query.addQueryItem("tAmbient",QString::number(m_tempAmb));
	query.addQueryItem("tFV",QString::number(m_tempFV));
	query.addQueryItem("time",QString::number(timeInt));

	params.setQuery(query);

	postData = params.toEncoded(QUrl::RemoveFragment);

	// Call the webservice
	QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
	        SLOT(onPostAnswer(QNetworkReply*)));

	QNetworkRequest networkRequest(serviceUrl);
	networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

	networkManager->post(networkRequest,postData);
}

void QSmartBrew::getTemperatures()
{
	// FV...
	QFile inputFileFV("/sys/bus/w1/devices/28-0216011c53ff/w1_slave");
	if (inputFileFV.open(QIODevice::ReadOnly))
	{
	   QTextStream inFV(&inputFileFV);
	   while (!inFV.atEnd())
	   {
	      QString line = inFV.readLine();
				QString isValid = line.right(4); // line contains "YES " if next line is valid temp

				if (isValid.compare("YES ") == 0)
				{
					line = inFV.readLine();
					QString tempFVraw = line.right(6);
					QString tempFVformatted = tempFVraw.left(2) + "." + tempFVraw.right(4);
					m_tempFV = tempFVformatted.toFloat();
				}
	   }
	   inputFileFV.close();
	}

	// Amb...
	QFile inputFileAmb("/sys/bus/w1/devices/28-021571c028ff/w1_slave");
	if (inputFileAmb.open(QIODevice::ReadOnly))
	{
		 QTextStream inAmb(&inputFileAmb);
		 while (!inAmb.atEnd())
		 {
				QString line = inAmb.readLine();
				QString isValid = line.right(4); // line contains "YES " if next line is valid temp

				if (isValid.compare("YES ") == 0)
				{
					line = inAmb.readLine();
					QString tempFVraw = line.right(6);
					QString tempFVformatted = tempFVraw.left(2) + "." + tempFVraw.right(4);
					m_tempFV = tempFVformatted.toFloat();
				}
		 }
		 inputFileAmb.close();
	}
}

void QSmartBrew::timerEvent(QTimerEvent *)
{
	m_timeout--;
	getTemperatures();
	//sendTemperatures();

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
	layout->addWidget(m_tempAmbLabel);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);
	widget->setStyleSheet("background-color: black");

	setCentralWidget(widget);
}
