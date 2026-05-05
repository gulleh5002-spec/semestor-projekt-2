#include "workspacedatajsonhandler.h"

#include "blockplacement.h"
#include "blocktype.h"

#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>

#include <vector>

namespace
{
QString blockTypeToString(BlockType blockType)
{
    switch (blockType) {
    case BlockType::Default:
        return "Default";
    case BlockType::Unknown:
        return "Unknown";
    }

    return "Unknown";
}

QJsonObject blockToJson(const BlockPlacement& block)
{
    QJsonObject blockObject;
    blockObject["x"] = block.position.x;
    blockObject["y"] = block.position.y;
    blockObject["z"] = block.position.z;
    blockObject["type"] = blockTypeToString(block.blockType);

    return blockObject;
}

QJsonArray blocksToJson(const std::vector<BlockPlacement>& blocks)
{
    QJsonArray blocksArray;

    for (const BlockPlacement& block : blocks) {
        blocksArray.append(blockToJson(block));
    }

    return blocksArray;
}
}

QJsonObject WorkspaceDataJsonHandler::toJson(const RobotWorkspace& workspace)
{
    QJsonObject workspaceObject;
    workspaceObject["isCreated"] = workspace.isCreated();
    workspaceObject["width"] = workspace.width();
    workspaceObject["height"] = workspace.height();
    workspaceObject["layerCount"] = workspace.layerCount();

    QJsonObject rootObject;
    rootObject["workspace"] = workspaceObject;
    rootObject["blocks"] = blocksToJson(workspace.placedBlocks());

    return rootObject;
}

QString WorkspaceDataJsonHandler::toJsonString(const RobotWorkspace& workspace)
{
    const QJsonDocument document{toJson(workspace)};
    return QString::fromUtf8(document.toJson(QJsonDocument::Indented));
}

bool WorkspaceDataJsonHandler::saveToFile(const RobotWorkspace& workspace, const QString& filePath)
{
    QFile file{filePath};

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }

    const QJsonDocument document{toJson(workspace)};
    const QByteArray jsonData = document.toJson(QJsonDocument::Indented);

    return file.write(jsonData) == jsonData.size();
}
