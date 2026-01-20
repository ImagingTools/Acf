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
	CPrinterProfile m_targetProfile;
	double m_gamutVolume;
	double m_gamutCoverage;
	
	explicit Impl(const CPrinterProfile& profile)
		: m_targetProfile(profile)
		, m_gamutVolume(1.0)
		, m_gamutCoverage(1.0)
	{
		// Initialize gamut metrics from profile
		m_gamutCoverage = profile.GetGamutCoverage();
		m_gamutVolume = m_gamutCoverage; // Simplified - would calculate actual volume
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
	return m_impl->m_gamutVolume;
}


double CClippingGamutMapper::GetGamutCoverage() const
{
	return m_impl->m_gamutCoverage;
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
	CPrinterProfile m_targetProfile;
	double m_compressionKnee;
	double m_gamutVolume;
	double m_gamutCoverage;
	
	Impl(const CPrinterProfile& profile, double knee)
		: m_targetProfile(profile)
		, m_compressionKnee(knee)
		, m_gamutVolume(1.0)
		, m_gamutCoverage(1.0)
	{
		// Initialize gamut metrics
		m_gamutCoverage = profile.GetGamutCoverage();
		m_gamutVolume = m_gamutCoverage;
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
		
		if (value >= 0.0 && value <= m_compressionKnee) {
			// Within knee - minimal compression (linear pass-through)
			return value;
		}
		else if (value > m_compressionKnee && value <= 1.0) {
			// Between knee and gamut boundary - smooth compression
			// Protect against division by zero when m_compressionKnee is 1.0
			double denominator = 1.0 - m_compressionKnee;
			if (denominator < 0.001) {
				// Knee is at or very close to boundary, no compression needed
				return value;
			}
			
			double normalized = (value - m_compressionKnee) / denominator;
			// Apply ease-out curve
			double compressed = 1.0 - qPow(1.0 - normalized, 2.0);
			return m_compressionKnee + compressed * denominator * 0.95;
		}
		else if (value > 1.0) {
			// Out of gamut - strong compression to fit within
			double excess = value - 1.0;
			// Compress excess using asymptotic function
			double compressedExcess = excess / (1.0 + excess);
			return 1.0 - (1.0 - m_compressionKnee) * 0.05 + compressedExcess * (1.0 - m_compressionKnee) * 0.05;
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
			if (value >= 0.0 && value <= m_compressionKnee) {
				// Well within gamut
				dist = qMin(value, m_compressionKnee - value);
			} else if (value > m_compressionKnee && value <= 1.0) {
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
	m_impl->m_compressionKnee = qMax(0.0, qMin(1.0, knee));
}


double CPerceptualGamutMapper::GetCompressionKnee() const
{
	return m_impl->m_compressionKnee;
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
	return m_impl->m_gamutVolume;
}


double CPerceptualGamutMapper::GetGamutCoverage() const
{
	return m_impl->m_gamutCoverage;
}


double CPerceptualGamutMapper::GetDistanceToGamutBoundary(const icmm::CVarColor& color) const
{
	return m_impl->CalculateDistanceToBoundary(color);
}


} // namespace iimgprint
