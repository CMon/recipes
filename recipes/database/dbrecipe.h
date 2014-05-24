#pragma once

#include <common/unit.h>

namespace DB {

void addOrUpdateUnit(const Unit & unit);
bool addOrUpdateUnitTranslations(const int unitId, const Unit & unit);
QList<Unit> getUnits();

}
