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
#include "ndBrainSaveLoad.h"
#include "ndBrainLayerConvolutional_2d.h"

#define ND_BRAIN_CONV_LAYER_USE_BIAS

ndBrainLayerConvolutional_2d::ndBrainLayerConvolutional_2d(ndInt32 inputWidth, ndInt32 inputHeight, ndInt32 inputDepth, ndInt32 kernelSize, ndInt32 numberOfKernels)
	:ndBrainLayer()
	,m_bias()
	,m_kernels()
	,m_inputOffsets()
	,m_inputGradOffsets()
	,m_kernelSize(kernelSize)
	,m_inputWidth(inputWidth)
	,m_inputHeight(inputHeight)
	,m_inputLayers(inputDepth)
	,m_outputWidth(m_inputWidth - m_kernelSize + 1)
	,m_outputHeight(m_inputHeight - m_kernelSize + 1)
	,m_outputLayers(numberOfKernels)
{
	ndAssert(m_inputWidth > 0);
	ndAssert(m_inputHeight > 0);
	ndAssert(m_outputWidth > 0);
	ndAssert(m_outputHeight > 0);

	m_bias.SetCount(m_outputLayers);
	m_kernels.SetCount(m_outputLayers * m_inputLayers * m_kernelSize * m_kernelSize);

	m_bias.Set(ndBrainFloat(0.0f));
	m_kernels.Set(ndBrainFloat(0.0f));

	ndInt32 inputOffset = 0;
	ndInt32 inputGradOffset = 0;
	for (ndInt32 j = 0; j < m_kernelSize; ++j)
	{
		for (ndInt32 i = 0; i < m_kernelSize; ++i)
		{
			m_inputOffsets.PushBack(inputOffset + i);
			m_inputGradOffsets.PushBack(inputGradOffset + i);
		}
		inputOffset += m_inputWidth;
		inputGradOffset += m_inputWidth + m_kernelSize - 1;
	}
}

ndBrainLayerConvolutional_2d::ndBrainLayerConvolutional_2d(const ndBrainLayerConvolutional_2d& src)
	:ndBrainLayer(src)
	,m_bias(src.m_bias)
	,m_kernels(src.m_kernels)
	,m_inputOffsets(src.m_inputOffsets)
	,m_inputGradOffsets(src.m_inputGradOffsets)
	,m_kernelSize(src.m_kernelSize)
	,m_inputWidth(src.m_inputWidth)
	,m_inputHeight(src.m_inputHeight)
	,m_inputLayers(src.m_inputLayers)
	,m_outputWidth(src.m_outputWidth)
	,m_outputHeight(src.m_outputHeight)
	,m_outputLayers(src.m_outputLayers)
{
}

ndBrainLayerConvolutional_2d::~ndBrainLayerConvolutional_2d()
{
}

const char* ndBrainLayerConvolutional_2d::GetLabelId() const
{
	return "ndBrainLayerConvolutional_2d";
}

ndBrainLayer* ndBrainLayerConvolutional_2d::Clone() const
{
	return new ndBrainLayerConvolutional_2d(*this);
}

ndInt32 ndBrainLayerConvolutional_2d::GetOutputSize() const
{
	return m_outputLayers * m_outputWidth * m_outputHeight;
}

ndInt32 ndBrainLayerConvolutional_2d::GetOutputBufferSize() const
{
	ndInt32 size = GetOutputSize();
	ndInt32 paddSizeWidth = m_inputWidth + m_kernelSize - 1;
	ndInt32 paddSizeHeight = m_inputHeight + m_kernelSize - 1;
	return size + paddSizeWidth * paddSizeHeight;
}

ndInt32 ndBrainLayerConvolutional_2d::GetInputSize() const
{
	return m_inputLayers * m_inputWidth * m_inputHeight;
}

ndInt32 ndBrainLayerConvolutional_2d::GetOutputWidth() const
{
	return m_outputWidth;
}

ndInt32 ndBrainLayerConvolutional_2d::GetOutputHeight() const
{
	return m_outputHeight;
}

ndInt32 ndBrainLayerConvolutional_2d::GetOutputChannels() const
{
	return m_outputLayers;
}

ndInt32 ndBrainLayerConvolutional_2d::GetNumberOfParameters() const
{
	return m_bias.GetCount() + m_kernelSize * m_kernelSize * m_inputLayers * m_outputLayers;
}

bool ndBrainLayerConvolutional_2d::HasParameters() const
{
	return true;
}

