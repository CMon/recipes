#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <QObject>

class RPCClient;

class ClientController : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)

public:
	explicit ClientController(RPCClient * rpcClient, QObject *parent = nullptr);

	Q_INVOKABLE bool connectToServer(const QString url, const bool waitForConnected);
	Q_INVOKABLE bool connectToServer(const QString & host, const quint16 port, const bool secure, const bool waitForConnected);
	Q_INVOKABLE bool isDebugBuild() const;

	QString status() const;

public slots:
	void setStatus(QString status);

signals:
	void statusChanged(QString status);

private slots:
	void handleMessageFromServer(const QVariantHash & map);
	void updateConnectedState();

private:
	void handleCallbackResponse(const QString & signature, const QJsonValue & rawData);
	void handleNotificationResponse(const QString & notification, const QJsonValue & rawData);

private:
	RPCClient * rpcClient_;

	QString status_;
};

#endif // CLIENTCONTROLLER_H
