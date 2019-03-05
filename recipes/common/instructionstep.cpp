#include "instructionstep.h"

InstructionStep::InstructionStep()
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
