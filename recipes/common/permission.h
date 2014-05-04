#pragma once

#include <cflib/serialize/serialize.h>

#include <QFlags>

class Permission
{
    SERIALIZE_CLASS
public:
    enum Type {
        Admin      = 1 << 0,
    };

    Permission(uint perm) : perm(perm) {}
    operator uint() const { return perm; }

    bool operator==(const Permission & rhs) const { return perm == rhs.perm; }
    bool operator!=(const Permission & rhs) const { return !operator==(rhs); }

private serialized:
    uint perm;
};

typedef QFlags<Permission> Permissions;

namespace QtPrivate { template <> struct is_unsigned<Permission> : integral_constant<bool, true> {}; }
