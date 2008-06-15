#include "CMeBase.h"


//#include "imebase.h"

#include "medriver.h"
#include "medefines.h"
#include "metypes.h"
#include "meerror.h"


namespace imebase
{


CMeBase::CMeBase()
{

	if (meOpen(0))
		return;

	int devicesCount;
	if (meQueryNumberDevices(&devicesCount))
		return;

	for (int deviceIndex=0; deviceIndex < devicesCount; deviceIndex++){
		CMeAddr addressDevice;
		addressDevice.device = deviceIndex;
		while (true){
			int index = 0;
			int addressSubdevice;
			if (meQuerySubdeviceByType(deviceIndex, index, ME_TYPE_AO, ME_SUBTYPE_STREAMING, &addressSubdevice))
				break;
			addressDevice.subdevice = index;
			meQueryNumberRanges(deviceIndex, index, ME_UNIT_ANY, &addressDevice.channel);
			MeOutputList.append(addressDevice);
		}

		while (true){
			int index = 0;
			int addressSubdevice;
			if (meQuerySubdeviceByType(deviceIndex, index, ME_TYPE_AI, ME_SUBTYPE_STREAMING, &addressSubdevice))
				break;
			addressDevice.subdevice = index;
			meQueryNumberRanges(deviceIndex, index, ME_UNIT_ANY, &addressDevice.channel);
			MeInputList.append(addressDevice);
		}
	}
}


CMeBase::~CMeBase()
{
	meClose(0);
}


imebase::CMeList& CMeBase::GetMeList(int direction)
{
	switch ((EMeDirection)direction){
		case input : 
			return MeInputList;
			break;

		case output:
			return MeOutputList;
			break;
	}
	return MeEmptyList;
}


imebase::CMeList& CMeBase::GetMeInputList()
{
	return MeInputList;
}


imebase::CMeList& CMeBase::GetMeOutputList()
{
	return MeOutputList;
}


bool CMeBase::CheckAddress(CMeAddr addres, int direction) const
{
	const CMeList* list;

	switch ((EMeDirection)direction){
		case input: 
			list = &MeInputList;
			break;

		case output:
			list = &MeOutputList;
			break;

		default:
			return false;
	}

	for (int index = 0; index < list->size(); index++){
		const CMeAddr& entry = list->at(index);
		if (		(entry.device == addres.device) &&
					(entry.subdevice == addres.subdevice) &&
					(entry.channel > addres.channel)){
			return true;
		}
	} 

	return false;
}


} // namespace imebase


