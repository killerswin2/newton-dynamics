/* Copyright (c) <2003-2021> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "ndCoreStdafx.h"
#include "ndCollisionStdafx.h"
#include "ndBodyKinematic.h"
#include "ndSceneNode.h"

#define D_AABB_QUANTIZATION		ndFloat32 (4.0f)
#define D_AABB_INV_QUANTIZATION	(ndFloat32 (1.0f) / D_AABB_QUANTIZATION)

ndVector ndSceneNode::m_aabbQuantization(D_AABB_QUANTIZATION, D_AABB_QUANTIZATION, D_AABB_QUANTIZATION, ndFloat32 (0.0f));
ndVector ndSceneNode::m_aabbInvQuantization(D_AABB_INV_QUANTIZATION, D_AABB_INV_QUANTIZATION, D_AABB_INV_QUANTIZATION, ndFloat32(0.0f));

ndSceneBodyNode::ndSceneBodyNode(ndBodyKinematic* const body)
	:ndSceneNode(nullptr)
	,m_body(body)
{
#ifdef _DEBUG
	static ndInt32 nodeId = 0;
	m_nodeId = nodeId;
	nodeId++;
#endif

	SetAabb(body->m_minAabb, body->m_maxAabb);
	m_body->SetSceneBodyNode(this);
}

ndSceneBodyNode::~ndSceneBodyNode()
{
	m_body->SetSceneBodyNode(nullptr);
}

ndSceneTreeNode::ndSceneTreeNode()
	:ndSceneNode(nullptr)
	,m_left(nullptr)
	,m_right(nullptr)
	,m_fitnessNode(nullptr)
{
	dAssert(0);
#ifdef _DEBUG
	static ndInt32 nodeId = 1000000;
	m_nodeId = nodeId;
	nodeId++;
#endif
}

ndSceneTreeNode::ndSceneTreeNode(ndSceneNode* const sibling, ndSceneNode* const myNode)
	:ndSceneNode(sibling->m_parent)
	,m_left(sibling)
	,m_right(myNode)
	,m_fitnessNode(nullptr)
{
#ifdef _DEBUG
	static ndInt32 nodeId = 1000000;
	m_nodeId = nodeId;
	nodeId++;
#endif

	if (m_parent) 
	{
		ndSceneTreeNode* const myParent = (ndSceneTreeNode*)m_parent;
		if (myParent->m_left == sibling) 
		{
			myParent->m_left = this;
		} 
		else 
		{
			dAssert(myParent->m_right == sibling);
			myParent->m_right = this;
		}
	}

	sibling->m_parent = this;
	myNode->m_parent = this;

	ndSceneNode* const left = m_left;
	ndSceneNode* const right = m_right;

	m_minBox = left->m_minBox.GetMin(right->m_minBox);
	m_maxBox = left->m_maxBox.GetMax(right->m_maxBox);
}

ndSceneTreeNode::~ndSceneTreeNode()
{
	if (m_left) 
	{
		delete m_left;
	}
	if (m_right) 
	{
		delete m_right;
	}
}

bool ndSceneTreeNode::SanityCheck(ndUnsigned32 level) const
{
	dAssert(m_left->m_parent == this);
	dAssert(m_right->m_parent == this);

	ndSceneNode::SanityCheck(level);
	m_left->SanityCheck(level + 1);
	m_right->SanityCheck(level + 1);
	return true;
}

#ifdef _DEBUG
bool ndSceneNode::SanityCheck(ndUnsigned32 level) const
{
	char margin[256];
	for (ndUnsigned32 i = 0; i < level; ++i)
	{
		margin[i * 2] = ' ';
		margin[i * 2 + 1] = ' ';
	}
	margin[level * 2] = 0;
	//dTrace(("%s nodeId:%d  dethth:%d\n", margin, m_nodeId, m_depthLevel));
	dAssert(!m_parent || (m_depthLevel < m_parent->m_depthLevel));
	dAssert(!m_parent || dBoxInclusionTest(m_minBox, m_maxBox, m_parent->m_minBox, m_parent->m_maxBox));
	return true;
}

#else
bool ndSceneNode::SanityCheck(ndUnsigned32) const
{
	return true;
}
#endif
