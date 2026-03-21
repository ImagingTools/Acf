// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <istd/Test/CInterfacePtrTest.h>


// ACF includes
#include <istd/TInterfacePtr.h>
#include <istd/TOptInterfacePtr.h>
#include <itest/CStandardTestExecutor.h>


class IInterface1: public istd::IPolymorphic
{
};

class IInterface2: public istd::IPolymorphic
{
};

class IInterface22: public IInterface2
{
};

class T1 : public IInterface1
{
};

class T2 : public IInterface2
{
};

class T22 : public IInterface22
{
};

class TCompositeRoot : public istd::IPolymorphic
{
public:
	IInterface1* GetInterface()
	{
		return &m_t1;
	}

private:
	T1 m_t1;
};


void CInterfacePtrTest::DoSharedInterfaceTest()
{
	// basic operations
	{
		istd::TSharedInterfacePtr<IInterface1> sharedPtr(new T1);
		QVERIFY(sharedPtr.IsValid());

		istd::TSharedInterfacePtr<IInterface1> sharedPtr2 = sharedPtr;
		QVERIFY(sharedPtr2.IsValid());
		QVERIFY(sharedPtr.GetBasePtr().use_count() == 2);

		istd::TSharedInterfacePtr<const IInterface1> sharedConstPtr;
		sharedConstPtr = sharedPtr;
		QVERIFY(sharedConstPtr.IsValid());
		QVERIFY(sharedConstPtr.GetBasePtr().use_count() == 3);
		
		sharedConstPtr = std::make_shared<T1>();
		QVERIFY(sharedConstPtr.IsValid());
		QVERIFY(sharedConstPtr);
		QVERIFY(sharedPtr.GetBasePtr().use_count() == 2);

		sharedPtr.Reset();
		QVERIFY(!sharedPtr.IsValid());

		sharedPtr2.Reset();
		QVERIFY(!sharedPtr2.IsValid());
		QVERIFY(sharedConstPtr.IsValid());
		sharedConstPtr.Reset();
		QVERIFY(!sharedConstPtr.IsValid());
	}

	// assign from std::unique_ptr
	{
		istd::TSharedInterfacePtr<const IInterface1> sharedConstPtr;
		sharedConstPtr = std::make_unique<T1>();
		QVERIFY(sharedConstPtr.IsValid());
	}

	// create from std::shared_ptr
	{
		std::shared_ptr<T1> stdSharedPtr = std::make_shared<T1>();
		istd::TSharedInterfacePtr<IInterface1> sharedPtr(stdSharedPtr);
		QVERIFY(sharedPtr.IsValid());
		QVERIFY(stdSharedPtr.use_count() == 2);
	}

	// move semantics
	{
		istd::TSharedInterfacePtr<IInterface1> countedPtr(new T1);
		QVERIFY(countedPtr.IsValid());

		istd::TSharedInterfacePtr<IInterface1> countedCopy = countedPtr;
		QVERIFY(countedPtr.GetBasePtr().use_count() == 2);

		istd::TSharedInterfacePtr<IInterface1> countedMoved(std::move(countedCopy));
		QVERIFY(!countedCopy.IsValid());
		QVERIFY(countedMoved.IsValid());
		QVERIFY(countedMoved.GetBasePtr().use_count() == 2);

		istd::TSharedInterfacePtr<IInterface1> countedAssigned;
		countedAssigned = std::move(countedMoved);
		QVERIFY(!countedMoved.IsValid());
		QVERIFY(countedAssigned.IsValid());
		QVERIFY(countedAssigned.GetBasePtr().use_count() == 2);

		countedPtr.Reset();
		QVERIFY(countedAssigned.IsValid());
		QVERIFY(countedAssigned.GetBasePtr().use_count() == 1);

		countedAssigned.Reset();
		QVERIFY(!countedAssigned.IsValid());
	}

	// assignment from derived shared pointer
	{
		istd::TSharedInterfacePtr<IInterface22> sharedPtrDerived(new T22);
		istd::TSharedInterfacePtr<IInterface2> sharedPtrBase;
		sharedPtrBase = sharedPtrDerived;
		QVERIFY(sharedPtrBase.IsValid());
		QVERIFY(sharedPtrDerived.IsValid());
		QVERIFY(sharedPtrBase.GetBasePtr().use_count() == 2);
	}

	// test dynamicCast() success
	{
		istd::TSharedInterfacePtr<IInterface2> sharedPtrBase(new T22);
		auto sharedPtrDerived = sharedPtrBase.dynamicCast<IInterface22>();
		QVERIFY(sharedPtrDerived.IsValid());
		QVERIFY(sharedPtrDerived.GetBasePtr().use_count() == 2);
	}
	// test dynamicCast() failure
	{
		istd::TSharedInterfacePtr<IInterface2> sharedPtrBase(new T2);
		auto sharedPtrDerived = sharedPtrBase.dynamicCast<IInterface22>();
		QVERIFY(!sharedPtrDerived.IsValid());
		QVERIFY(sharedPtrBase.GetBasePtr().use_count() == 1);
	}
	// test dynamicCast() on const
	{
		// TODO:
		// const istd::TSharedInterfacePtr<IInterface2> sharedPtrBase(new T22);
		// auto sharedPtrDerived = sharedPtrBase.dynamicCast<IInterface22>();
		// QVERIFY(sharedPtrDerived.IsValid());
		// QVERIFY(sharedPtrDerived.GetBasePtr().use_count() == 2);
	}

	// test SetCastedPtr() success
	{
		istd::TSharedInterfacePtr<IInterface22> sharedPtrDerived(new T22);
		istd::TSharedInterfacePtr<IInterface2> sharedPtrBase;
		QVERIFY(sharedPtrBase.SetCastedPtr(sharedPtrDerived));
		QVERIFY(sharedPtrBase.IsValid());
		QVERIFY(sharedPtrDerived.IsValid());
		QVERIFY(sharedPtrBase.GetBasePtr().use_count() == 2);
	}

	// test SetCastedPtr() failure keeps target unchanged
	{
		istd::TSharedInterfacePtr<IInterface2> sharedPtrBase(new T22);
		IInterface2* originalInterface = sharedPtrBase.GetPtr();
		istd::TSharedInterfacePtr<IInterface1> incompatiblePtr(new T1);
		QVERIFY(!sharedPtrBase.SetCastedPtr(incompatiblePtr));
		QVERIFY(sharedPtrBase.IsValid());
		QVERIFY(sharedPtrBase.GetPtr() == originalInterface);
		QVERIFY(incompatiblePtr.IsValid());
	}

	// separate root and interface pointers share ownership correctly
	{
		TCompositeRoot* root = new TCompositeRoot;
		istd::TSharedInterfacePtr<IInterface1> compositePtr(root, [root]() { return root->GetInterface(); });
		QVERIFY(compositePtr.IsValid());
		QVERIFY(compositePtr.GetBasePtr().get() == root);
		QVERIFY(compositePtr.GetPtr() == root->GetInterface());
		QVERIFY(compositePtr.GetPtr() != compositePtr.GetBasePtr().get());

		istd::TSharedInterfacePtr<IInterface1> compositeCopy = compositePtr;
		QVERIFY(compositeCopy.GetBasePtr().use_count() == 2);

		compositePtr.Reset();
		QVERIFY(compositeCopy.IsValid());
		QVERIFY(compositeCopy.GetBasePtr().use_count() == 1);

		compositeCopy.Reset();
		QVERIFY(!compositeCopy.IsValid());
	}

	// conversion from unique through constructor
	{
		istd::TUniqueInterfacePtr<IInterface22> uniqueDerived(new T22);
		istd::TSharedInterfacePtr<IInterface2> sharedBase(std::move(uniqueDerived));
		QVERIFY(sharedBase.IsValid());
		QVERIFY(!uniqueDerived.IsValid());
		QVERIFY(sharedBase.GetBasePtr().use_count() == 1);
	}

	// conversion from unique through FromUnique()
	{
		istd::TUniqueInterfacePtr<IInterface1> uniquePtr(new T1);
		istd::TSharedInterfacePtr<IInterface1> sharedFromUnique;
		sharedFromUnique.FromUnique(uniquePtr);
		QVERIFY(sharedFromUnique.IsValid());
		QVERIFY(!uniquePtr.IsValid());
		QVERIFY(sharedFromUnique.GetBasePtr().use_count() == 1);

		istd::TUniqueInterfacePtr<IInterface1> emptyUnique;
		sharedFromUnique.FromUnique(emptyUnique);
		QVERIFY(!sharedFromUnique.IsValid());
	}

	// CreateFromUnique() cast success/failure
	{
		istd::TUniqueInterfacePtr<IInterface22> uniqueDerived(new T22);
		auto sharedBase = istd::TSharedInterfacePtr<IInterface2>::CreateFromUnique(uniqueDerived);
		QVERIFY(sharedBase.IsValid());
		QVERIFY(!uniqueDerived.IsValid());

		istd::TUniqueInterfacePtr<IInterface1> incompatibleUnique(new T1);
		auto incompatibleShared = istd::TSharedInterfacePtr<IInterface2>::CreateFromUnique(incompatibleUnique);
		QVERIFY(!incompatibleShared.IsValid());
		QVERIFY(incompatibleUnique.IsValid());
	}

	// MoveCastedPtr() success and failure
	{
		istd::TUniqueInterfacePtr<IInterface22> uniqueDerived(new T22);
		istd::TSharedInterfacePtr<IInterface2> sharedBase;
		QVERIFY(sharedBase.MoveCastedPtr(std::move(uniqueDerived)));
		QVERIFY(sharedBase.IsValid());
		QVERIFY(!uniqueDerived.IsValid());

		istd::TSharedInterfacePtr<IInterface2> existingShared(new T22);
		IInterface2* originalInterface = existingShared.GetPtr();
		istd::TUniqueInterfacePtr<IInterface1> incompatibleUnique(new T1);
		QVERIFY(!existingShared.MoveCastedPtr(std::move(incompatibleUnique)));
		QVERIFY(existingShared.IsValid());
		QVERIFY(existingShared.GetPtr() == originalInterface);
		QVERIFY(incompatibleUnique.IsValid());
	}
}


