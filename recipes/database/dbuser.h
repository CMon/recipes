#pragma once

#include <QList>

class User;
class UserId;
namespace DB {

bool updateUser(const User & user, const QString password);
bool updatePassword(const QString & login, const QString & password);

User getUser(const QString & login);
UserId getUserId(const QString & login);

bool checkPassword(const QString & login, const QString & password);
QList<User> getAllUsers(const int & id = -1);

}
