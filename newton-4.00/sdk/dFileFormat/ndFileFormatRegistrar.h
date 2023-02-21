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

#ifndef _ND_FILE_FORMAT_REGISTRY_H__
#define _ND_FILE_FORMAT_REGISTRY_H__

#include "ndFileFormatStdafx.h"
#include "ndTinyXmlGlue.h"

class ndFileFormat;

class ndFileFormatRegistrar : public ndClassAlloc
{
	protected:	
	ndFileFormatRegistrar(const char* const className);
	virtual ~ndFileFormatRegistrar();
	
	public:
	virtual void SaveBody(ndFileFormat* const scene, nd::TiXmlElement* const parentNode, const ndBody* const body);
	virtual void SaveShape(ndFileFormat* const scene, nd::TiXmlElement* const parentNode, const ndShape* const shape);
	virtual void SaveNotify(ndFileFormat* const scene, nd::TiXmlElement* const parentNode, const ndBodyNotify* const notify);
	virtual void SaveCollision(ndFileFormat* const scene, nd::TiXmlElement* const parentNode, const ndShapeInstance* const collision);

	static ndFileFormatRegistrar* GetHandler(const char* const className);

	private:
	static void Init();
	static ndFixSizeArray<ndFileFormatRegistrar*, 256> m_registry;

	ndUnsigned64 m_hash;
	friend class ndFileFormat;
};

#endif 

