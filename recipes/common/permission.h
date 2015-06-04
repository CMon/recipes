#pragma once

#include <recipes/common/util.h>

#include <cflib/serialize/serialize.h>

#include <QFlags>

class Permission
{
	SERIALIZE_CLASS
	Q_GADGET

public:
	enum Type {
		NoPermission = 0,
		Admin        = 1 << 0,
	};
	Q_ENUMS(Type)

	Permission() : perm(NoPermission) {}
	Permission(uint perm) : perm(perm) {}
	operator uint() const { return perm; }

	bool operator==(const Permission & rhs) const { return perm == rhs.perm; }
	bool operator!=(const Permission & rhs) const { return !operator==(rhs); }

	Q_ENUM_getAll(Permission, Type)
	Q_ENUM_toString(Permission, Type, perm)

private serialized:
	uint perm;
};

class Permissions
{
	SERIALIZE_CLASS

public:
	Permissions() {}
	Permissions(const Permission & perm);
	Permissions(const QSet<Permission> & permissions);

	bool contains(const Permission & permission) const;
	void insert(const Permission & permission);

	static Permissions fromFlags(const uint perm);
	static Permissions fromFlags(const QFlags<Permission> permissions);
	QFlags<Permission> convertToFlags() const;

	bool operator==(const Permissions & rhs) const;
	bool operator!=(const Permissions & rhs) const;

	QString toString() const;

private serialized:
	QSet<Permission> permissions_;
};

namespace QtPrivate { template <> struct is_unsigned<Permission> : integral_constant<bool, true> {}; }
