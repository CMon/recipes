#pragma once

#include <recipes/common/permissiondata.h>

#include <QTest>

// usage: COMPARE_OP(17, <, 1); this could be done with a verify, but the verify does not printout the the values themself
#define COMPARE_OP(actual, operation, expected) \
    QVERIFY2((actual) operation (expected), qPrintable(QString("%1 %2 %3").arg(actual).arg(#operation).arg(expected)))

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
