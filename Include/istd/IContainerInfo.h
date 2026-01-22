// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace istd
{


/**
	Common interface for all container implementations.
*/
class IContainerInfo: virtual public IChangeable
{
public:
	/**
		Returns number of elements in the container.
	*/
	virtual int GetItemsCount() const = 0;

	/**
		Returns a \c true if container is empty, otherwise a \c false.
	*/
	virtual bool IsEmpty() const = 0;

	/*
		Returns \c true if the index \c index is valid, otherwise a \c false.
	*/
	virtual bool IsIndexValid(int index) const = 0;
};


} // namespace istd



