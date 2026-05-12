#ifndef BUILDPLANCONSOLEPRINTER_H
#define BUILDPLANCONSOLEPRINTER_H

#include "BuildPlan.h"

#include <iosfwd>

namespace BuildPlanConsolePrinter
{
    void print(const BuildPlan& buildPlan, std::ostream& output);
}

#endif // BUILDPLANCONSOLEPRINTER_H
