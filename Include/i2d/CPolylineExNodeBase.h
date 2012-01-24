#ifndef i2d_CPolylineExNodeBase_included
#define i2d_CPolylineExNodeBase_included


// ACF includes
#include "i2d/CPolyline.h"


namespace i2d
{


class CPolylineExNodeBase: public CPolyline
{
public:
	typedef CPolyline BaseClass;

	/** Get segment user data.
	*/
	virtual const iser::ISerializable& GetNodeData(int nodeIndex) const = 0;
	virtual iser::ISerializable& GetNodeDataRef(int nodeIndex) = 0;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


} // namespace i2d


#endif // !i2d_CPolylineExNodeBase_included

