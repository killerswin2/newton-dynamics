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

#ifndef __ND_CUDA_SCENE_INFO_H__
#define __ND_CUDA_SCENE_INFO_H__

#include <cuda.h>
#include <vector_types.h>
#include <cuda_runtime.h>
#include "ndCudaVector.h"
#include "ndCudaDeviceBuffer.h"

class ndCudaBodyProxy;
class ndCudaBodyAabbCell;

class ndCudaSpatialVector
{
	public:
	ndCudaVector m_linear;
	ndCudaVector m_angular;
};

class ndCudaBoundingBox
{
	public:
	ndCudaVector m_min;
	ndCudaVector m_max;
};

#define D_AABB_GRID_CELL_BITS			10

class ndCudaBodyAabbCell
{
	public:
	union
	{
		struct
		{
			union
			{
				struct
				{
					unsigned m_x : D_AABB_GRID_CELL_BITS;
					unsigned m_y : D_AABB_GRID_CELL_BITS;
					unsigned m_z : D_AABB_GRID_CELL_BITS;
				};
				unsigned m_key;
			};
			unsigned m_id;
		};
		long long m_value;
	};
};


template <class T>
class ndCudaBuffer
{
	public:
	ndCudaBuffer()
		:m_array(nullptr)
		,m_size(0)
		,m_capacity(0)
	{
	}

	ndCudaBuffer(const ndCudaDeviceBuffer<T>& src)
		:m_array((T*)&src[0])
		,m_size(src.GetCount())
		,m_capacity(src.GetCapacity())
	{
	}

	T* m_array;
	unsigned m_size;
	unsigned m_capacity;
};

class ndCudaSceneInfo
{
	public:
	ndCudaSceneInfo()
		:m_worldBox()
		,m_histogram()
		,m_bodyArray()
		,m_bodyAabbArray()
		,m_bodyAabbCell()
		,m_bodyAabbCellScrath()
		,m_transformBuffer0()
		,m_transformBuffer1()
		,m_timeSlice(0)
		,m_frameIsValid(0)
		,m_frameCount(0)
	{
	}

	ndCudaBoundingBox m_worldBox;
	ndCudaBuffer<unsigned> m_histogram;
	ndCudaBuffer<ndCudaBodyProxy> m_bodyArray;
	ndCudaBuffer<ndCudaBoundingBox> m_bodyAabbArray;
	ndCudaBuffer<ndCudaBodyAabbCell> m_bodyAabbCell;
	ndCudaBuffer<ndCudaBodyAabbCell> m_bodyAabbCellScrath;
	ndCudaBuffer<ndCudaSpatialVector> m_transformBuffer0;
	ndCudaBuffer<ndCudaSpatialVector> m_transformBuffer1;
	
	long long m_timeSlice;
	unsigned m_frameIsValid;
	unsigned m_frameCount;
};

#endif