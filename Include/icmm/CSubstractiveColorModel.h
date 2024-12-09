#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <iser/CArchiveTag.h>
#include <icmm/CSubstractiveColorModelBase.h>


namespace icmm
{


/**
	Common implementation of the general device-based, substractive color model based on the list of colorants.
	All kinds of colorants (Process, ECG and Spot) can be combined in this color model.
*/
class CSubstractiveColorModel:
			public CSubstractiveColorModelBase,
			virtual public iser::ISerializable
{
public:
	CSubstractiveColorModel();
	CSubstractiveColorModel(const ColorantIds& colorantIds);
	CSubstractiveColorModel(const CSubstractiveColorModel& other);
	CSubstractiveColorModel(const ISubstractiveColorModel& other);

	bool operator==(const CSubstractiveColorModel& ref) const;
	bool operator!=(const CSubstractiveColorModel& ref) const;

	/**
		Returns \c true if the colorant with the given ID exists in the color model.
	*/
	bool ContainsColorant(const ColorantId& colorantId) const;

	/**
		Insert a new colorant at the given position.
		If a colorant with the same colorant-ID already exists, the method will fail.
		\param index	Position, where the new colorant will be inserted.
		A negative value means the new colorant will be inserted at the end of the colorant list.
	*/
	bool InsertColorant(const ColorantId& colorantId, ColorantUsage usage, int index = -1);

	/**
		Remove a colorant with the given ID.
		\return Returns \c true, if the operation was succesful or \c false otherwise.
	*/
	bool RemoveColorant(const ColorantId& colorantId);

	/**
		Set a colorant usage for an existing colorant.
	*/
	bool SetColorantUsage(const ColorantId& colorantId, ColorantUsage usage);

	/**
		Set a new ID for an existing colorant.
	*/
	bool SetColorantId(const ColorantId& colorantId, const ColorantId& newColorantId);

	/**
		Append colorants from from another model to this one.
		\return Returns \c true, if the operation was succesful or \c false otherwise.
	*/
	bool AppendColorModel(const ISubstractiveColorModel& other);

	/**
		Check if the color model contains process colorants.
	*/
	bool HasProcessColorants() const;

	/**
		Check if the color model contains ECG colorants.
	*/
	bool HasEcg() const;

	/**
		Check if the color model contains any spot colors.
	*/
	bool HasSpot() const;

	/**
		Get the list of process colors in this color model.
	*/
	ColorantIds GetProcessColorants() const;

	/**
		Get the list of ECG colorants in this color model.
	*/
	ColorantIds GetEcgColorants() const;

	/**
		Get the list of spot colors in this color model.
	*/
	ColorantIds GetSpotColorants() const;

	// reimplemented (icmm::IColorantList)
	virtual ColorantIds GetColorantIds() const override;
	virtual ColorantUsage GetColorantUsage(const ColorantId& colorantId) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	struct ColorantInfo
	{
		ColorantId id;
		ColorantUsage usage = CU_NONE;

		inline bool operator==(const ColorantInfo& ref) const
		{
			return (ref.id == id) && (ref.usage == usage);
		}

		inline bool operator!=(const ColorantInfo& ref)  const
		{
			return !operator==(ref);
		}
	};

	typedef QVector<ColorantInfo> ColorantInfoList;

protected:
	static icmm::ColorantUsage GetDefaultUsageFromColorantName(const ColorantId& colorantId);
	static ColorantId GetEcgGreen();
	static ColorantId GetEcgOrange();
	static ColorantId GetEcgViolet();
	static ColorantId GetEcgRed();
	static ColorantId GetEcgBlue();

	int FindColorantIndex(const ColorantId& colorantId) const;
	bool SerializeColorantInfo(
				iser::IArchive& archive,
				ColorantInfo& colorantInfo,
				const iser::CArchiveTag* parentTagPtr) const;

private:
	ColorantInfoList m_colorants;
};


} // namespace icmm


