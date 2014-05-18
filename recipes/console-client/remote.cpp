#include "remote.h"

#include <cflib/util/log.h>
#include <common/waiter.h>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextStream>
#include <QString>
#include <QNetworkAccessManager>
#include <QRegExp>

USE_LOG(LogCat::Network)

Remote::Remote(QObject *parent)
    :
      QObject(parent),
      networkManager_(new QNetworkAccessManager(this))
{
}

void Remote::connectTo(const int port, const QHostAddress &serverAddress)
{
	baseURL_ = "http://" + serverAddress.toString() + ":" + QString::number(port);
}

QString Remote::getBaseUrl()
{
	return baseURL_;
}

QStringList Remote::getServices()
{
	QNetworkRequest request(baseURL_ + "/api/services/");
	Waiter waiter;

	QNetworkReply * reply = networkManager_->get(request);
	connect(reply, &QNetworkReply::finished, &waiter, &Waiter::succeed);

	waiter.wait(35 * 1000);
	if (waiter.timedOut()) {
		logInfo("Request timedout");
		return QStringList();
	}

	return parseServiceResponse(reply->readAll());
}

QStringList Remote::getMethodSignatures(const QString &service)
{
	const QNetworkRequest request(baseURL_ + "/api/services/" + service.toLower());
	Waiter waiter;

	QNetworkReply * reply = networkManager_->get(request);
	connect(reply, &QNetworkReply::finished, &waiter, &Waiter::succeed);

	waiter.wait(35 * 1000);
	if (waiter.timedOut()) {
		logInfo("Request timedout");
		return QStringList();
	}

	return parseMethodResponse(reply->readAll());
}

QStringList Remote::parseServiceResponse(const QString &reply)
{
	QRegExp liExp("<li><a.*>(.*)</a></li>", Qt::CaseInsensitive, QRegExp::RegExp2);
	liExp.setMinimal(true);

	QStringList services;
	int pos = 0;
	while ((pos = liExp.indexIn(reply, pos)) != -1) {
		services << liExp.cap(1);
		pos += liExp.matchedLength();
	}

	return services;
}

QStringList Remote::parseMethodResponse(const QString &reply)
{
	QRegExp liExp("<li>(.*)</li>", Qt::CaseInsensitive, QRegExp::RegExp2);
	liExp.setMinimal(true);

	QStringList methods;
	int pos = 0;
	while ((pos = liExp.indexIn(reply, pos)) != -1) {
		methods << liExp.cap(1);
		pos += liExp.matchedLength();
	}

	return methods;
}
