#include "inexor/flowgraph/comments/fl_comment.hpp"

namespace inexor {
namespace vscript {

    CCommentNode::CCommentNode(vec position,
                               const char* comment, 
                               const char* name)
    {
        pos = position;
        type = NODE_TYPE_COMMENT;
        node_name = name;
        node_comment = comment;
        default_box_color = VSCRIPT_COLOR_COMMENT;
        box_color = default_box_color;
    }


    CCommentNode::~CCommentNode()
    {
    }

    bool CCommentNode::OnLinkAsChildNodeAttempt(CScriptNode* parent)
    {
        conoutf(CON_DEBUG, "You cannot link a comment as child node!");
        return false;
    }

    bool CCommentNode::OnLinkAsParentNodeAttempt(CScriptNode* child)
    {
        conoutf(CON_DEBUG, "You cannot link a comment as parent node!");
        return false;
    }

};
};