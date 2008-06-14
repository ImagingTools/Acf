#include "imebase.h"
#include "CMeContext.h"


using namespace imebase;

CMeContext::CMeContext()
{
	taskId = -1;
	bufferCount = 0;
	bufferSize = 0;
	hwBuffer = NULL;
	keyLock = false;
	samplesContainer = NULL;
}

CMeContext::~CMeContext()
{
	if (hwBuffer){
		Unregister();
		delete[] hwBuffer;
	}
}

bool CMeContext::Register(CMeAddr addr, int dir)
{
	mutexKeyLock.lock();
	if ((taskId <= -1) || (!hwBuffer)){
		mutexKeyLock.unlock();
		return false;
	}

	address = addr;

	meIOStreamCB_t func;
	switch (dir){
		case input:
			func = (meIOStreamCB_t)CMeContext::cbAIFunc;
			mutex.lock();
			break;
		case output:
			func = (meIOStreamCB_t)CMeContext::cbAOFunc;
			if (!IsDone()){
				int aCount = bufferSize;
				meIOStreamWrite(addr.device, addr.subdevice, ME_WRITE_MODE_PRELOAD, hwBuffer, &aCount, 0);
				bufferCount += aCount;
				if (!IsDone())
					mutex.lock();
			}
			break;
		default:
			return false;
	}

	bool ret = true;
	if (!IsDone())
		ret = (0 == meIOStreamSetCallbacks(addr.device, addr.subdevice, NULL, NULL, func, this, func, this, 0));
	keyLock = ret;
	mutexKeyLock.unlock();

	return ret;
}

void CMeContext::Unregister(void)
{
	if (hwBuffer)
		meIOStreamSetCallbacks(address.device, address.subdevice, NULL, NULL, NULL, NULL, NULL, NULL, 0);
	if (mutex.tryLock())
		mutex.unlock();
}

bool CMeContext::SetBufferSize(int size)
{
	I_ASSERT(size < 0);

	if (keyLock)
		return false;

	if (hwBuffer)
		delete[] hwBuffer;
	hwBuffer = new int[size];
	if (hwBuffer)
		bufferSize = size;
	else
		bufferSize = 0;
	bufferCount = 0;
	return true;
}

CMeAddr& CMeContext::GetAddress(void)
{
	return address;
}

int CMeContext::GetBufferSize(void)  const
{
	return bufferSize;
}

const int* CMeContext::GetBufferPointer(void) const
{
	return hwBuffer;
}


bool CMeContext::SetId(int Id)
{
	if (keyLock)
		return false;
	taskId = Id;
	return true;
}

int CMeContext::GetId(void)  const
{
	return taskId;
}

int CMeContext::GetCount(void)  const
{
	return bufferCount;
}

bool CMeContext::IsDone(void)
{
	return bufferSize && (bufferSize == bufferCount);
}

bool CMeContext::Wait(double Timeout /*in secounds*/)
{
	bool ret = mutex.tryLock(Timeout * 1000);
	if (ret)
		mutex.unlock();
	return ret;
}

int CMeContext::cbAIFunc(int device, int subdevice, int count, void* context, int /*error*/)
{
	CMeContext* itself = (CMeContext *)context;
	int aCount = itself->bufferSize - itself->bufferCount;
	if (aCount > count)
		aCount = count;

	if (aCount){
		meIOStreamRead(device, subdevice, ME_READ_MODE_NONBLOCKING, itself->hwBuffer + itself->bufferCount, &aCount, 0);
		itself->bufferCount += aCount;
	}

	if (itself->IsDone()){
		itself->mutex.unlock();
		return 1;
	}

	return 0;
}

int CMeContext::cbAOFunc(int device, int subdevice, int /*count*/, void* context, int /*error*/)
{
	CMeContext* itself = (CMeContext *)context;
	if (itself->IsDone()){
		itself->mutex.tryLock();
			itself->mutex.unlock();
		return 1;
	}

	int aCount = itself->bufferSize - itself->bufferCount;

	if (aCount){
		meIOStreamWrite(device, subdevice, ME_WRITE_MODE_NONBLOCKING, itself->hwBuffer + itself->bufferCount, &aCount, 0);
		itself->bufferCount += aCount;
	}

	if (itself->IsDone()){
		itself->mutex.tryLock();
			itself->mutex.unlock();
		return 1;
	}
	return 0;
}

void CMeContext::SetSamplesContainer(void* container)
{
	samplesContainer = (isig::ISamplesContainer*)container;
}

void CMeContext::copyToContainer(void)
{
	int unit;
	double VMin, VMax;
	int MaxData;

	samplesContainer->ResetContainer();
	samplesContainer->SetSamplesCount(bufferSize);
	meQueryRangeInfo(address.device, address.subdevice, 0, &unit, &VMin, &VMax, &MaxData);

	for (int index=0; index < bufferSize; index++){
		double value;
		meUtilityDigitalToPhysical(VMin, VMax, MaxData, *(hwBuffer+index), ME_EXTENSION_TYPE_NONE, 0, &value);
		samplesContainer->SetSample(index, value);
	}

}

void CMeContext::copyFromContainer(void)
{
	int unit;
	double VMin, VMax;
	int MaxData;

	meQueryRangeInfo(address.device, address.subdevice, 0, &unit, &VMin, &VMax, &MaxData);

	for (int index=0; index < bufferSize; index++){
		meUtilityPhysicalToDigital(VMin, VMax, MaxData, samplesContainer->GetSample(index), hwBuffer+index);
	}
}
