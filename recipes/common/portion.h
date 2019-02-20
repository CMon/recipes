#pragma once

#include <cereal/cereal.hpp>
#include <recipes/common/locale2string.h>

class Portion
{
public:
	Portion();

	bool isNull() const;

	void updateDescriptions(const QLocale & locale, const QString & description);
	Locale2String getDescriptions() const { return description; }

	void setCount(int count) { this->count = count; }
	int getCount() const { return count; }

	bool operator ==(const Portion & rhs) const;
	bool operator !=(const Portion & rhs) const;

	QString toString() const;

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("count", count),
		   cereal::make_nvp("description", description));
	}

private:
	int count;
	Locale2String description;
};
