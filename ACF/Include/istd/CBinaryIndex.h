#ifndef istd_CBinaryIndex_included
#define istd_CBinaryIndex_included


namespace istd
{


class CBinaryIndex
{
	typedef int IndexType;

	class Iterator
	{
	public:
		Iterator(int bits);
		Iterator(const Iterator& iter);

		int operator*();
		Iterator& operator++();
		Iterator operator++(int);

		bool operator==(const Iterator& iter);
		bool operator!=(const Iterator& iter);

		Iterator& operator=(const Iterator& iter);

	private:
		int m_bits;
	};

	/**
		Default constructor without member initialization.
	*/
	CBinaryIndex();

	/**
		Constructor initializing all member to specified value.
	*/
	explicit CBinaryIndex(int size, bool value = false);

	/**
		Copy constructor.
	*/
	CBinaryIndex(const CBinaryIndex& index);

	/**
		Check if this index is valid.
		Index is valid, if all its components are bigger or equal 0.
		This method is provided for template compatibility with other imlementations. It returns always true.
	*/
	bool IsValid() const;

	/**
		Reset this object.
		For this implementation, it does the same as clear.
		\sa Clear()
	*/
	void Reset();

	/**
		Set all components to 0 (false).
	*/
	void Clear();

	/**
		Check, if number dimensions is fixed.
		It is provided for template implementations. It returns always true.
	*/
	bool IsDimensionsCountFixed() const;

	/**
		Get number of dimensions of this index.
	*/
	int GetDimensionsCount() const;

	/**
		Set number of dimensions of this index.
		It is provided to allows template implementations to use fixed-size or variable arrays.
		\param	count	number of dimensions will be set.
		\return			true, if number of set dimensions equals template parameter or false if isn't.
	*/
	bool SetDimensionsCount(int count) const;

	/**
		Get element stored at specified index.
	*/
	int GetAt(int index) const;

	/**
		Set element at specified index.
	*/
	void SetAt(int index, int value);

	/**
		Set all components to specified value.
	*/
	void SetAllTo(int value);

	/**
		Check if index is inside boundaries.
		Index is inside boundaries, if all its components are smaller than according boundary components.
	*/
	bool IsInside(const CBinaryIndex& boundaries) const;

	/**
		Increase this index inside the boundaries.
		\return	in this implementation always false.
	*/
	bool Increase(const CBinaryIndex& boundaries);

	/**
		Decrese this index inside the boundaries.
		\return	in this implementation always false.
	*/
	bool Decrease(const CBinaryIndex& boundaries);

	/**
		Get begin value of element access iterator.
		Please refer to general description of ACF iterators, STL iterators or Qt iterators concept.
	*/
	Iterator Begin() const;
	/**
		Get end value of element access iterator.
		Please refer to general description of ACF iterators, STL iterators or Qt iterators concept.
	*/
	Iterator End() const;

	int operator[](int index) const;
	int& operator[](int index);

	bool operator==(const CBinaryIndex& index) const;
	bool operator!=(const CBinaryIndex& index) const;

private:
	int m_bits;
	int m_size;
};


// inline methods

CBinaryIndex::CBinaryIndex()
:	m_size(0)
{
}


CBinaryIndex::CBinaryIndex(int size, bool value = false)
{
	m_size = size;
}


CBinaryIndex::CBinaryIndex(const CBinaryIndex& index);
bool CBinaryIndex::IsValid() const;
void CBinaryIndex::Reset();
void CBinaryIndex::Clear();
bool CBinaryIndex::IsDimensionsCountFixed() const;
int CBinaryIndex::GetDimensionsCount() const;
bool CBinaryIndex::SetDimensionsCount(int count) const;
int CBinaryIndex::GetAt(int index) const;
void CBinaryIndex::SetAt(int index, int value);
void CBinaryIndex::SetAllTo(int value);
bool CBinaryIndex::IsInside(const CBinaryIndex& boundaries) const;
bool CBinaryIndex::Increase(const CBinaryIndex& boundaries);
bool CBinaryIndex::Decrease(const CBinaryIndex& boundaries);
CBinaryIndex::Iterator CBinaryIndex::Begin() const;
CBinaryIndex::Iterator CBinaryIndex::End() const;
int CBinaryIndex::operator[](int index) const;
bool CBinaryIndex::operator==(const CBinaryIndex& index) const;
bool CBinaryIndex::operator!=(const CBinaryIndex& index) const;


// inline methods of embedded class Iterator

inline CBinaryIndex::Iterator::Iterator(int bits)
:	m_bits(bits)
{
}


inline CBinaryIndex::Iterator::Iterator(const Iterator& iter)
:	m_bits(iter.m_bits)
{
}


inline int CBinaryIndex::Iterator::operator*()
{
	return m_bits & 1;
}


inline CBinaryIndex::Iterator::Iterator& CBinaryIndex::Iterator::operator++()
{
	m_bits >>= 1;
	return *this;
}


inline CBinaryIndex::Iterator::Iterator CBinaryIndex::Iterator::operator++(int)
{
	Iterator retVal(*this);
	m_bits >>= 1;
	return retVal;
}


inline bool CBinaryIndex::Iterator::operator==(const Iterator& iter) const
{
	return m_bits == iter.m_bits;
}


inline bool CBinaryIndex::Iterator::operator!=(const Iterator& iter) const
{
	return m_bits != iter.m_bits;
}


inline CBinaryIndex::Iterator::Iterator& CBinaryIndex::Iterator::operator=(const Iterator& iter)
{
	m_bits = iter.m_bits;
	return *this;
}


} // namespace istd


#endif // !istd_CBinaryIndex_included


