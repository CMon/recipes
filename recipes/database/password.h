#ifndef PASSWORD_H
#define PASSWORD_H

#include <QByteArray>

namespace Password {

QByteArray hashPassword(const QString & password);
bool checkPassword(const QString & password, const QByteArray & hash);

}

#endif // PASSWORD_H
