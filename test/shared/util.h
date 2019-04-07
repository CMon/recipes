#pragma once

#include <recipes/common/permissiondata.h>

#include <QTest>

namespace QTest {
inline bool qCompare(PermissionData const &t1, PermissionData const &t2,
                     const char *actual, const char *expected, const char *file, int line)
{
	return QTest::qCompare(t1.toString(), t2.toString(), actual, expected, file, line);
}

inline bool qCompare(Locale2String const &t1, Locale2String const &t2,
                     const char *actual, const char *expected, const char *file, int line)
{
	return QTest::qCompare(t1.toString(), t2.toString(), actual, expected, file, line);
}

}
