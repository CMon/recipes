#ifndef PASSWORD_H
#define PASSWORD_H

#include <QByteArray>

namespace Password {

QByteArray hashPassword(const QString & password, QString & usedCrypto);
bool checkPassword(const QString & password, const QByteArray & originalHash, const QString & usedCrypto);

}

#endif // PASSWORD_H
