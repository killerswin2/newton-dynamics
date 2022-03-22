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

#ifndef __ND_CUMATRIX3x3_H__
#define __ND_CUMATRIX3x3_H__

#include "cuVector3.h"
#include "cuIntrisics.h"

class cuMatrix3x3
{
	public:
	inline __device__ cuMatrix3x3()
	{
	}

	inline __device__ cuMatrix3x3(const cuMatrix3x3& src)
		:m_front(src.m_front)
		,m_up(src.m_up)
		,m_right(src.m_right)
	{
	}

	inline __device__ cuMatrix3x3(const cuVector3& front, const cuVector3& up, const cuVector3& right)
		:m_front(front)
		,m_up(up)
		,m_right(right)
	{
	}

	inline cuVector3 __device__ UnrotateVector(const cuVector3 &v) const
	{
		return cuVector3((m_front * v).AddHorizontal(), (m_up * v).AddHorizontal(), (m_right * v).AddHorizontal());
	}

	inline cuVector3 __device__ RotateVector(const cuVector3& v) const
	{
		return m_front.Scale(v.m_x)  + m_up.Scale(v.m_y) + m_right.Scale(v.m_z);
	}

	inline cuVector3 __device__ SolveByGaussianElimination(const cuVector3 &v) const
	{
		cuMatrix3x3 tmp(*this);
		cuVector3 ret(v);
		for (int i = 0; i < 3; ++i)
		{
			float pivot = cuAbs(tmp.m_data[i].GetElement(i));
			if (pivot < float(0.01f))
			{
				int permute = i;
				for (int j = i + 1; j < 3; ++j)
				{
					float pivot1 = cuAbs(tmp.m_data[j].GetElement(i));
					if (pivot1 > pivot)
					{
						permute = j;
						pivot = pivot1;
					}
				}

				if (permute != i)
				{
					//dSwap(ret[i], ret[permute]);
					float a0 = ret.GetElement(i);
					ret.SetElement(i, ret.GetElement(permute));
					ret.SetElement(permute, a0);

					cuSwap(tmp.m_data[i], tmp.m_data[permute]);
					//cuVector3 b (tmp.GetElement(i));
					//tmp.SetElement(i, tmp.GetElement(permute));
					//tmp.SetElement(permute, a0);
				}
			}

			for (int j = i + 1; j < 3; ++j)
			{
				const cuVector3 scale(tmp.m_data[j].GetElement(i) / tmp.m_data[i].GetElement(i));
				tmp.m_data[j] = tmp.m_data[j] - tmp.m_data[i] * scale;
				//ret[j] -= ret[i] * scale.GetScalar();
				ret.SetElement(j, ret.GetElement(j) - ret.GetElement(i) * scale.m_x);
				tmp.m_data[j].SetElement(i, 0.0f);
			}
		}

		for (int i = 2; i >= 0; --i)
		{
			const cuVector3 pivot(tmp.m_data[i] * ret);
			//ret[i] = (ret[i] - pivot.AddHorizontal().GetScalar() + tmp[i][i] * ret[i]) / tmp[i][i];
			ret.SetElement(i, (ret.GetElement(i) - pivot.AddHorizontal() + tmp.m_data[i].GetElement(i) * ret.GetElement(i)) / tmp.m_data[i].GetElement(i));
		}

		return ret;
	}

	union
	{
		struct
		{
			cuVector3 m_front;
			cuVector3 m_up;
			cuVector3 m_right;
		};
		cuVector3 m_data[3];
	};
};

#endif