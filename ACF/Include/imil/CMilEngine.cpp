#include "imil/CMilEngine.h"


namespace imil
{


// public methods

CMilEngine::CMilEngine()
{
	s_criticalSection.Enter();
	if (s_referenceCount == 0){
		MappAllocDefault(M_SETUP, &s_milApplication, &s_milSystemId, M_NULL, M_NULL, M_NULL);
	}
	s_referenceCount++;
	s_criticalSection.Leave();
}


CMilEngine::~CMilEngine()
{
	s_criticalSection.Enter();
	if (s_referenceCount == 1){
		MappFreeDefault(s_milApplication, s_milSystemId, M_NULL, M_NULL, M_NULL);
		s_milApplication = M_NULL;
		s_milSystemId = M_NULL;
	}
	s_referenceCount--;
	s_criticalSection.Leave();
}


MIL_ID CMilEngine::GetSystemId() const
{
	I_ASSERT(s_milSystemId != M_NULL);

	return s_milSystemId;
}


// private static members

MIL_ID CMilEngine::s_milSystemId = M_NULL;
MIL_ID CMilEngine::s_milApplication = M_NULL;
int CMilEngine::s_referenceCount = 0;
inat::CCriticalSection CMilEngine::s_criticalSection;


} // namespace imil