void ndBrainLayerConvolutional_2d::InitWeightsXavierMethod()
{
	ndBrainFloat weighVariance = ndBrainFloat(ndSqrt(ndFloat32(6.0f) / ndFloat32(GetInputSize() + GetOutputSize())));
	InitWeights(weighVariance, ndBrainFloat(0.0f));
}

void ndBrainLayerConvolutional_2d::InitGaussianBias(ndBrainFloat)
{
	m_bias.Set(ndBrainFloat(0.0f));
}

void ndBrainLayerConvolutional_2d::InitGaussianWeights(ndBrainFloat variance)
{
	m_kernels.InitGaussianWeights(variance);
}

void ndBrainLayerConvolutional_2d::InitWeights(ndBrainFloat weighVariance, ndBrainFloat biasVariance)
{
	biasVariance = ndBrainFloat(0.0f);
	weighVariance = ndMin(weighVariance, ndBrainFloat(0.05f));
	InitGaussianBias(biasVariance);
	InitGaussianWeights(weighVariance);
}

void ndBrainLayerConvolutional_2d::Set(const ndBrainLayer& src)
{
	const ndBrainLayerConvolutional_2d& convSrc = (ndBrainLayerConvolutional_2d&)src;
	m_bias.Set(convSrc.m_bias);
	m_kernels.Set(convSrc.m_kernels);
}

void ndBrainLayerConvolutional_2d::Clear()
{
	m_bias.Set(ndBrainFloat(0.0f));
	m_kernels.Set(ndBrainFloat(0.0f));
}

void ndBrainLayerConvolutional_2d::FlushToZero()
{
	m_bias.FlushToZero();
	m_kernels.FlushToZero();
}

void ndBrainLayerConvolutional_2d::Scale(ndBrainFloat scale)
{
	m_bias.Scale(scale);
	m_kernels.Scale(scale);
}

void ndBrainLayerConvolutional_2d::Add(const ndBrainLayer& src)
{
	const ndBrainLayerConvolutional_2d& linearSrc = (ndBrainLayerConvolutional_2d&)src;
	m_bias.Add(linearSrc.m_bias);
	m_kernels.Add(linearSrc.m_kernels);
}

void ndBrainLayerConvolutional_2d::Mul(const ndBrainLayer& src)
{
	const ndBrainLayerConvolutional_2d& linearSrc = (ndBrainLayerConvolutional_2d&)src;
	m_bias.Mul(linearSrc.m_bias);
	m_kernels.Mul(linearSrc.m_kernels);
}

void ndBrainLayerConvolutional_2d::ScaleAdd(const ndBrainLayer& src, ndBrainFloat scale)
{
	const ndBrainLayerConvolutional_2d& linearSrc = (ndBrainLayerConvolutional_2d&)src;
	m_bias.ScaleAdd(linearSrc.m_bias, scale);
	m_kernels.ScaleAdd(linearSrc.m_kernels, scale);
}

void ndBrainLayerConvolutional_2d::Blend(const ndBrainLayer& src, ndBrainFloat blend)
{
	const ndBrainLayerConvolutional_2d& linearSrc = (ndBrainLayerConvolutional_2d&)src;
	m_bias.Blend(linearSrc.m_bias, blend);
	m_kernels.Blend(linearSrc.m_kernels, blend);
}

void ndBrainLayerConvolutional_2d::AdamUpdate(const ndBrainLayer& u, const ndBrainLayer& v, ndBrainFloat epsilon)
{
	const ndBrainLayerConvolutional_2d& linear_U = (ndBrainLayerConvolutional_2d&)u;
	const ndBrainLayerConvolutional_2d& linear_V = (ndBrainLayerConvolutional_2d&)v;

	const ndBrainVector& bias_U = linear_U.m_bias;
	const ndBrainVector& bias_V = linear_V.m_bias;
	for (ndInt32 i = m_bias.GetCount() - 1; i >= 0; --i)
	{
		ndBrainFloat bias_den = ndBrainFloat(1.0f) / (ndBrainFloat(ndSqrt(bias_V[i])) + epsilon);
		m_bias[i] = bias_U[i] * bias_den;
	}

	const ndBrainVector& kernels_U = linear_U.m_kernels;
	const ndBrainVector& kernels_V = linear_V.m_kernels;
	for (ndInt32 j = m_kernels.GetCount() - 1; j >= 0; --j)
	{
		ndBrainFloat weight_den = ndBrainFloat(1.0f) / (ndBrainFloat(ndSqrt(kernels_V[j])) + epsilon);
		m_kernels[j] = kernels_U[j] * weight_den;
	}
}

