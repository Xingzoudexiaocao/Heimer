// This file is part of Heimer.
// Copyright (C) 2018 Jussi Lind <jussi.lind@iki.fi>
//
// Heimer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// Heimer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Heimer. If not, see <http://www.gnu.org/licenses/>.

#ifndef EDITOR_DATA_HPP
#define EDITOR_DATA_HPP

#include <memory>
#include <set>
#include <utility>
#include <vector>

#include <QObject>
#include <QPointF>
#include <QString>
#include <QTimer>

#include "copy_context.hpp"
#include "edge.hpp"
#include "file_exception.hpp"
#include "grid.hpp"
#include "mind_map_data.hpp"
#include "mouse_action.hpp"
#include "node.hpp"
#include "types.hpp"
#include "undo_stack.hpp"

class Node;
class NodeBase;
class MindMapTile;
class SelectionGroup;
class QGraphicsLineItem;

class EditorData : public QObject
{
    Q_OBJECT

public:
    EditorData();

    ~EditorData() override;

    EdgeS addEdge(EdgeS edge);

    void addNodeToSelectionGroup(NodeR node);

    //! \return true if at least one selected node pair can be connected.
    bool areSelectedNodesConnectable() const;

    //! \return true if at least one selected node pair can be disconnected.
    bool areSelectedNodesDisconnectable() const;

    void deleteEdge(EdgeR edge);

    void deleteEdge(int index0, int index1);

    void deleteNode(NodeR node);

    void deleteSelectedNodes();

    NodeS addNodeAt(QPointF pos);

    void clearCopyStack();

    void clearSelectionGroup();

    //! Connects selected nodes in the order they were selected.
    //! \return the new edge objects.
    std::vector<EdgeS> connectSelectedNodes();

    //! Disconnects (deletes edges) directly connected nodes in the group if possible.
    void disconnectSelectedNodes();

    CopyContext::CopiedData copiedData() const;

    NodeS copyNodeAt(NodeCR source, QPointF pos);

    void copySelectedNodes();

    size_t copyStackSize() const;

    QColor backgroundColor() const;

    MouseAction & mouseAction();

    QString fileName() const;

    NodeS getNodeByIndex(int index);

    void initializeNewMindMap();

    bool isInSelectionGroup(NodeR node);

    bool isUndoable() const;

    bool isRedoable() const;

    bool isModified() const;

    void loadMindMapData(QString fileName);

    MindMapDataPtr mindMapData();

    void moveSelectionGroup(NodeR reference, QPointF location);

    bool nodeHasImageAttached() const;

    void redo();

    void removeImageRefsOfSelectedNodes();

    bool saveMindMap();

    bool saveMindMapAs(QString fileName);

    void saveUndoPoint(bool dontClearRedoStack = false);

    void saveRedoPoint();

    void setColorForSelectedNodes(QColor color);

    void setGridSize(int size, bool autoSnap);

    void setMindMapData(MindMapDataPtr newMindMapData);

    void setSelectedEdge(EdgeP edge);

    void setSelectedNode(NodeP node);

    void setImageRefForSelectedNodes(size_t id);

    void setTextColorForSelectedNodes(QColor color);

    EdgeP selectedEdge() const;

    NodeP selectedNode() const;

    size_t selectionGroupSize() const;

    void selectNodesByText(QString text);

    void toggleNodeInSelectionGroup(NodeR node);

    void undo();

    void unselectText();

signals:

    void isModifiedChanged(bool isModified);

    void undoEnabled(bool enable);

    void redoEnabled(bool enable);

private:
    EditorData(const EditorData & e) = delete;
    EditorData & operator=(const EditorData & e) = delete;

    void clearImages();

    using NodePairVector = std::vector<std::pair<NodeP, NodeP>>;
    NodePairVector getConnectableNodes() const;

    NodePairVector getDisconnectableNodes() const;

    void removeEdgeFromScene(EdgeR edge);

    void removeNodeFromScene(NodeR node);

    void requestAutosave();

    void sendUndoAndRedoSignals();

    void setIsModified(bool isModified);

    MouseAction m_mouseAction;

    MindMapDataPtr m_mindMapData;

    std::unique_ptr<CopyContext> m_copyContext;

    std::unique_ptr<SelectionGroup> m_selectionGroup;

    UndoStack m_undoStack;

    EdgeP m_selectedEdge = nullptr;

    NodeP m_dragAndDropNode = nullptr;

    QPointF m_dragAndDropSourcePos;

    std::vector<QGraphicsLineItem *> m_targetNodes;

    unsigned int m_activeColumn = 0;

    unsigned int m_activeRow = 0;

    bool m_isModified = false;

    QString m_fileName;

    QTimer m_undoTimer;

    Grid m_grid;
};

#endif // EDITOR_DATA_HPP