static istd::TSharedInterfacePtr<IInterface1> returningStdSharedPtrCompiles()
{
	return std::make_shared<T1>();
}


void CInterfacePtrTest::DoUniqueInterfaceTest()
{
	istd::TUniqueInterfacePtr<IInterface1> ptr1(new T1);
	QVERIFY(ptr1.IsValid());

	istd::TSharedInterfacePtr<IInterface1> sharedPtr = istd::TSharedInterfacePtr<IInterface1>::CreateFromUnique(ptr1);
	QVERIFY(sharedPtr.IsValid());
	QVERIFY(!ptr1.IsValid());

	istd::TUniqueInterfacePtr<IInterface1> ptr2(std::make_unique<T1>());
	QVERIFY(ptr2.IsValid());

	istd::TUniqueInterfacePtr<IInterface2> ptr3(std::make_unique<T22>());
	QVERIFY(ptr3.IsValid());

	istd::TUniqueInterfacePtr<const IInterface1> ptr4(std::make_unique<T1>());
	QVERIFY(ptr4.IsValid());
}


void CInterfacePtrTest::DoOptionalInterfaceTest()
{
	istd::TSharedInterfacePtr<IInterface1> sharedPtr(new T1);
	QVERIFY(sharedPtr.IsValid());

	T1 instance1;
	istd::TOptInterfacePtr<IInterface1> opt1;
	opt1.SetUnmanagedPtr(&instance1);
	QVERIFY(opt1.IsValid());
	QVERIFY(opt1.IsUnmanaged());
	QVERIFY(!opt1.IsManaged());

	istd::TOptInterfacePtr<IInterface1> opt2(sharedPtr);
	QVERIFY(opt2.IsValid());
	QVERIFY(sharedPtr.IsValid());

	sharedPtr.Reset();
	QVERIFY(opt2.IsValid());

	opt2 = opt1;
	QVERIFY(opt2.IsValid());
	QVERIFY(opt2.IsUnmanaged());
	QVERIFY(!opt2.IsManaged());

	// Incompatible interfaces
	{
		istd::TUniqueInterfacePtr<IInterface2> t2Ptr(new T2);
		QVERIFY(t2Ptr.IsValid());
		istd::TOptInterfacePtr<IInterface1> optInterface1;
		QVERIFY(!optInterface1.TakeOver(t2Ptr));
		QVERIFY(!optInterface1.IsValid());
		QVERIFY(t2Ptr.IsValid());
	}

	// Same interfaces
	{
		istd::TUniqueInterfacePtr<IInterface1> t1tr(new T1);
		QVERIFY(t1tr.IsValid());
		istd::TOptInterfacePtr<IInterface1> optInterface1;
		QVERIFY(optInterface1.TakeOver(t1tr));
		QVERIFY(optInterface1.IsValid());
		QVERIFY(optInterface1.IsManaged());
		QVERIFY(!t1tr.IsValid());
	}

	// Compatible interfaces
	{
		istd::TSharedInterfacePtr<IInterface22> t2Ptr(new T22);
		QVERIFY(t2Ptr.IsValid());
		istd::TOptInterfacePtr<IInterface2> optInterface1;
		QVERIFY(optInterface1.SetCastedPtr(t2Ptr));
		QVERIFY(optInterface1.IsValid());
		QVERIFY(t2Ptr.IsValid());
	}

	// Adopt
	{
		
		IInterface22* t2Ptr = new T22;
		istd::TOptInterfacePtr<IInterface2> optInterface1;
		QVERIFY(optInterface1.AdoptCastedRawPtr(t2Ptr));
		QVERIFY(optInterface1.IsValid());
	}
}


I_ADD_TEST(CInterfacePtrTest);


