// ACF includes
#include "CGamutMapper.h"
#include <icmm/CVarColor.h>
#include <imath/imath.h>

// Qt includes
#include <QtCore/qmath.h>


namespace iimgprint
{


// ============================================================================
// CClippingGamutMapper Implementation
// ============================================================================


class CClippingGamutMapper::Impl
{
public:
	CPrinterProfile targetProfile;
	double gamutVolume;
	double gamutCoverage;
	
	explicit Impl(const CPrinterProfile& profile)
		: targetProfile(profile)
		, gamutVolume(1.0)
		, gamutCoverage(1.0)
	{
		// Initialize gamut metrics from profile
		gamutCoverage = profile.GetGamutCoverage();
		gamutVolume = gamutCoverage; // Simplified - would calculate actual volume
	}
	
	bool IsColorInGamut(const icmm::CVarColor& color) const
	{
		// Simple check: all components must be in valid range [0, 1]
		int count = color.GetElementsCount();
		for (int i = 0; i < count; ++i) {
			double value = color.GetElement(i);
			if (value < 0.0 || value > 1.0) {
				return false;
			}
		}
		
		// Additional check: color must be within printer's reproducible gamut
		// This is simplified - full implementation would use spectral data
		// to determine actual gamut boundary
		return true;
	}
	
	bool ClipToGamut(const icmm::CVarColor& source, icmm::CVarColor& clipped) const
	{
		int count = source.GetElementsCount();
		clipped = icmm::CVarColor(count);
		
		// Clip each component to [0, 1] range
		for (int i = 0; i < count; ++i) {
			double value = source.GetElement(i);
			value = qMax(0.0, qMin(1.0, value));
			clipped.SetElement(i, value);
		}
		
		return true;
	}
	
	double CalculateDistanceToBoundary(const icmm::CVarColor& color) const
	{
		// Calculate minimum distance to gamut boundary
		// Positive = in gamut, negative = out of gamut
		
		double minDistance = 1000.0; // Large positive number
		int count = color.GetElementsCount();
		
		for (int i = 0; i < count; ++i) {
			double value = color.GetElement(i);
			
			// Distance to lower boundary (0)
			double distToLower = value;
			// Distance to upper boundary (1)
			double distToUpper = 1.0 - value;
			
			// Take minimum
			double dist = qMin(distToLower, distToUpper);
			
			// If outside [0,1], distance is negative
			if (value < 0.0) {
				dist = value; // negative
			} else if (value > 1.0) {
				dist = 1.0 - value; // negative
			}
			
			minDistance = qMin(minDistance, dist);
		}
		
		return minDistance;
	}
};


CClippingGamutMapper::CClippingGamutMapper(const CPrinterProfile& targetProfile)
	: m_impl(new Impl(targetProfile))
{
}


CClippingGamutMapper::~CClippingGamutMapper()
{
	delete m_impl;
}


bool CClippingGamutMapper::IsInGamut(const icmm::CVarColor& color) const
{
	return m_impl->IsColorInGamut(color);
}


bool CClippingGamutMapper::MapToGamut(const icmm::CVarColor& sourceColor,
                                      icmm::CVarColor& mappedColor) const
{
	return m_impl->ClipToGamut(sourceColor, mappedColor);
}


double CClippingGamutMapper::GetGamutVolume() const
{
	return m_impl->gamutVolume;
}


double CClippingGamutMapper::GetGamutCoverage() const
{
	return m_impl->gamutCoverage;
}


double CClippingGamutMapper::GetDistanceToGamutBoundary(const icmm::CVarColor& color) const
{
	return m_impl->CalculateDistanceToBoundary(color);
}


// ============================================================================
// CPerceptualGamutMapper Implementation
// ============================================================================


class CPerceptualGamutMapper::Impl
{
public:
	CPrinterProfile targetProfile;
	double compressionKnee;
	double gamutVolume;
	double gamutCoverage;
	
	Impl(const CPrinterProfile& profile, double knee)
		: targetProfile(profile)
		, compressionKnee(knee)
		, gamutVolume(1.0)
		, gamutCoverage(1.0)
	{
		// Initialize gamut metrics
		gamutCoverage = profile.GetGamutCoverage();
		gamutVolume = gamutCoverage;
	}
	
