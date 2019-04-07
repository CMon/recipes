#include "dbpermission.h"

#include <recipes/database/database.h>

#include <QSqlQuery>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(DB_PERMISSION)
Q_LOGGING_CATEGORY(DB_PERMISSION, "recipe.db.permission")

QList<PermissionData> DB::getPermissions()
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	query.prepare(
	            "SELECT "
	                "p.id, p.techName, i18n.language, i18n.title, i18n.description "
	            "FROM "
	                "permission p LEFT JOIN permission_i18n i18n "
	            "ON "
	                "p.id = i18n.permissionId"
	            );

	if (!Database::executeQuery(query)) return QList<PermissionData>();

	QHash<int, PermissionData> dummy;
	while (query.next()) {
		const int permissionId = query.value(0).toInt();
		const QString techName = query.value(1).toString();

		if(!dummy.contains(permissionId)){
			dummy[permissionId] = PermissionData(permissionId, techName);
		}
		if (!query.isNull(2)) {
			const QLocale locale   = QLocale(query.value(2).toString());

			if (!query.isNull(3)) {
				Locale2String origTitle = dummy[permissionId].title();
				origTitle.add(locale, query.value(3).toString());
				dummy[permissionId].setTitle(origTitle);
			}
			if (!query.isNull(4)) {
				Locale2String origDescription = dummy[permissionId].description();
				origDescription.add(locale, query.value(4).toString());
				dummy[permissionId].setDescription(origDescription);
			}
		}
	}

	if (!ta.commit()) return QList<PermissionData>();

	return dummy.values();
}

int DB::addPermission(const QString & techName)
{
	if (techName.isEmpty()) {
		return -1;
	}

	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	qCInfo(DB_PERMISSION) << "Adding Permission:" << techName;
	query.prepare(
	            "INSERT INTO "
	                "permission "
	            "("
	                "techName "
	            ") VALUES ("
	                ":techName "
	            ")"
	            );
	query.bindValue(":techName", techName);

	if (!Database::executeQuery(query)) return -1;

	const int id = query.lastInsertId().toInt();
	if (!ta.commit()) return -1;

	return id;
}

bool DB::setPermissionTitle(int permissionId, const Locale2String & title)
{
	if (title.isEmpty()) {
		return false;
	}

	TRANSACTION(ta);

	QSqlQuery query(ta.db);
	query.prepare(
	        "INSERT INTO "
	          "permission_i18n "
	        "("
	          "permissionId, language, title "
	        ") VALUES ("
	          ":permissionId, :language, :title "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "title=VALUES(title) "
	            );
	query.bindValue(":permissionId", permissionId);

	for (const QLocale & locale: title.keys()) {
		query.bindValue(":language", locale.name());
		query.bindValue(":title", title.value(locale));
		Database::executeQuery(query);
	}

	return ta.commit();
}

bool DB::setPermissionDescription(int permissionId, const Locale2String & description)
{
	if (description.isEmpty()) {
		return false;
	}

	TRANSACTION(ta);

	QSqlQuery query(ta.db);
	query.prepare(
	        "INSERT INTO "
	          "permission_i18n "
	        "("
	          "permissionId, language, description "
	        ") VALUES ("
	          ":permissionId, :language, :description "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "description=VALUES(description) "
	            );
	query.bindValue(":permissionId", permissionId);

	for (const QLocale & locale: description.keys()) {
		query.bindValue(":language", locale.name());
		query.bindValue(":description", description.value(locale));
		Database::executeQuery(query);
	}

	return ta.commit();
}
