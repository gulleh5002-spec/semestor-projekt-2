#ifndef WORKSPACEDATAJSONHANDLER_H
#define WORKSPACEDATAJSONHANDLER_H

#include "robotworkspace.h"

#include <QJsonObject>
#include <QString>

class WorkspaceDataJsonHandler
{
public:
    static QJsonObject toJson(const RobotWorkspace& workspace);
    static QString toJsonString(const RobotWorkspace& workspace);
};

#endif // WORKSPACEDATAJSONHANDLER_H
