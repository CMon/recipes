#include <recipes/common/permission.h>


Permissions::Permissions(const Permission & perm)
{
	permissions_ << perm;
}

Permissions::Permissions(const QSet<Permission> & permissions)
    : permissions_(permissions)
{
}

bool Permissions::contains(const Permission & permission) const
{
	return permissions_.contains(permission);
}

void Permissions::insert(const Permission & permission)
{
	permissions_.insert(permission);
}

Permissions Permissions::fromFlags(const uint perm)
{
	QFlags<Permission> perms;
	perms |= perm;

	return fromFlags(perms);
}

Permissions Permissions::fromFlags(const QFlags<Permission> permissions)
{
	Permissions set;

	foreach (const Permission & permission, Permission::getAll()) {
		if (permissions.testFlag(permission)) set.insert(permission);
	}
	return set;
}

QFlags<Permission> Permissions::convertToFlags() const
{
	QFlags<Permission> retval;

	foreach (const Permission & permission, permissions_) {
		retval |= permission;
	}

	return retval;
}

bool Permissions::operator==(const Permissions & rhs) const
{
	return permissions_ == rhs.permissions_;
}

bool Permissions::operator!=(const Permissions & rhs) const
{
	return !operator ==(rhs);
}

QString Permissions::toString() const
{
	QString retval;
	foreach(const Permission & perm, permissions_) {
		if (!retval.isEmpty()) retval += ", ";
		retval += perm.toString();
	}
	return retval;
}
