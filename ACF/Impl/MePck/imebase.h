#ifndef imebase_included
#define imebase_included

#include <QList>
#include <QVector>

#include "istd\CRange.h"
#include "isig\ISamplesContainer.h"
#include "iproc\TIAssyncProcessor.h"

#include "medriver.h"
#include "medefines.h"
#include "metypes.h"
#include "meerror.h"



namespace imebase
{

	
/**
	Logical address of hardware endpoint.
*/
class CMeAddr
{
public:
	int device;
	int subdevice;
	int channel;
};

/**
	Direction of endpont.
*/
enum EMeDirection
{
	input,
	output
};

typedef QList<CMeAddr> CMeList;

typedef QVector<double> imebase_Buffer;


} // namespace imebase


#endif // !imebase_included
