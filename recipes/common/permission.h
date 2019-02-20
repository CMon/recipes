#pragma once

#include <cereal/cereal.hpp>

class Permissions
{
public:
	enum Permission {
		NoPermission = 0,
		Admin        = 1 << 0,
	};

	Permissions();
	Permissions(uint perms);

	bool operator==(const Permissions & rhs) const;
	bool operator!=(const Permissions & rhs) const;
	operator uint() const;

	bool hasPermission(const Permissions::Permission & permission) const;
	void addPermission(const Permissions::Permission & permission);
	void removePermission(const Permissions::Permission & permission);

	uint toUint() const;

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("perm", perms_));
	}

private:
	uint perms_;
};
CEREAL_CLASS_VERSION(Permissions, 1);
