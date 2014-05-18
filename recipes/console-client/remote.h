#pragma once

#include <QHostAddress>

class QNetworkAccessManager;

class Remote : public QObject
{
	Q_OBJECT
public:
	Remote(QObject * parent = 0);

	void connectTo(const int port, const QHostAddress & serverAddress = QHostAddress::LocalHost);
	QString getBaseUrl();

	QStringList getServices();
	QStringList getMethodSignatures(const QString & service);

private:
	QStringList parseServiceResponse(const QString &reply);
	QStringList parseMethodResponse(const QString &reply);

private:
	QString baseURL_;
	QNetworkAccessManager * networkManager_;
};
