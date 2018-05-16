#include "jsonserialize.h"

#include <QMetaProperty>

QJsonObject JsonSerialize::toJson() const
{
	QJsonObject retval;

	const QMetaObject* metaObj = metaObject();
	for (int i = 0; i < metaObj->propertyCount(); ++i) {
		const char * propertyName = metaObj->property(i).name();
		if (strcmp(propertyName, "objectName") == 0) continue;

		const QVariant value = property(propertyName);
		retval.insert(QString(propertyName), QJsonValue::fromVariant(value));
	}

	return retval;
}

void JsonSerialize::fromJson(const QJsonObject & obj)
{
	for(const QString & valueName: obj.keys()) {
		QJsonValue value = obj.value(valueName);
		setProperty(qPrintable(valueName), value.toVariant());
	}
}
