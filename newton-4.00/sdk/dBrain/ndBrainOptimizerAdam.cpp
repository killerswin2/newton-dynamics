/* Copyright (c) <2003-2022> <Julio Jerez, Newton Game Dynamics>
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

#include "ndBrainStdafx.h"
#include "ndBrain.h"
#include "ndBrainMatrix.h"
#include "ndBrainTrainer.h"
#include "ndBrainOptimizerAdam.h"

class ndBrainOptimizerAdam::ndBrainOptimizerAdam::ndAdamData : public ndClassAlloc
{
	public:
	ndAdamData(ndBrainLayer* const layer)
		:ndClassAlloc()
	{
		if (layer->HasParameters())
		{
			m_biasGradient_u.SetCount(layer->GetOutputSize());
			m_biasGradient_v.SetCount(layer->GetOutputSize());
			m_biasGradient_v2.SetCount(layer->GetOutputSize());
			m_weightGradient_u.Init(layer->GetOutputSize(), layer->GetInputSize());
			m_weightGradient_v.Init(layer->GetOutputSize(), layer->GetInputSize());
			m_weightGradient_v2.Init(layer->GetOutputSize(), layer->GetInputSize());

			m_biasGradient_u.Set(ndReal(0.0f));
			m_biasGradient_v.Set(ndReal(0.0f));
			m_biasGradient_v2.Set(ndReal(0.0f));
			m_weightGradient_u.Set(ndReal(0.0f));
			m_weightGradient_v.Set(ndReal(0.0f));
			m_weightGradient_v2.Set(ndReal(0.0f));
		}
	}

	ndBrainVector m_biasGradient_u;
	ndBrainVector m_biasGradient_v;
	ndBrainVector m_biasGradient_v2;
	ndBrainMatrix m_weightGradient_u;
	ndBrainMatrix m_weightGradient_v;
	ndBrainMatrix m_weightGradient_v2;
};

ndBrainOptimizerAdam::ndBrainOptimizerAdam(ndBrainTrainer* const trainer)
	:ndBrainOptimizer(trainer)
	,m_beta(0.999f)
	,m_alpha(0.9f)
	,m_epsilon(1.0e-8f)
	,m_betaAcc(m_beta)
	,m_alphaAcc(m_alpha)
{
	ndBrain* const brain = trainer->GetBrain();
	for (ndInt32 i = 0; i < brain->GetCount(); ++i)
	{
		m_data.PushBack(new ndAdamData((*brain)[i]));
	}
}

ndBrainOptimizerAdam::~ndBrainOptimizerAdam()
{
	for (ndInt32 i = 0; i < m_data.GetCount(); ++i)
	{
		delete (m_data[i]);
	}
}

void ndBrainOptimizerAdam::Update(ndReal learnRate, ndInt32 bashSize)
{
	learnRate *= ndReal(-1.0f);
	ndReal regularizer = -GetRegularizer();
	ndReal den = ndReal(1.0f) / ndReal(bashSize);

	ndReal betaWeight = ndReal(1.0f) / (ndReal(1.0f) - m_betaAcc);
	ndReal alphaWeight = ndReal(1.0f) / (ndReal(1.0f) - m_alphaAcc);
	ndBrain* const brian = m_trainer->GetBrain();

	for (ndInt32 i = brian->GetCount() - 1; i >= 0 ; --i)
	{
		ndBrainLayer* const layer = (*brian)[i];
		if (layer->HasParameters())
		{
			ndAdamData& data = *m_data[i];
			ndBrainVector& bias = *m_trainer->GetBias(i);
			ndBrainMatrix& weight = *m_trainer->GetWeight(i);
			ndBrainVector& biasGradients = *m_trainer->GetBiasGradients(i);
			ndBrainMatrix& weightGradients = *m_trainer->GetWeightGradients(i);

			biasGradients.Scale(den);
			data.m_biasGradient_v.Scale(m_beta);
			data.m_biasGradient_u.Scale(m_alpha);
			
			data.m_biasGradient_v2.Set(biasGradients);
			data.m_biasGradient_v2.Mul(biasGradients);
			data.m_biasGradient_u.ScaleAdd(biasGradients, ndReal(1.0f) - m_alpha);
			data.m_biasGradient_v.ScaleAdd(data.m_biasGradient_v2, ndReal (1.0f) - m_beta);
			
			if (m_betaAcc > ndReal(0.0f))
			{
				ndBrainVector& vHat = biasGradients;
				ndBrainVector& uHat = data.m_biasGradient_v2;

				uHat.Set(data.m_biasGradient_u);
				vHat.Set(data.m_biasGradient_v);
				vHat.Scale(betaWeight);
				uHat.Scale(alphaWeight);
				for (ndInt32 j = biasGradients.GetCount() - 1; j >= 0; --j)
				{
					ndReal bias_den = ndReal(1.0f) / (ndReal(ndSqrt(vHat[j])) + m_epsilon);
					biasGradients[j] = uHat[j] * bias_den;
				}
			}
			else
			{
				for (ndInt32 j = biasGradients.GetCount() - 1; j >= 0; --j)
				{
					ndReal bias_den = ndReal(1.0f) / (ndReal(ndSqrt(data.m_biasGradient_v[j])) + m_epsilon);
					biasGradients[j] = data.m_biasGradient_u[j] * bias_den;
				}
			}
			biasGradients.Scale(learnRate);
			biasGradients.ScaleAdd(bias, regularizer);
			bias.Add(biasGradients);
			bias.FlushToZero();


			weightGradients.Scale(den);
			data.m_weightGradient_v.Scale(m_beta);
			data.m_weightGradient_u.Scale(m_alpha);
			data.m_weightGradient_v2.Set(weightGradients);
			data.m_weightGradient_v2.Mul(weightGradients);
			data.m_weightGradient_v.ScaleAdd(data.m_weightGradient_v2, ndReal(1.0f) - m_beta);
			data.m_weightGradient_u.ScaleAdd(weightGradients, ndReal(1.0f) - m_alpha);

			if (m_betaAcc > ndReal(0.0f))
			{
				ndBrainMatrix& vHat = weightGradients;
				ndBrainMatrix& uHat = data.m_weightGradient_v2;
				
				uHat.Set(data.m_weightGradient_u);
				vHat.Set(data.m_weightGradient_v);
				vHat.Scale(betaWeight);
				uHat.Scale(alphaWeight);

				for (ndInt32 j = weightGradients.GetRows() - 1; j >= 0; --j)
				{
					for (ndInt32 k = weightGradients[j].GetCount() - 1; k >= 0; --k)
					{
						ndReal bias_den = ndReal(1.0f) / (ndReal(ndSqrt(vHat[j][k])) + m_epsilon);
						weightGradients[j][k] = uHat[j][k] * bias_den;
					}
				}
			}
			else
			{
				for (ndInt32 j = weightGradients.GetRows() - 1; j >= 0; --j)
				{
					for (ndInt32 k = weightGradients[j].GetCount() - 1; k >= 0; --k)
					{
						ndReal bias_den = ndReal(1.0f) / (ndReal(ndSqrt(data.m_weightGradient_v[j][k])) + m_epsilon);
						weightGradients[j][k] = data.m_weightGradient_u[j][k] * bias_den;
					}
				}
			}
			weightGradients.Scale(learnRate);
			weightGradients.ScaleAdd(weight, regularizer);
			weight.Add(weightGradients);
			weight.FlushToZero();
		}
	}

	m_betaAcc = ndFlushToZero(m_betaAcc * m_beta);
	m_alphaAcc = ndFlushToZero(m_alphaAcc * m_alpha);
	if (m_betaAcc < ndReal(1.0e-6f))
	{
		m_betaAcc = ndReal(0.0f);
	}
}