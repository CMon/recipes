#pragma once

#include <common/unit.h>

namespace DB {

void addOrUpdateUnit(const Unit & unit);
QList<Unit> getUnits();

}
