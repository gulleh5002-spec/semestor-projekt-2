#pragma once

#include "BuildPlan.h"

#include <iosfwd>

namespace BuildPlanConsolePrinter
{
void print(const BuildPlan& buildPlan, std::ostream& output);
}
