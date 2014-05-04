#include "user.h"

User::User()
    : isDeleted_(false)
{
}

User::User(const UserId &id, const QString &login, const Permissions &permissions, const QString &firstName, const QString &lastName, bool isDeleted)
    : id_(id),
      login_(login),
      permissions_(permissions),
      firstName_(firstName),
      lastName_(lastName),
      isDeleted_(isDeleted)
{
}

bool User::isNull() const
{
    return id_.isNull();
}

bool User::hasPermission(const Permission &permission) const
{
    return permissions_.testFlag(permission);
}

bool User::isValid() const
{
    return id_.isValid() && !login_.isEmpty();
}

QString User::toString() const
{
    return QString("%1 (%2)").arg(login_).arg(id_.toString());
}
