#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QSet>

// ACF includes
#include <istd/IPolymorphic.h>


namespace istd
{


/**
	Base interface providing information about factory-producible objects.
	
	\section FactoryInfoPurpose Purpose
	IFactoryInfo is part of the ACF factory pattern implementation. It provides metadata
	about objects that can be created by factory classes, specifically the keys (identifiers)
	that can be used to instantiate different types of objects through the factory.

	\section FactoryInfoUsage Usage Context
	This interface is typically implemented by factory classes to advertise which object
	types they can create. The keys returned by GetFactoryKeys() serve as identifiers
	that clients can use with the factory's CreateInstance() or similar methods.

	\section FactoryInfoExample Example
	\code
	// Factory interface for creating shapes
	class IShapeFactory: virtual public istd::IFactoryInfo
	{
	public:
		virtual IShape* CreateShape(const QByteArray& shapeType) = 0;
	};

	// Concrete factory implementation
	class CShapeFactory: public IShapeFactory
	{
	public:
		// Reimplemented from IFactoryInfo
		virtual KeyList GetFactoryKeys() const override
		{
			KeyList keys;
			keys.insert("circle");
			keys.insert("rectangle");
			keys.insert("triangle");
			keys.insert("polygon");
			return keys;
		}

		virtual IShape* CreateShape(const QByteArray& shapeType) override
		{
			if (shapeType == "circle") {
				return new CCircle();
			} else if (shapeType == "rectangle") {
				return new CRectangle();
			} else if (shapeType == "triangle") {
				return new CTriangle();
			} else if (shapeType == "polygon") {
				return new CPolygon();
			}
			return nullptr;
		}
	};

	// Usage example
	CShapeFactory factory;

	// Query available shape types
	IFactoryInfo::KeyList availableShapes = factory.GetFactoryKeys();
	for (const QByteArray& shapeType : availableShapes) {
		qDebug() << "Factory can create:" << shapeType;
	}

	// Create shapes using the factory
	if (availableShapes.contains("circle")) {
		IShape* circle = factory.CreateShape("circle");
		// Use the circle...
		delete circle;
	}
	\endcode

	\section FactoryInfoKeyFormat Key Format
	Factory keys are QByteArray values that uniquely identify object types within the factory.
	The format and naming convention for keys is typically defined by the specific factory
	implementation, but common practices include:
	- Using lowercase names (e.g., "widget", "component")
	- Using qualified names for namespaced types (e.g., "gui.button", "model.data")
	- Using class names or type identifiers

	\sa istd::IFactory, istd::TIFactory

	\ingroup Main
*/
class IFactoryInfo: virtual public istd::IPolymorphic
{
public:
	typedef QSet<QByteArray> KeyList;

	/**
		Returns all possible keys for this factory.
		
		Retrieves the complete set of keys (identifiers) that this factory can use
		to create objects. Each key corresponds to a specific object type or variant
		that the factory knows how to instantiate.
		
		\return A QSet containing all valid factory keys. The set may be empty if
		        the factory cannot currently create any objects, but typically contains
		        at least one key for factories that are properly configured.
		
		\note The returned set represents a snapshot of available keys at the time
		      of the call. For dynamic factories, the available keys may change over
		      time (e.g., through plugin loading).
		\note Keys are typically case-sensitive QByteArray values. Clients should
		      use exact string matching when working with factory keys.
		
		\code
		// Example usage
		IFactoryInfo* factory = GetFactory();
		IFactoryInfo::KeyList keys = factory->GetFactoryKeys();
		
		// Check if a specific type is available
		if (keys.contains("mytype")) {
			// Create object using "mytype" key
		}
		
		// List all available types
		for (const QByteArray& key : keys) {
			qDebug() << "Available type:" << key;
		}
		\endcode
		
		\sa istd::IFactory::CreateInstance()
	*/
	virtual KeyList GetFactoryKeys() const = 0;
};


} // namespace istd



