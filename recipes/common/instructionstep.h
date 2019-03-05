#pragma once

#include <recipes/common/locale2string.h>

#include <QString>

#include <rpclib/common/types/types.h>
#include <cereal/cereal.hpp>

class InstructionStep
{
public:
	InstructionStep();

	quint8 getPosition() const;
	void setPosition(const quint8 & value);

	QString getPicturePath() const;
	void setPicturePath(const QString & value);

	Locale2String getDescription() const;
	Locale2String & getDescription();
	void setDescription(const Locale2String & value);

	bool operator ==(const InstructionStep & rhs) const;
	bool operator !=(const InstructionStep & rhs) const;

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("position", position),
		   cereal::make_nvp("picturePath", picturePath),
		   cereal::make_nvp("description", description)
		   );
	}

private:
	quint8 position;
	QString picturePath;
	Locale2String description;
};
CEREAL_CLASS_VERSION(InstructionStep, 1);
