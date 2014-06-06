#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H
#include "wgraph/wtree.h"
#include "wgraph/wnode.h"
#include "3d/transform.h"
#include "3d/graphicobject3d.h"

class SceneGraphNode;
class SceneGraphElement
{
protected:
    SceneGraphNode * _tree_node;
};

class SceneGraphNode : public WNode<SceneGraphElement>
{

};
class SceneGraph : public WTree<SceneGraphElement>
{
public:
    SceneGraph();
};

class WTransform : public GraphicObject3D,SceneGraphElement
{
public:
    Transform get_world_transform() const {
//        return _transform * (WTransform*)(_tree_node->get_data())->get_world_transform();
        return Transform();
//                form * ((WTransform*)_tree_node->get_data())->get_world_transform();
    }
};


#endif // SCENEGRAPH_H

