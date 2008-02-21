#include "inemo/CNemoSensor.h"


#include "istd/IChangeable.h"
#include "istd/TChangeNotifier.h"


#include "imod/TSingleModelObserverBase.h"
#include "imod/TModelWrap.h"


namespace inemo
{


class TestObjectInterface: public istd::IChangeable
{
public:
	void SetData(int data)
	{
		istd::TChangeNotifier<TestObjectInterface> blockUpdates(this);

		m_data = data;
	}

private:
	int m_data;
};


class TestObserver: public imod::TSingleModelObserverBase<TestObjectInterface>
{
protected:
	void Update(imod::IModel *,int,istd::IPolymorphic *)
	{

	}
};



CNemoSensor::CNemoSensor()
{
	TestObserver observer;
	imod::TModelWrap<TestObjectInterface> model;

	model.AttachObserver(&observer);

	istd::TChangeNotifier<TestObjectInterface> updatePtr(&model);

	updatePtr->SetData(0);

	m_state = StateUnknown;
}


void CNemoSensor::SetSpecification(const inemo::CNemoSensorSpecification& specification)
{
	m_specification = specification;
}


void CNemoSensor::SetInfo(const inemo::CNemoSensorInfo& info)
{
	m_info = info;
}


void CNemoSensor::SetData(const inemo::CNemoSensorData& data)
{
	m_data = data;
}


void CNemoSensor::SetMeasurementRange(const imeas::CMeasurementRange& measurementRange)
{
	m_measurementRange = measurementRange;
}


void CNemoSensor::SetState(int state)
{
	m_state = state;
}


void CNemoSensor::SetPredictedState(int predictedState)
{
	m_predictedState = predictedState;
}


// reimplemented (inemo::INemoSensor)

const INemoSensorSpecification& CNemoSensor::GetSpecification() const
{
	return m_specification;
}


const inemo::INemoSensorInfo& CNemoSensor::GetInfo() const
{
	return m_info;
}


const inemo::INemoSensorData& CNemoSensor::GetData() const
{
	return m_data;
}


const imeas::CMeasurementRange& CNemoSensor::GetMeasurementRange() const
{
	return m_measurementRange;
}


int CNemoSensor::GetState() const
{
	return m_state;
}


int CNemoSensor::GetPredictedState() const
{
	return m_predictedState;
}


} // namespace inemo
