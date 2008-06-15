#ifndef imeids_CMeBase_included
#define imeids_CMeBase_included

#include "imebase.h"

namespace imebase
{
	class CMeBase
	{
//		Q_OBJECT

	public:
		CMeBase();
		~CMeBase();

		CMeList& GetMeList(int direction);
		CMeList& GetMeInputList();
		CMeList& GetMeOutputList();
		bool CheckAddress(CMeAddr address, int direction) const;

	private:
		CMeList MeInputList;
		CMeList MeOutputList;

		/**
			This is dummy list.
		*/
		CMeList MeEmptyList;
	};

} // namespace imebase


#endif // !imeids_CMeBase_included


