#include <recipes/common/permission.h>

Permissions::Permissions()
{
}

bool Permissions::operator==(const Permissions & rhs) const
{
	return perms_ == rhs.perms_;
}

bool Permissions::operator!=(const Permissions & rhs) const
{
	return !operator==(rhs);
}

bool Permissions::hasPermission(const Permissions::Permission & permission) const
{
	return perms_.contains(permission);
}

void Permissions::addPermission(const Permissions::Permission & permission)
{
	perms_ << permission;
}

void Permissions::removePermission(const Permissions::Permission & permission)
{
	perms_.remove(permission);
}