void ndBrainLayerConvolutional_2d::UpdateDropOut()
{
}

void ndBrainLayerConvolutional_2d::Save(const ndBrainSave* const loadSave) const
{
	char buffer[1024];
	auto Save = [this, &buffer, &loadSave](const char* const fmt, ...)
		{
			va_list v_args;
			buffer[0] = 0;
			va_start(v_args, fmt);
			vsprintf(buffer, fmt, v_args);
			va_end(v_args);
			loadSave->WriteData(buffer);
		};

	Save("\tinput_width %d\n", m_inputWidth);
	Save("\tinput_heigh %d\n", m_inputHeight);
	Save("\tinput_layers %d\n", m_inputLayers);
	Save("\tkernel_Size %d\n", m_kernelSize);
	Save("\touput_layers %d\n", m_outputLayers);

	Save("\tbias ");
	for (ndInt32 i = 0; i < m_bias.GetCount(); ++i)
	{
		Save("%g ", m_bias[i]);
	}
	Save("\n");

	Save("\tfilters\n");
	//for (ndInt32 i = 0; i < m_weights.GetCount(); ++i)

	const ndInt32 kernelSize = m_kernelSize * m_kernelSize;

	ndInt32 kernelOffset = 0;
	for (ndInt32 i = 0; i < m_outputLayers * m_inputLayers; ++i)
	{
		Save("\t\tfilter_%d ", i);
		//const ndBrainVector& row = m_weights[i];
		const ndBrainMemVector kernels(&m_kernels[kernelOffset], kernelSize);
		for (ndInt32 j = 0; j < kernelSize; ++j)
		{
			Save("%g ", kernels[j]);
		}
		kernelOffset += kernelSize;
		Save("\n");
	}
}

ndBrainLayer* ndBrainLayerConvolutional_2d::Load(const ndBrainLoad* const loadSave)
{
	char buffer[1024];
	loadSave->ReadString(buffer);

	loadSave->ReadString(buffer);
	ndInt32 inputWidth = loadSave->ReadInt();

	loadSave->ReadString(buffer);
	ndInt32 inputHeight = loadSave->ReadInt();

	loadSave->ReadString(buffer);
	ndInt32 inputLayers = loadSave->ReadInt();

	loadSave->ReadString(buffer);
	ndInt32 kernelSize = loadSave->ReadInt();

	loadSave->ReadString(buffer);
	ndInt32 ouputLayers = loadSave->ReadInt();

	ndBrainLayerConvolutional_2d* const layer = new ndBrainLayerConvolutional_2d(inputWidth, inputHeight, inputLayers, kernelSize, ouputLayers);

	loadSave->ReadString(buffer);
	for (ndInt32 i = 0; i < ouputLayers; ++i)
	{
		ndBrainFloat val = ndBrainFloat(loadSave->ReadFloat());
		layer->m_bias[i] = val;
	}

	loadSave->ReadString(buffer);
	ndInt32 kernelWeights = kernelSize * kernelSize;

	ndInt32 index = 0;
	for (ndInt32 i = 0; i < ouputLayers * inputLayers; ++i)
	{
		loadSave->ReadString(buffer);
		for (ndInt32 j = 0; j < kernelWeights; ++j)
		{
			ndBrainFloat val = ndBrainFloat(loadSave->ReadFloat());
			layer->m_kernels[index] = val;
			index++;
		}
	}

	loadSave->ReadString(buffer);
	return layer;
}

