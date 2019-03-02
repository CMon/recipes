#pragma once

#include <cereal/cereal.hpp>
#include <QSet>

#include <rpclib/common/types/qsetcereal.h>

class Permissions
{
public:
	enum Permission {
		Administrator = 1,
	};

	Permissions();
	Permissions(uint perms);

	bool operator==(const Permissions & rhs) const;
	bool operator!=(const Permissions & rhs) const;

	bool hasPermission(const Permissions::Permission & permission) const;
	void addPermission(const Permissions::Permission & permission);
	void removePermission(const Permissions::Permission & permission);

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("perm", perms_));
	}

	inline QSet<int>::iterator begin() { return perms_.begin(); }
	inline QSet<int>::iterator end() { return perms_.end(); }
	inline QSet<int>::const_iterator begin() const { return perms_.begin(); }
	inline QSet<int>::const_iterator end() const { return perms_.end(); }

private:
	QSet<int> perms_;
};
CEREAL_CLASS_VERSION(Permissions, 1);
