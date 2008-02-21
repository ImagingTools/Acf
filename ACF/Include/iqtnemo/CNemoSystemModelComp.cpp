#include "iqtnemo/CNemoSystemModelComp.h"


#include <QSqlTableModel>


#include "QtAcf/QtAcf.h"


namespace iqtnemo
{


CNemoSystemModelComp::CNemoSystemModelComp()
	:BaseClass()
{
	registerInterface<inemo::INemoSystemModel>(this);
}


// reimplemented (acf::INemoSystemModel)

int CNemoSystemModelComp::GetSensorCount() const
{
	return int(m_sensors.size());
}


inemo::INemoSensor& CNemoSystemModelComp::GetSensor(int sensorIndex) const
{
	static acf::ModelTemplate<inemo::CNemoSensor> sensor;

	if (sensorIndex < GetSensorCount() && sensorIndex >= 0){
		return *m_sensors.at(sensorIndex);
	}

	return sensor; 
}

// reimplemented (acf::ModelInterface)

void CNemoSystemModelComp::notifyUpdate(int updateFlags, acf::PolymorphicInterface* objectPtr)
{
	BaseClass2::notifyUpdate(updateFlags, objectPtr); 
}


// protected methods

// reimplemented (iqtdb::CTableModelCompBase)
	
bool CNemoSystemModelComp::IsModelChanged() const
{
	I_ASSERT(m_tableModelPtr != NULL);
	
	if (m_sensors.size() != m_tableModelPtr->rowCount()){
		return true;
	}
	else{
	}
	
	return false;
}


void CNemoSystemModelComp::SynchronizeModelWithTable()
{
	if (m_tableModelPtr != NULL){
		m_sensors.reset();

		int rowCount = m_tableModelPtr->rowCount();
		for (int rowIndex = 0; rowIndex < rowCount; rowIndex++){
			QSqlRecord sensorRecord = m_tableModelPtr->record(rowIndex);

			inemo::CNemoSensor* sensor = new acf::ModelTemplate<inemo::CNemoSensor>;
			sensor->setName(acf::acfString(sensorRecord.value("name").toString()));

			double bottomError = sensorRecord.value("lowred").toDouble();
			double topError = sensorRecord.value("highred").toDouble();
			double bottomWarning = sensorRecord.value("lowyellow").toDouble();
			double topWarning = sensorRecord.value("highyellow").toDouble();

			imeas::CMeasurementRange measurementRange;

			measurementRange.SetLowerErrorLimit(bottomError);
			measurementRange.SetUpperErrorLimit(topError);
			measurementRange.SetLowerWarningLimit(bottomWarning);
			measurementRange.SetUpperWarningLimit(topWarning);

			sensor->SetMeasurementRange(measurementRange);

			// Set sensor specification:
			int modelId = sensorRecord.value("sensor").toInt();

			QSqlTableModel specificationTable(this, m_tableModelPtr->database());
			specificationTable.setTable(CalculateFullTableName("models"));
			if (specificationTable.select()){
				for (int row = 0; row < specificationTable.rowCount(); row++){
					QSqlRecord rowRecord = specificationTable.record(row);

					int id = rowRecord.value("id").toInt();
					if (id == modelId){
						inemo::CNemoSensorSpecification specification;
						istd::CRange range(rowRecord.value("low").toDouble(), rowRecord.value("high").toDouble());
						specification.SetRange(range);
						sensor->SetSpecification(specification);
						break;
					}
				}
			}

			// Set measurement data:
			int sensorId = sensorRecord.value("id").toInt();
			QString measurementTableName = CalculateFullTableName(QString("sensor%1").arg(sensorId, 5, 10, QLatin1Char('0')));
			QString sqlQuery = QString("SELECT time, value, predict_a, predict_b FROM %1").arg(measurementTableName);
			QSqlQuery query(sqlQuery, m_tableModelPtr->database());
			bool result = query.exec();
			if (result){
				acf::Sequence measuredData;
				acf::Sequence predictedDataA;
				acf::Sequence predictedDataB;
				double prevx = -1;

				int timeColumn = query.record().indexOf("time");
				int valueColumn = query.record().indexOf("value");
				int predictaColumn = query.record().indexOf("predict_a");
				int predictbColumn = query.record().indexOf("predict_b");
				while (query.next()) {
					double x = query.value(timeColumn).toDouble();
					if (x < prevx){
						int a = x;
					}
					else{
						prevx = x;
					}

					double value = query.value(valueColumn).toDouble();
					double predicedValueA = query.value(predictaColumn).toDouble();
					double predicedValueB = query.value(predictbColumn).toDouble();
			
					measuredData.addPoint(x, value);
					predictedDataA.addPoint(x, predicedValueA);
					predictedDataB.addPoint(x, predicedValueB);
				}

				inemo::CNemoSensorData sensorData;
				sensorData.SetMeasurementData(measuredData);
				sensorData.SetPredictedDataA(predictedDataA);
				sensorData.SetPredictedDataB(predictedDataB);

				sensor->SetData(sensorData);
			}

			m_sensors.push_back(sensor);
		}
	}
}


} // namespace iqtnemo
