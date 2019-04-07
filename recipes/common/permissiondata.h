#pragma once

#include <recipes/common/locale2string.h>

#include <cereal/cereal.hpp>
#include <rpclib/common/types/qsetcereal.h>

#include <QDebug>

class PermissionData
{
public:
	PermissionData();
	PermissionData(int id, const QString & techName = QString(), const Locale2String & title = Locale2String(), const Locale2String & description = Locale2String());

	bool isValid() const;

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("permissionId", permissionId_),
		   cereal::make_nvp("techName", techName_),
		   cereal::make_nvp("title", title_),
		   cereal::make_nvp("description", description_)
		   );
	}

	int permissionId() const;

	QString techName() const;
	void setTechName(const QString & techName);

	Locale2String title() const;
	void setTitle(const Locale2String & title);

	Locale2String description() const;
	void setDescription(const Locale2String & description);

	QString toString() const;

	bool operator ==(const PermissionData & rhs) const;
	bool operator !=(const PermissionData & rhs) const;

private:
	int permissionId_{-1};
	QString techName_;
	Locale2String title_;
	Locale2String description_;
};
CEREAL_CLASS_VERSION(PermissionData, 1);

QDebug operator <<(QDebug d, const PermissionData &pd);
