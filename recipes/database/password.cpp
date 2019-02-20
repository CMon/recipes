#include "password.h"

#include <QCryptographicHash>
#include <QString>

QByteArray Password::hashPassword(const QString & password)
{
	return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Keccak_512);
}

bool Password::checkPassword(const QString & password, const QByteArray & hash)
{
	QByteArray hash2 = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Keccak_512);

	return  hash == hash2;
}
