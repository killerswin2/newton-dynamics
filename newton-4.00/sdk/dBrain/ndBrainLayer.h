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

#ifndef _ND_BRAIN_LAYER_H__
#define _ND_BRAIN_LAYER_H__

#include "ndBrainStdafx.h"
#include "ndBrainTypes.h"

class ndBrainLoad;
class ndBrainSave;
class ndBrainVector;
class ndBrainMatrix;

class ndBrainLayer : public ndClassAlloc
{
	public: 
	ndBrainLayer();
	ndBrainLayer(const ndBrainLayer& src);

	virtual ~ndBrainLayer();
	virtual ndBrainLayer* Clone() const;

	virtual bool HasParameters() const;
	virtual const char* GetLabelId() const;

	virtual ndInt32 GetInputSize() const;
	virtual ndInt32 GetOutputSize() const;
	virtual void CopyFrom(const ndBrainLayer& src);

	virtual ndBrainVector* GetBias();
	virtual ndBrainMatrix* GetWeights();
	
	virtual void InitWeightsXavierMethod();
	virtual void InitWeights(ndReal weighVariance, ndReal biasVariance);

	virtual void MakePrediction(const ndBrainVector& input, ndBrainVector& output) const;
	virtual void InputDerivative(const ndBrainVector& output, const ndBrainVector& outputDerivative, ndBrainVector& inputDerivative) const;
	virtual void CalculateParamGradients (
		const ndBrainVector& input, const ndBrainVector& output, const ndBrainVector& outputDerivative,
		ndBrainVector& inputGradient, ndBrainVector& biasGradient, ndBrainMatrix& weightGradient);

	virtual void Blend(const ndBrainLayer& src, ndReal blend);
	virtual void ClearGradAcc(ndBrainVector& gradBiasAcc, ndBrainMatrix& gradWeightAcc) const;
	
	virtual void Load(const ndBrainLoad* const loadSave);
	virtual void Save(const ndBrainSave* const loadSave) const;
};

#endif 