void ndBrainLayerConvolutional_2d::MakePrediction(const ndBrainVector& input, ndBrainVector& output) const
{
	ndAssert(input.GetCount() == GetInputSize());

	ndInt32 outputOffset = 0;
	ndInt32 kernelOffset = 0;
	const ndInt32 inputSize = m_inputWidth * m_inputHeight;
	const ndInt32 kernelSize = m_kernelSize * m_kernelSize;
	const ndInt32 outputSize = m_outputWidth * m_outputHeight;

	ndBrainFloat convKernelBuffer[512];
	ndBrainMemVector convKernel(convKernelBuffer, kernelSize);

	auto CrossCorrelation = [this, &convKernel](const ndBrainVector& input)
	{
		ndBrainFloat value = ndBrainFloat(0.0f);
		for (ndInt32 i = m_inputOffsets.GetCount() - 1; i >= 0; --i)
		{
			ndInt32 index = m_inputOffsets[i];
			ndAssert(convKernel[i] < ndBrainFloat(100.0f));
			ndAssert(convKernel[i] > ndBrainFloat(-100.0f));
			value += input[index] * convKernel[i];
		}
		return value;
	};

	#ifdef ND_BRAIN_CONV_LAYER_USE_BIAS
		ndBrainFloat biasScale = ndBrainFloat(1.0f) / ndBrainFloat(m_inputLayers * outputSize);
	#else
		ndBrainFloat biasScale = ndBrainFloat(0.0f);
	#endif

	auto RotateKernel = [&convKernel](const ndBrainMemVector& kernel)
	{
		ndAssert(convKernel.GetCount() == kernel.GetCount());
		for (ndInt32 i = kernel.GetCount() - 1; i >= 0; --i)
		{
			convKernel[kernel.GetCount() - 1 - i] = kernel[i];
		}
	};

	for (ndInt32 i = 0; i < m_outputLayers; ++i)
	{
		ndBrainMemVector out(&output[outputOffset], outputSize);

		ndInt32 inputOffset = 0;
		out.Set(m_bias[i] * biasScale);
		for (ndInt32 channel = 0; channel < m_inputLayers; ++channel)
		{
			ndInt32 outIndex = 0;
			ndInt32 inputBase = inputOffset;

			const ndBrainMemVector filter(&m_kernels[kernelOffset], kernelSize);
			RotateKernel(filter);

			for (ndInt32 y = 0; y < m_outputHeight; ++y)
			{
				for (ndInt32 x = 0; x < m_outputWidth; ++x)
				{
					const ndBrainMemVector in(&input[inputBase + x], inputSize);
					out[outIndex] += CrossCorrelation(in);
					outIndex++;
				}
				inputBase += m_inputWidth;
			}
			inputOffset += inputSize;
			kernelOffset += kernelSize;
		}
		outputOffset += outputSize;
	}
}

//void ndBrainLayerConvolutional_2d::InputDerivative(const ndBrainVector& output, const ndBrainVector& outputDerivative, ndBrainVector& inputDerivative) const
void ndBrainLayerConvolutional_2d::InputDerivative(const ndBrainVector&, const ndBrainVector&, ndBrainVector&) const
{
	ndAssert(0);
}

