#pragma once


// ACF includes
#include <icmm/CCmyColorModel.h>


namespace icmm
{


/**
	CMYK color model implementation for four-color printing.
	
	\section CmykColorModelPurpose Purpose
	CCmykColorModel extends CCmyColorModel to represent the full CMYK (Cyan, Magenta,
	Yellow, Black) color model used in printing. The addition of black (K) ink provides
	better shadow detail, reduced ink consumption, and improved print quality compared
	to CMY alone.

	\section CmykColorModelDesign Design
	CCmykColorModel provides:
	- **Four colorants**: Cyan, Magenta, Yellow, Black (K)
	- **Colorant management**: Query and manipulate individual ink channels
	- **Subspace creation**: Create models with subset of colorants
	- **Printing optimization**: Black generation and ink limiting

	\section CmykColorModelUsage Usage Examples
	\code
	// Example 1: Basic CMYK model usage
	icmm::CCmykColorModel cmykModel;
	
	// Query model properties
	IColorModel::ModelType type = cmykModel.GetModelType();  // MT_CMYK
	int dims = cmykModel.GetColorSpaceDimensionality();      // 4
	
	// Get colorant IDs
	icmm::ColorantIds colorants = cmykModel.GetColorantIds();
	// Returns list: ["Cyan", "Magenta", "Yellow", "Black"]

	// Example 2: Querying colorant information
	void InspectCmykColorants(const icmm::CCmykColorModel& model)
	{
		icmm::ColorantIds ids = model.GetColorantIds();
		
		for (const icmm::ColorantId& id : ids) {
			icmm::ColorantUsage usage = model.GetColorantUsage(id);
			
			qDebug() << "Colorant:" << id;
			qDebug() << "Usage:" << (int)usage;
			
			// Usage indicates if colorant is process, spot, etc.
		}
	}

	// Example 3: Getting black colorant specifically
	icmm::ColorantId GetBlackColorant()
	{
		// Static method to get black colorant ID
		return icmm::CCmykColorModel::GetBlack();
	}

	// Example 4: Creating subspace models
	icmm::ISubstractiveColorModel* CreateCmyOnly()
	{
		icmm::CCmykColorModel cmykModel;
		
		// Create subspace with only CMY (no black)
		QStringList cmyColorants;
		cmyColorants << "Cyan" << "Magenta" << "Yellow";
		
		std::unique_ptr<icmm::ISubstractiveColorModel> cmyModel = 
			cmykModel.CreateSubspaceModel(cmyColorants);
		
		return cmyModel.release();
	}

	// Example 5: Converting RGB to CMYK
	void ConvertRgbToCmyk(const icmm::CVarColor& rgb)
	{
		icmm::CRgbColorModel rgbModel;
		icmm::CCmykColorModel cmykModel;
		
		// Create transformation
		const icmm::IColorTransformation* transform = 
			rgbModel.CreateColorTranformation(cmykModel);
		
		if (transform) {
			icmm::CVarColor cmyk;
			if (transform->Calculate(rgb, cmyk)) {
				double c = cmyk.GetElement(0);  // Cyan
				double m = cmyk.GetElement(1);  // Magenta
				double y = cmyk.GetElement(2);  // Yellow
				double k = cmyk.GetElement(3);  // Black
				
				qDebug() << "CMYK:" << c << m << y << k;
			}
			delete transform;
		}
	}

	// Example 6: Checking ink coverage
	double CalculateInkCoverage(const icmm::CVarColor& cmyk)
	{
		// Total Area Coverage (TAC) - sum of all ink percentages
		double tac = 0.0;
		
		for (int i = 0; i < cmyk.GetElementsCount(); ++i) {
			tac += cmyk.GetElement(i);
		}
		
		// TAC typically should not exceed 300-400% depending on press
		return tac * 100.0;  // Convert to percentage
	}

	// Example 7: Working with spot colors
	void AddSpotColorant(icmm::CCmykColorModel& model)
	{
		// CMYK model can be extended with spot colors
		// for special printing applications (metallic, varnish, etc.)
		
		// Get current colorants
		icmm::ColorantIds ids = model.GetColorantIds();
		
		// Could add additional colorants for extended gamut printing
		// (e.g., Orange, Green for 6-color CMYK+OG)
	}

	// Example 8: Comparing CMYK models
	bool ModelsSupportSameColorants(const icmm::CCmykColorModel& model1,
	                               const icmm::CCmykColorModel& model2)
	{
		icmm::ColorantIds ids1 = model1.GetColorantIds();
		icmm::ColorantIds ids2 = model2.GetColorantIds();
		
		if (ids1.size() != ids2.size()) {
			return false;
		}
		
		// Check if all colorants match
		for (const auto& id : ids1) {
			if (!ids2.contains(id)) {
				return false;
			}
		}
		
		return true;
	}
	\endcode

	\section CmykColorModelBlackGen Black Generation
	The "K" component provides several benefits:
	- **Better shadows**: Pure black ink creates deeper blacks than CMY mix
	- **Reduced ink**: Replacing CMY with K saves expensive color inks
	- **Faster drying**: Less total ink on paper
	- **Registration**: Easier to align (K carries most shadow detail)
	
	Common strategies:
	- **GCR** (Gray Component Replacement): Replace CMY gray with K
	- **UCR** (Under Color Removal): Remove CMY under blacks
	- **Maximum K**: Use K for all neutral tones
	- **Minimum K**: Use K only in deep shadows

	\section CmykColorModelApplications Applications
	- **Commercial printing**: Offset, digital, screen printing
	- **Desktop publishing**: Print preparation and proofing
	- **Color separation**: Converting RGB artwork to print
	- **Ink optimization**: Reducing costs and improving quality
	- **Spot color**: Extended gamut and special inks

	\section CmykColorModelBestPractices Best Practices
	- Monitor total ink coverage (TAC) - typically max 300-400%
	- Use appropriate black generation for content type
	- Consider paper type (coated vs uncoated affects ink limits)
	- Proof on actual print device when possible
	- Use ICC profiles for accurate RGB→CMYK conversion
	- Check registration and trapping requirements

	\sa icmm::CCmyColorModel, icmm::ISubstractiveColorModel, icmm::CCmyk,
	    icmm::CRgbToCmykTransformation, icmm::IColorantList

	\ingroup Color
*/
class CCmykColorModel: public CCmyColorModel
{
public:
	typedef CCmyColorModel BaseClass;

	/**
		Gets the colorant ID for the black (K) component.
		
		Static method to retrieve the standard identifier for the black
		colorant in CMYK printing. Useful for querying or manipulating
		the black channel specifically.
		
		\return ColorantId for black component
		
		\sa GetColorantIds()
	*/
	static ColorantId GetBlack();

	// reimplemented (icmm::IColorantList)
	virtual ColorantIds GetColorantIds() const override;
	virtual icmm::ColorantUsage GetColorantUsage(const ColorantId& colorantId) const override;

	// reimplemented ISubstractiveColorModel
	virtual std::unique_ptr<ISubstractiveColorModel> CreateSubspaceModel(const QStringList& colorantIds) const override;
};


} // namespace icmm


