#ifndef iprm_CSelectionParam_included
#define iprm_CSelectionParam_included


#include "iser/ISerializable.h"

#include "iprm/ISelectionParam.h"


namespace iprm
{


/**
	Basic implementation of selection parameter.
*/
class CSelectionParam: virtual public ISelectionParam
{
public:
	CSelectionParam();

	void SetSelectionConstraints(const ISelectionConstraints* constraintsPtr);

	/**
		Set selection index according to a given option ID.
	*/
	bool SetSelectedOptionById(const QByteArray& selectedOptionId);

	// reimplemented (iprm::ISelectionParam)
	virtual const ISelectionConstraints* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	int m_selectedOptionIndex;

private:
	const ISelectionConstraints* m_constraintsPtr;
};


} // namespace iprm


#endif // !iprm_CSelectionParam_included