void ndBrainLayerConvolutional_2d::CalculateParamGradients(
	const ndBrainVector& input, const ndBrainVector& output,
	const ndBrainVector& outputDerivative, ndBrainVector& inputGradient, ndBrainLayer* const gradientOut) const
{
	ndAssert(!strcmp(GetLabelId(), gradientOut->GetLabelId()));
	ndBrainLayerConvolutional_2d* const gradients = (ndBrainLayerConvolutional_2d*)gradientOut;

	ndAssert(gradients->m_bias.GetCount() == m_outputLayers);

	const ndInt32 inputSize = m_inputWidth * m_inputHeight;
	const ndInt32 kernelSize = m_kernelSize * m_kernelSize;
	const ndInt32 outputSize = m_outputWidth * m_outputHeight;

	#ifdef ND_BRAIN_CONV_LAYER_USE_BIAS
		ndBrainFloat biasScale = ndBrainFloat(1.0f) / ndBrainFloat(m_inputLayers * outputSize);
	#else
		ndBrainFloat biasScale = ndBrainFloat(0.0f);
	#endif

	// calculate bias gradients
	for (ndInt32 i = 0; i < m_bias.GetCount(); ++i)
	{
		ndBrainFloat value = ndBrainFloat(0.0f);
		const ndBrainMemVector biasGrad(&outputDerivative[i * outputSize], outputSize);
		for (ndInt32 j = 0; j < outputSize; ++j)
		{
			value += biasGrad[j];
		}
		gradients->m_bias[i] = value * biasScale;
	}
	
	// calculate filter weight gradients
	auto CrossCorrelationGradient = [this](const ndBrainVector& input, ndInt32 y0, ndInt32 x0, const ndBrainVector& outputDerivative)
	{
		ndBrainFloat value = ndBrainFloat(0.0f);

		ndInt32 inputDerivativeBase = 0;
		ndInt32 inputBase = y0 * m_inputWidth + x0;
		for (ndInt32 y = 0; y < m_outputHeight; ++y)
		{
			const ndBrainMemVector in(&input[inputBase], m_outputWidth);
			const ndBrainMemVector outputGrad(&outputDerivative[inputDerivativeBase], m_outputWidth);
			for (ndInt32 x = 0; x < m_outputWidth; ++x)
			{
				value += in[x] * outputGrad[x];
			}
			inputBase += m_inputWidth;
			inputDerivativeBase += m_outputWidth;
		}
		return value;
	};

	ndInt32 outputOffset = 0;
	ndInt32 kernelOffset = 0;

	for (ndInt32 filter = 0; filter < m_outputLayers; ++filter)
	{
		ndInt32 inputOffset = 0;
		const ndBrainMemVector outDerivative(&outputDerivative[outputOffset], outputSize);
		for (ndInt32 channel = 0; channel < m_inputLayers; ++channel)
		{
			const ndBrainMemVector inputChannel(&input[inputOffset], inputSize);
			ndBrainMemVector kernelGradients(&gradients->m_kernels[kernelOffset], kernelSize);

			ndInt32 index = kernelSize - 1;
			for (ndInt32 y = 0; y < m_kernelSize; ++y)
			{
				for (ndInt32 x = 0; x < m_kernelSize; ++x)
				{
					kernelGradients[index] = CrossCorrelationGradient(inputChannel, y, x, outDerivative);
					index--;
				}
			}
			inputOffset += inputSize;
			kernelOffset += kernelSize;
		}
		outputOffset += outputSize;
	}

	// calculate input gradients
	const ndInt32 paddSizeWidth = m_inputWidth + m_kernelSize - 1;
	const ndInt32 paddSizeHeight = m_inputHeight + m_kernelSize - 1;
	ndBrainMemVector paddedGradientOut(&output[0] + GetOutputSize(), paddSizeWidth * paddSizeHeight);
	paddedGradientOut.Set(ndBrainFloat(0.0f));

	auto CopyOutput = [this, &outputDerivative, &paddedGradientOut](ndInt32 filter)
	{
		ndInt32 srcOffset = 0;
		ndInt32 dstWidth = m_inputWidth + m_kernelSize - 1;
		ndInt32 outputSize = m_outputWidth * m_outputHeight;
		ndInt32 dstOffset = dstWidth * (m_kernelSize - 1) + m_kernelSize - 1;
		const ndBrainMemVector srcGrad(&outputDerivative[filter * outputSize], outputSize);
		for (ndInt32 y = 0; y < m_outputHeight; ++y)
		{
			const ndBrainMemVector src(&srcGrad[srcOffset], m_outputWidth);
			//ndBrainMemVector dst(&m_paddedGradientOut[dstOffset], m_outputWidth);
			ndBrainMemVector dst(&paddedGradientOut[dstOffset], m_outputWidth);
			dst.Set(src);
			dstOffset += dstWidth;
			srcOffset += m_outputWidth;
		}
	};

	auto CalculateInpuGradient = [this, &paddedGradientOut](const ndBrainVector& filter, ndBrainVector& output)
	{
		auto CrossCorrelation = [this, &filter](const ndBrainVector& input)
		{
			ndBrainFloat value = ndBrainFloat(0.0f);
			for (ndInt32 i = m_inputGradOffsets.GetCount() - 1; i >= 0 ; --i)
			{
				ndInt32 index = m_inputGradOffsets[i];
				value += input[index] * filter[i];
			}
			return value;
		};
		
		ndInt32 outputOffset = 0;
		ndInt32 gradInputOffset = 0;
		const ndInt32 gradInputWidth = m_inputWidth + m_kernelSize - 1;
		const ndInt32 gradInputSize = gradInputWidth * m_kernelSize;

		for (ndInt32 y = 0; y < m_inputHeight; ++y)
		{
			for (ndInt32 x = 0; x < m_inputWidth; ++x)
			{
				//const ndBrainMemVector in(&m_paddedGradientOut[gradInputOffset + x], gradInputSize);
				const ndBrainMemVector in(&paddedGradientOut[gradInputOffset + x], gradInputSize);
				output[outputOffset + x] += CrossCorrelation(in);
			}
			outputOffset += m_inputWidth;
			gradInputOffset += gradInputWidth;
		}
	};

	kernelOffset = 0;
	ndInt32 inputOffset = 0;
	inputGradient.Set(ndBrainFloat(0.0f));
	for (ndInt32 filter = 0; filter < m_outputLayers; ++filter)
	{
		inputOffset = 0;
		CopyOutput(filter);
		for (ndInt32 channel = 0; channel < m_inputLayers; ++channel)
		{
			ndBrainMemVector inputGrad(&inputGradient[inputOffset], inputSize);
			const ndBrainMemVector kernelGradients(&m_kernels[kernelOffset], kernelSize);
			CalculateInpuGradient(kernelGradients, inputGrad);
			inputOffset += inputSize;
			kernelOffset += kernelSize;
		}
	}
}