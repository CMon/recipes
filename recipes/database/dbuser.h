#pragma once

#include <QList>

class User;
class UserId;
namespace DB {

/**
 * @brief addOrUpdateUser
 * Add or update the given user, if the user is new the user object will be changed to contain the userId
 *
 * @param user The updated user object
 * @param password The password to update if it is empty the password will not be changed or in case of a user creation it will be set to a random one
 * @return true if successful
 */
bool addOrUpdateUser(User & user, const QString & password);
bool resetPassword(const UserId & userId);

User getUser(const QString & login);
UserId getUserId(const QString & login);

bool checkPassword(const QString & login, const QString & password);
QList<User> getAllUsers(const int & id = -1);

}
