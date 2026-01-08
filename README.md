# ACF - Application Component Framework

ACF (Application Component Framework) is a flexible and extensible framework for building component-based applications with support for parameters, serialization, and document management.

## Documentation

The full API documentation is available at: [https://imagingtools.github.io/Acf/](https://imagingtools.github.io/Acf/)

The documentation is automatically generated from the source code using Doxygen and deployed to GitHub Pages.

## Key Features

- **Component Framework**: Flexible component-based architecture
- **Type-Safe Parameter Management**: Strongly typed parameter interfaces
- **Serialization**: Built-in support for parameter persistence
- **Document Management**: Comprehensive document-view framework
- **2D/3D Graphics**: Utilities for 2D and 3D geometric operations
- **Testing**: Qt Test-based unit testing framework

## Libraries

### Core Libraries

- **istd**: Standard utilities and interfaces (change notification, smart pointers, utilities)
- **icomp**: Component framework (component model, registry, dependency injection)
- **iser**: Serialization framework (archive-based object persistence)
- **iprm**: Parameter management (type-safe parameter handling and validation)

### Data and Mathematics

- **imath**: Mathematical utilities (interpolation, complex numbers, numerical algorithms)
- **i2d**: 2D graphics utilities (geometric primitives, transformations, calibration)
- **i3d**: 3D graphics utilities (3D geometric primitives and transformations)
- **icmm**: Color management (color models and conversions)

### Document and View Framework

- **idoc**: Document framework (template-based document management, undo/redo)
- **iview**: View framework (interactive shapes, calibrated views)
- **imod**: Model-observer pattern (model base classes and observers)

### File and Data Handling

- **ifile**: File handling (file loading, auto-persistence, path management)
- **ifilegui**: File GUI components (file dialogs, file list providers)
- **iimg**: Imaging (bitmap handling and image loading)

### Logging and Testing

- **ilog**: Logging (message handling and console logging)
- **iloggui**: Logging GUI components (log display widgets and message boxes)
- **itest**: Testing framework (Qt Test-based unit testing)

### Qt Integration

- **iqt**: Qt utilities (application settings, clipboard, console reader)
- **iqt2d**: Qt 2D graphics (Qt widgets for 2D geometry parameters)
- **iqtdoc**: Qt document framework (document import/export, help file providers)
- **iqtgui**: Qt GUI utilities (application base, designer support, GUI components)
- **iqtprm**: Qt parameter widgets (GUI components for parameter editing)
- **iwidgets**: Widget utilities (custom Qt widgets and delegates)

### Infrastructure

- **iattr**: Attributes (metadata and attribute management)
- **ibase**: Base components (application infrastructure and progress management)
- **ipackage**: Package management (dynamic package and plugin loading)

## Building

See the documentation for detailed build instructions.

## License

See the LGPL directory for license information.
