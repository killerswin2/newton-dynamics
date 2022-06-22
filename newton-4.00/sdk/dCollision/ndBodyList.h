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

#ifndef __ND_BODY_LIST_H__
#define __ND_BODY_LIST_H__

#include "ndCollisionStdafx.h"

class ndBodyKinematic;

template<class T >
class ndListView : public ndList<T*, ndContainersFreeListAlloc<T*>>
{
	public:
	ndListView()
		:ndList<T*, ndContainersFreeListAlloc<T*>>()
		,m_view(1024)
		,m_listIsDirty(1)
	{
	}

	ndListView(const ndListView& src)
		:ndList<T*, ndContainersFreeListAlloc<T*>>()
		,m_view()
		,m_listIsDirty(1)
	{
		ndListView* const stealData = (ndListView*)&src;
		ndNode* nextNode;
		for (ndNode* node = stealData->GetFirst(); node; node = nextNode)
		{
			nextNode = node->GetNext();
			stealData->Unlink(node);
			Append(node);
		}
		m_view.Swap(stealData->m_view);
	}

	ndNode* AddItem(T* const item)
	{
		m_listIsDirty = 1;
		return Append(item);
	}

	void RemoveItem(ndNode* const node)
	{
		m_listIsDirty = 1;
		Remove(node);
	}

	bool UpdateView()
	{
		bool ret = false;
		if (m_listIsDirty)
		{
			ret = true;
			m_listIsDirty = 0;
			m_view.SetCount(GetCount());
			ndInt32 index = 0;
			for (ndNode* node = GetFirst(); node; node = node->GetNext())
			{
				m_view[index] = node->GetInfo();
				index++;
			}
		}
		return ret;
	}

	ndArray<T*> m_view;
	ndUnsigned8 m_listIsDirty;
};


class ndBodyList: public ndListView<ndBodyKinematic>
{
	public:
	ndBodyList()
		:ndListView<ndBodyKinematic>()
	{
	}

	ndBodyList(const ndBodyList& src)
		:ndListView<ndBodyKinematic>(src)
	{
	}
};


#endif