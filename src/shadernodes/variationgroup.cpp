#include "variationgroup.h"

/*!
 * \class VariationGroup
 * \brief This defines a group of VariationNode.
 * When a property of a ShaderNode is a VariationGroup, it will cause
 * the target of the VariationGroup to be evaluated multiple times, once
 * for each VariationNode in the group.
 */

VariationGroup::VariationGroup(QNode *parent)
    : ShaderNode(parent)
{
    setName("variationgroup");
}
