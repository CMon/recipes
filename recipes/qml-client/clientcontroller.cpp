#include "clientcontroller.h"

#include <recipes/qml-client/logcategory.h>
#include <rpclib/client/rpcclient.h>

#include <QJsonObject>
#include <QJsonValue>

Q_LOGGING_CATEGORY(GUI_CLIENT_CC, "client.clientcontroller")

ClientController::ClientController(RPCClient * rpcClient, QObject * parent)
    : QObject (parent)
    , rpcClient_(rpcClient)
{
	connect(rpcClient_, &RPCClient::connected, this, &ClientController::updateConnectedState);
	connect(rpcClient_, &RPCClient::disconnected, this, &ClientController::updateConnectedState);
	connect(rpcClient_, &RPCClient::newMessageArrived, this, &ClientController::handleMessageFromServer);
	connect(rpcClient_, &RPCClient::error, this, [this](const QString& error) {
		setStatus(error);
	});
}

void ClientController::connectToServer(const QString & host, const quint16 port, const bool secure)
{
	if (rpcClient_->isConnected()) {
		qCInfo(GUI_CLIENT_CC) << "Already connected not reconnecting";
		return;
	}

	if (host.isEmpty()) return;

	rpcClient_->setUrl(QString("%1://%2:%3").arg(secure ? "wss" : "ws").arg(host).arg(port));
	rpcClient_->connectToServer();
}

QString ClientController::status() const
{
	return status_;
}

void ClientController::setStatus(QString status)
{
	if (status_ == status)
		return;

	status_ = status;
	emit statusChanged(status_);
}

void ClientController::handleMessageFromServer(const QVariantHash & map)
{
	const QJsonObject message = QJsonObject::fromVariantHash(map);

	if (message.contains("signature")) {
		handleCallbackResponse(message.value("signature").toString(), message.value("data"));
	}

	if (message.contains("notification")) {
		handleNotificationResponse(message.value("notification").toString(), message.value("data"));
	}
}

void ClientController::updateConnectedState()
{
	if (rpcClient_->isConnected()) {
		setStatus("Connected");
	} else {
		setStatus("Disconnected");
	}
}

void ClientController::handleCallbackResponse(const QString & signature, const QJsonValue & rawData)
{
	Q_UNUSED(signature);
	Q_UNUSED(rawData);
}

void ClientController::handleNotificationResponse(const QString & notification, const QJsonValue & rawData)
{
	Q_UNUSED(notification);
	Q_UNUSED(rawData);
}
