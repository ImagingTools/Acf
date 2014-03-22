#ifndef ACF_included
#define ACF_included


/**
\defgroup ACF ACF

\mainpage
\section Introduction
ACF is a C++ based, cross-plattform component framework, that was especially designed for creation of very complex, customizable and highly scalable applications.
ACF follows the component-oriented programming paradigm and provides instruments to build software from reusable components and serves interfaces and concepts to create those components.
ACF contains a set of libraries with implementations of most usefull design patterns, such Serialization, Model/Observer, Factory, Dependency Injection etc..
ACF implements a powerfull component concept and includes many well designed components, which you can directly use in your application.
The UI-part of the framework is based on Qt library provided by Digia.
ACF is open source and released under the terms of the GNU Lesser General Public License (LGPL).
See http://www.ilena.org or write info@imagingtools.de or contact
by Skype to ACF_infoline for further information about the ACF.

More theoretical considerations about the component-based development can be found at the following link:
http://en.wikipedia.org/wiki/Component-based_software_engineering

\section Basics
The main idea behind ACF is to see each software product as a composition of components with clearly defined interfaces. Interface is also the only way for communication between components. The usual steps to implement a component are:

- A C++ interface must be defined.
- A class that implements this interface must be created.There are several ways to make a "normal" C++ class to a ACF-Component, the simplest way is to derive from a corresponding basic implementation or use a generic macro. A component can have dependencies from other components (e.g. component <b>A</b> needs component <b>B</b> to realize some functionality) and can have attributes. Via attributes it is possible, to parameterize some behaviour aspects of the component. Another way to create a component is to combine more existing components into a composition. This composition is also perceived by the framework as a component. A component composition is created with the framework tool called <b>ACF Compositor</b>. ACF Compositor provides graphical editing for the component dependencies and attributes. The compositions are stored in so-called ARX files (we also call them registries).
- After a component is implemented, it must be registered in a so called Package. Component packages are logical containers of components. There are two kinds of packages:
	- For C++ based component the package is a DLL with special export interface.
	- For component compositions the package is a special folder in the file system.

To keep the management component packages consistent, all kind of packages have the same file extension - ARP (ACF Registry Package).

If all the necessary components have been implemented, the application can be executed. There are two basic ways. A composition of components can be run directly by Framework-Runtime.
However, this mechanism is usefull only in the prototyping stage. For the final application a different approach can be used. ACF provides a special source-code generator, which makes it possible to produce source code back from component compositions. The resulting source code is then compiled by the usual way.
This has the advantage that during the runtime of the application no modules (DLLs) must be loaded.

\section Features
Below are some of the main features of ACF:
- Graphical overview and editing of component connections and properties in UML-like manner
- Definition of composite components
- C++ Code Generator (ARX Compiler)
- C++ library containing several implementatons of common business logik components, which can be used in every application
- Component library for abstract data model definition, visualization and processing
- High level components for data binding and persistence
- Multi-Document Interface application framework
- Component library for industrial and scientic image processing
- Component library for 2D and 3D visualization
- Component wrappers for popular 3rd-party libraries such:
	- Qt/Qt Solutions
	- Qwt
	- Qwt3d
	- OpenCV
	- phonon
	- FFMpeg
	- zlib
	- QScintilla
*/
/**
	\defgroup ModelObserver Model/Observer concept
	The main use-case of this concept is to get information on the observer side about changes of model.
	It can be used e.g. to realize GUI update if related data model changes.

	\ingroup ACF
*/

/**
	\defgroup Persistence Persistence concept.

	\ingroup ACF
*/


/**
	\defgroup Helpers Set of helper classes.

	\ingroup ACF
*/


/**
	\defgroup Main Main concepts and implementations
	If you can learn how to use ACF you should begin with elements defined in this module.

	\ingroup ACF
*/


/**
	\defgroup System Defines access to operating system dependent implementation

	\ingroup ACF
*/

/**
	\defgroup ComponentConcept Component concept
	This concept allows to divide application into smaller modules.
	At the implementation level you don't need to care on the application topology.
	The topology will be described separatelly using \em .arx files and can be started using
	runtime environment over ACF tool or compiled to C++ code using ARXC compiler.
	Additionally you can use component simulation to use components as "normal" classes.

	\ingroup ACF
*/


#endif // !ACF_included
