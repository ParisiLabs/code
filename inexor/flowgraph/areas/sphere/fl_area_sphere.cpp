#include "inexor/flowgraph/areas/sphere/fl_area_sphere.hpp"

namespace inexor {
namespace vscript {


    CSphereAreaNode::CSphereAreaNode(vec position, vec rad, const char* name, const char* comment)
    {
        type = NODE_TYPE_AREA_SPHERE;
        pos = position;
        radius = rad;
    }


    CSphereAreaNode::~CSphereAreaNode()
    {
    }


    void CSphereAreaNode::render(int orient, bool sel_blocked)
    {
        glBegin(GL_QUADS);
        // TODO: How to render a sphere correctly?
        glEnd();
    }

    bool CSphereAreaNode::collide(vec p)
    {
        return true;
    }

};
};