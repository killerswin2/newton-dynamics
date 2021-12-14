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

#ifndef __NDG_CONVEXHULL_4D__
#define __NDG_CONVEXHULL_4D__

#include "ndCoreStdafx.h"
#include "ndList.h"
#include "ndArray.h"
#include "ndPlane.h"
#include "ndVector.h"
#include "ndMatrix.h"
#include "ndQuaternion.h"

class ndConvexHull4dAABBTreeNode;

class ndConvexHull4dVector: public ndBigVector
{
	public:
	void operator = (const ndBigVector& a) 
	{
		m_x = a.m_x;
		m_y = a.m_y;
		m_z = a.m_z;
		m_w = a.m_w;
		m_index = 0;
		m_mark = 0;
	}

	dInt32 m_index;
	dInt32 m_mark;
};

class ndConvexHull4dTetraherum
{
	public:
	class ndTetrahedrumFace 
	{
		public:
		dInt32 m_index[4];
		ndList<ndConvexHull4dTetraherum>::ndNode* m_twin;
	};
	
	class ndTetrahedrumPlane: public ndBigVector
	{
		public: 
		ndTetrahedrumPlane (const ndBigVector& p0, const ndBigVector& p1, const ndBigVector& p2, const ndBigVector& p3);
		dFloat64 Evalue (const ndBigVector& point) const;
		dFloat64 m_dist;
	};

	ndConvexHull4dTetraherum();
	ndTetrahedrumPlane GetPlaneEquation (const ndConvexHull4dVector* const points) const;
	dFloat64 GetTetraVolume (const ndConvexHull4dVector* const pointArray) const;	
	D_CORE_API ndBigVector CircumSphereCenter (const ndConvexHull4dVector* const pointArray) const;
	dFloat64 Evalue (const ndConvexHull4dVector* const pointArray, const ndBigVector& point) const;

	dInt32 GetMark() const { return m_mark; }
	void SetMark(dInt32 mark) { m_mark = mark; }

	private:
	void Init (const ndConvexHull4dVector* const points, dInt32 v0, dInt32 v1, dInt32 v2, dInt32 v3);

	public:
	ndTetrahedrumFace m_faces[4];
	dInt32 m_mark;
	dInt32 m_uniqueID;

#ifdef _DEBUG
	dInt32 m_debugID;
#endif
	friend class ndConvexHull4d;
	friend class ndDelaunayTetrahedralization;
};

class ndConvexHull4d: public ndList<ndConvexHull4dTetraherum>
{
	public:
	class ndTempList: public ndList<ndNode*, ndContainersFreeListAlloc<ndNode*>>
	{
	};

	D_CORE_API ndConvexHull4d(const ndConvexHull4d& source);
	D_CORE_API ndConvexHull4d(const dFloat64* const vertexCloud, dInt32 strideInBytes, dInt32 count, dFloat64 distTol);
	D_CORE_API virtual ~ndConvexHull4d();

	dInt32 GetVertexCount() const;
	dInt32 GetVertexIndex(dInt32 i) const;
	const ndBigVector& GetVertex(dInt32 i) const;

	const ndConvexHull4dVector* GetHullVertexArray() const;
	dFloat64 GetTetraVolume (const ndConvexHull4dTetraherum* const tetra) const;

	dInt32 IncMark (); 
	void Save (const char* const filename) const;

	protected:
	ndConvexHull4d();

	void BuildHull (const dFloat64* const vertexCloud, dInt32 strideInBytes, dInt32 count, dFloat64 distTol);

	virtual dInt32 AddVertex (const ndBigVector& vertex);
	virtual dInt32 InitVertexArray(ndConvexHull4dVector* const points, const dFloat64* const vertexCloud, dInt32 strideInBytes, dInt32 count, void* const memoryPool, dInt32 maxMemSize);
	
	virtual ndNode* AddFace (dInt32 i0, dInt32 i1, dInt32 i2, dInt32 i3);
	virtual void DeleteFace (ndNode* const node);

	ndNode* FindFacingNode(const ndBigVector& vertex);
	
	void InsertNewVertex(dInt32 vertexIndex, ndNode* const frontFace, ndTempList& deletedFaces, ndTempList& newFaces);
	dInt32 SupportVertex (ndConvexHull4dAABBTreeNode** const tree, const ndConvexHull4dVector* const points, const ndBigVector& dir, const bool removeEntry = true) const;
	
	void CalculateConvexHull (ndConvexHull4dAABBTreeNode* vertexTree, ndConvexHull4dVector* const points, dInt32 count, dFloat64 distTol);
	void LinkSibling (ndNode* node0, ndNode* node1)	const;
	bool Sanity() const;
	ndConvexHull4dAABBTreeNode* BuildTree (ndConvexHull4dAABBTreeNode* const parent, ndConvexHull4dVector* const points, dInt32 count, dInt32 baseIndex, dInt8** const memoryPool, dInt32& maxMemSize) const;
	
	class ndNormalMap
	{
		public:
		ndNormalMap();
		private:
		void TessellateTriangle(dInt32 level, const ndVector& p0, const ndVector& p1, const ndVector& p2, ndBigVector* const buffer, dInt32& count);

		ndBigVector m_normal[1024];
		dInt32 m_count;
		friend class ndConvexHull4d; 
	};
	static const ndNormalMap& GetNormaMap();

	dInt32 m_mark;
	dInt32 m_count;
	dFloat64 m_diag;
	ndArray<ndConvexHull4dVector> m_points;
};

inline dInt32 ndConvexHull4d::IncMark ()
{
	m_mark ++;
	return m_mark;
}

inline dInt32 ndConvexHull4d::GetVertexCount() const
{
	return m_count;
}

inline dInt32 ndConvexHull4d::GetVertexIndex(dInt32 index) const
{
	dAssert (index >= 0);
	dAssert (index < m_count);
	return m_points[index].m_index;
}


inline const ndBigVector& ndConvexHull4d::GetVertex(dInt32 index) const
{
	dAssert (index >= 0);
	dAssert (index < m_count);
	return m_points[index];
}

inline const ndConvexHull4dVector* ndConvexHull4d::GetHullVertexArray() const
{
	return &m_points[0];
}

inline dFloat64 ndConvexHull4d::GetTetraVolume (const ndConvexHull4dTetraherum* const tetra) const
{
	return tetra->GetTetraVolume (&m_points[0]);
}

#endif