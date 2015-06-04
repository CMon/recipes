#pragma once

#include <QLocale>
#include <QMetaEnum>
#include <QString>
#include <QSet>

namespace Util
{
bool enableConsoleEcho(const bool enable);
bool enableConsoleBufferedIo(const bool enable);
QString enterPassword(const QString & prompt);
QString boolToString(const bool b);
}

uint qHash(const QLocale & locale);

#define Q_ENUM_getAll(CLASS, ENUM) \
	static QSet<CLASS> getAll() {\
		QSet<CLASS> retSet;\
		const QMetaEnum me = CLASS::staticMetaObject.enumerator(CLASS::staticMetaObject.indexOfEnumerator(#ENUM));\
		Q_ASSERT(me.keyCount() > 0);\
		for (int i = 0; i < me.keyCount(); ++i) {\
			retSet << static_cast<ENUM>(me.value(i));\
		}\
		return retSet;\
	}

#define Q_ENUM_toString(CLASS, ENUM, MEMBER) \
	QString toString() const { \
		return CLASS::staticMetaObject.enumerator(CLASS::staticMetaObject.indexOfEnumerator(#ENUM)).valueToKey(MEMBER);\
	}