	bool IsColorInGamut(const icmm::CVarColor& color) const
	{
		// Check if color is within reproducible range
		int count = color.GetElementsCount();
		for (int i = 0; i < count; ++i) {
			double value = color.GetElement(i);
			if (value < 0.0 || value > 1.0) {
				return false;
			}
		}
		return true;
	}
	
	bool PerceptualCompress(const icmm::CVarColor& source, icmm::CVarColor& compressed) const
	{
		int count = source.GetElementsCount();
		compressed = icmm::CVarColor(count);
		
		// Apply perceptual compression using sigmoidal function
		// This smoothly compresses colors near and outside gamut boundary
		
		for (int i = 0; i < count; ++i) {
			double value = source.GetElement(i);
			double compressedValue = ApplySigmoidalCompression(value);
			compressed.SetElement(i, compressedValue);
		}
		
		return true;
	}
	
	double ApplySigmoidalCompression(double value) const
	{
		// Sigmoidal compression function
		// Colors within knee point are minimally affected
		// Colors beyond knee point are smoothly compressed
		
		if (value >= 0.0 && value <= compressionKnee) {
			// Within knee - minimal compression
			return value * (compressionKnee / compressionKnee); // Linear in this region
		}
		else if (value > compressionKnee && value <= 1.0) {
			// Between knee and gamut boundary - smooth compression
			double normalized = (value - compressionKnee) / (1.0 - compressionKnee);
			// Apply ease-out curve
			double compressed = 1.0 - qPow(1.0 - normalized, 2.0);
			return compressionKnee + compressed * (1.0 - compressionKnee) * 0.95;
		}
		else if (value > 1.0) {
			// Out of gamut - strong compression to fit within
			double excess = value - 1.0;
			// Compress excess using asymptotic function
			double compressedExcess = excess / (1.0 + excess);
			return 1.0 - (1.0 - compressionKnee) * 0.05 + compressedExcess * (1.0 - compressionKnee) * 0.05;
		}
		else {
			// Below 0 - compress symmetrically
			double absValue = qAbs(value);
			double compressed = ApplySigmoidalCompression(absValue);
			return -compressed;
		}
	}
	
	double CalculateDistanceToBoundary(const icmm::CVarColor& color) const
	{
		// Calculate perceptual distance to gamut boundary
		double minDistance = 1000.0;
		int count = color.GetElementsCount();
		
		for (int i = 0; i < count; ++i) {
			double value = color.GetElement(i);
			
			// Distance considering compression knee
			double dist;
			if (value >= 0.0 && value <= compressionKnee) {
				// Well within gamut
				dist = qMin(value, compressionKnee - value);
			} else if (value > compressionKnee && value <= 1.0) {
				// Near boundary
				dist = 1.0 - value;
			} else {
				// Out of gamut
				dist = qMin(value, 1.0 - value);
				if (dist > 0.0) dist = -dist; // Make negative for out-of-gamut
			}
			
			minDistance = qMin(minDistance, dist);
		}
		
		return minDistance;
	}
};


CPerceptualGamutMapper::CPerceptualGamutMapper(const CPrinterProfile& targetProfile,
                                               double compressionKnee)
	: m_impl(new Impl(targetProfile, compressionKnee))
{
}


CPerceptualGamutMapper::~CPerceptualGamutMapper()
{
	delete m_impl;
}


void CPerceptualGamutMapper::SetCompressionKnee(double knee)
{
	m_impl->compressionKnee = qMax(0.0, qMin(1.0, knee));
}


double CPerceptualGamutMapper::GetCompressionKnee() const
{
	return m_impl->compressionKnee;
}


bool CPerceptualGamutMapper::IsInGamut(const icmm::CVarColor& color) const
{
	return m_impl->IsColorInGamut(color);
}


bool CPerceptualGamutMapper::MapToGamut(const icmm::CVarColor& sourceColor,
                                        icmm::CVarColor& mappedColor) const
{
	return m_impl->PerceptualCompress(sourceColor, mappedColor);
}


double CPerceptualGamutMapper::GetGamutVolume() const
{
	return m_impl->gamutVolume;
}


double CPerceptualGamutMapper::GetGamutCoverage() const
{
	return m_impl->gamutCoverage;
}


double CPerceptualGamutMapper::GetDistanceToGamutBoundary(const icmm::CVarColor& color) const
{
	return m_impl->CalculateDistanceToBoundary(color);
}


} // namespace iimgprint
