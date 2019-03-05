#include "instructionstep.h"

InstructionStep::InstructionStep()
{
}

InstructionStep::InstructionStep(quint8 position, const Locale2String & description, const QString & picturePath)
    : position(position)
    , picturePath(picturePath)
    , description(description)
{
}

quint8 InstructionStep::getPosition() const
{
	return position;
}

void InstructionStep::setPosition(const quint8 & value)
{
	position = value;
}

QString InstructionStep::getPicturePath() const
{
	return picturePath;
}

void InstructionStep::setPicturePath(const QString & value)
{
	picturePath = value;
}

Locale2String InstructionStep::getDescription() const
{
	return description;
}

Locale2String &InstructionStep::getDescription()
{
	return description;
}

void InstructionStep::addDescription(const Locale2String & value)
{
	description.add(value);
}

void InstructionStep::setDescription(const Locale2String & value)
{
	description = value;
}

bool InstructionStep::operator ==(const InstructionStep & rhs) const
{
	return
	        position    == rhs.position    &&
	        picturePath == rhs.picturePath &&
	        description == rhs.description
	        ;
}

bool InstructionStep::operator !=(const InstructionStep & rhs) const
{
	return !operator ==(rhs);
}
