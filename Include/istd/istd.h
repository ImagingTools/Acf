#ifndef istd_included
#define istd_included


// STL includes
#include <limits>

// Qt includes
#include <QtCore/QtGlobal>


/**
	Standard library.
	Contains the system indenendent primitives and basic implementations.

	\ingroup Main
*/
namespace istd
{
} // namespace istd


static const double I_BIG_EPSILON = 1.0e-8;

/**
	Some very small number.
*/
#define I_EPSILON std::numeric_limits<double>::epsilon()


/**
	Some very big number.
*/
#define I_HUGE_NUMBER 10e20


/**
	Infinite value.
*/
#define I_INFINITY std::numeric_limits<double>::infinity()


#ifndef QT_NO_DEBUG


#define I_IF_DEBUG(instructions) instructions
#define I_CRITICAL() Q_ASSERT(false)


#else // !QT_NO_DEBUG


#define I_CRITICAL()
#define I_IF_DEBUG(instructions)


#endif // !QT_NO_DEBUG


#ifndef NULL
#define NULL 0
#endif // !NULL


#define I_DECLARE_ENUM(Enum, ...)\
	inline static QList<int> Enum##GetValues(){\
		QList<int> values;\
		int vars[] = {0, __VA_ARGS__};\
		int count = (sizeof(vars) / sizeof(int));\
		for(int i = 0; i < count; ++i){\
			if (i > 0){\
				values << vars[i];\
			}\
		}\
		return values;\
	}\
	inline static QByteArray ToString(Enum enumValue){\
		static QByteArray emptyString;\
		QString enumValuesString = #__VA_ARGS__;\
		QStringList values = enumValuesString.split(",");\
		for (int i = 0; i < values.count(); ++i){\
			values[i] = values[i].simplified();\
		}\
		QList<int> enumValues = Enum##GetValues();\
		Q_ASSERT(enumValues.count() == values.count());\
		for (int i = 0; i < enumValues.count(); ++i){\
			if (enumValues[i] == enumValue){\
				return values[i].toUtf8();\
			}\
		}\
		return emptyString;\
	}\
	inline static bool FromString(const QByteArray& enumString, Enum& enumValue){\
		QString enumValuesString = #__VA_ARGS__;\
		QStringList values = enumValuesString.split(",");\
		for (int i = 0; i < values.count(); ++i){\
			values[i] = values[i].simplified();\
		}\
		QList<int> enumValues = Enum##GetValues();\
		Q_ASSERT(enumValues.count() == values.count());\
		for (int i = 0; i < enumValues.count(); ++i){\
			if (values[i].toUtf8() == enumString){\
				enumValue = Enum(enumValues[i]);\
				return true;\
			}\
		}\
		return false;\
	}\


#endif // !istd_included

