// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CComponentTeardownCompTest.h"


// ACF includes
#include <iprm/ITextParam.h>


// private slots

void CComponentTeardownCompTest::testDestroyWithAttributeError()
{
	std::unique_ptr<CComponentTeardownTest> instancePtr(new CComponentTeardownTest);

	// DefaultText is a text attribute declared as String in the registry, so resolving it reports an error with the context hierarchy
	QVERIFY(instancePtr->GetInterface<iprm::ITextParam>("MistypedText") != nullptr);

	// Teardown resolves the attribute again and must not walk a released parent context while reporting the error
	instancePtr.reset();
}


I_ADD_TEST(CComponentTeardownCompTest);
