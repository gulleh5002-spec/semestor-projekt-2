#ifndef DEBUGHELPER_H
#define DEBUGHELPER_H

#include "robotworkspace.h"


/*
 Debug helper klasse, til håndtering af debug funktioner
  tilgår funktionalitet ved: DebugHelper::'metode();'
 */
class DebugHelper
{
public:
    // Debug for workspace
    static void workspaceCreated(const RobotWorkspace& workspace);
    // Debug for fjerndesle af workspace
    static void workspaceCleared();
    // Debug af lag ændringer
    static void layerChanged(const RobotWorkspace& workspace);
    // debug placering af klodser
    static void blockPlacementUpdated(const RobotWorkspace& workspace, int x, int y);
    // Debug af ugyldig placering
    static void blockPlacementRejected(const RobotWorkspace& workspace, int x, int y);
};

#endif
