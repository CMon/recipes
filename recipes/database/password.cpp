#include "password.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QRandomGenerator>
#include <QString>

#ifdef WITH_BOTAN
#include <botan/bcrypt.h>
#include <botan/auto_rng.h>
#endif

QString Password::getRandomPassword()
{
	static const int pwLength = 14;

	QString str;
	str.resize(pwLength);
	for (int c = 0; c < pwLength ; ++c) {
		str[c] = QChar('A' + char(QRandomGenerator::global()->generate() % ('Z' - 'A')));
	}

	return str;
}

QByteArray Password::hashPassword(const QString & password, QString & usedCrypto)
{
#ifdef WITH_BOTAN
	usedCrypto = "bcrypt";
	Botan::AutoSeeded_RNG rng;
	std::string hash = Botan::generate_bcrypt(password.toStdString(), rng, 12, 'a');
	return QByteArray::fromStdString(hash);
#else
	usedCrypto = "Keccak_512";
	return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Keccak_512).toHex();
#endif
}

bool Password::checkPassword(const QString & password, const QByteArray & originalHash, const QString & usedCrypto)
{
	if (usedCrypto == "bcrypt") {
#ifdef WITH_BOTAN
		return Botan::check_bcrypt(password.toStdString(), originalHash.toStdString());
#else
		qCritical() << "The password was hashed with bcrypt, but this library was build without botan support, can not verify password";
		return false;
#endif
	}

	const QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Keccak_512).toHex();
	return  originalHash == hash;
}
