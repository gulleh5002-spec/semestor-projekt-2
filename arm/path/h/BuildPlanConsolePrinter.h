#ifndef BUILDPLANCONSOLEPRINTER_H
#define BUILDPLANCONSOLEPRINTER_H

#include "BuildPlan.h"

#include <iosfwd>
// Hjælpe klasse til at skrive buildplan til konsol
namespace BuildPlanConsolePrinter
{
    void print(const BuildPlan& buildPlan, std::ostream& output);
}

#endif // BUILDPLANCONSOLEPRINTER_H
