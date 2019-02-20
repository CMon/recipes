#include <recipes/common/permission.h>


Permissions::Permissions()
    : perms_(Permission::NoPermission)
{
}

Permissions::Permissions(uint perms)
    : perms_(perms)
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

Permissions::operator uint() const
{
	return perms_;
}

bool Permissions::hasPermission(const Permissions::Permission & permission) const
{
	return perms_ & (1<<(uint(permission)));
}

void Permissions::addPermission(const Permissions::Permission & permission)
{
	perms_ |= uint(permission);
}

void Permissions::removePermission(const Permissions::Permission & permission)
{
	perms_ &= ~(uint(permission));
}

uint Permissions::toUint() const
{
	return perms_;
}
