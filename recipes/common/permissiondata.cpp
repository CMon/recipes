#include "permissiondata.h"

PermissionData::PermissionData()
{

}

PermissionData::PermissionData(int id, const QString & techName, const Locale2String & title, const Locale2String & description)
    : permissionId_(id)
    , techName_(techName)
    , title_(title)
    , description_(description)
{
}

bool PermissionData::isValid() const
{
	return permissionId_ >= 0;
}

int PermissionData::permissionId() const
{
	return permissionId_;
}

QString PermissionData::techName() const
{
	return techName_;
}

void PermissionData::setTechName(const QString & techName)
{
	techName_ = techName;
}

Locale2String PermissionData::title() const
{
	return title_;
}

void PermissionData::setTitle(const Locale2String & title)
{
	title_ = title;
}

Locale2String PermissionData::description() const
{
	return description_;
}

void PermissionData::setDescription(const Locale2String & description)
{
	description_ = description;
}

QString PermissionData::toString() const
{
	return QString("id: %1; techName: %2; title: [%3]; description: [%4]")
	        .arg(permissionId_)
	        .arg(techName_)
	        .arg(title_.toString())
	        .arg(description_.toString());
}

bool PermissionData::operator ==(const PermissionData & rhs) const
{
	return
	        permissionId_ == rhs.permissionId_ &&
	        techName_     == rhs.techName_     &&
	        title_        == rhs.title_        &&
	        description_  == rhs.description_;
}

bool PermissionData::operator !=(const PermissionData & rhs) const
{
	return !operator==(rhs);
}

QDebug operator <<(QDebug d, const PermissionData & pd) {
	d << pd.toString();
	return d;
}
