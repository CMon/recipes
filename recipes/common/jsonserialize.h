#pragma once

#include <QObject>
#include <QJsonObject>

class JsonSerialize : public QObject
{
	Q_OBJECT
public:
	QJsonObject toJson() const;
	void fromJson(const QJsonObject & obj);
};
