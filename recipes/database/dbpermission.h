#pragma once

#include <recipes/common/permissiondata.h>

#include <QList>

namespace DB {

QList<PermissionData> getPermissions();
int addPermission(const QString & techName);
bool setPermissionTitle(int permissionId, const Locale2String & title);
bool setPermissionDescription(int permissionId, const Locale2String & description);

}
