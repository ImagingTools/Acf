# Document/View Framework Documentation

This directory contains comprehensive documentation for the ACF Document/View framework, specifically the `idoc` and `iqtdoc` libraries.

## Documentation Organization

### 1. API Reference Documentation (Doxygen)

All interface and component header files in `Include/idoc/` and `Include/iqtdoc/` now contain detailed Doxygen documentation including:

- **Class descriptions** - Comprehensive explanations of purpose, responsibilities, and usage
- **Method documentation** - Detailed parameter and return value descriptions
- **Usage examples** - Code snippets demonstrating common use cases
- **Cross-references** - Links to related classes and concepts
- **Architecture diagrams** - ASCII diagrams showing component relationships

Generate the HTML documentation by running:
```bash
cd Install
doxygen Doxyfile
```

The generated documentation will be in `Docs/html/`.

### 2. Code Examples (DocumentViewExamples.md)

The file `Docs/DocumentViewExamples.md` contains comprehensive, working examples:

- **Example 1**: Simple Single Document Application
  - Complete implementation of a text editor
  - Shows document, view, template, and manager configuration
  - Includes C++ code for custom view component

- **Example 2**: Multi-Document MDI Application
  - Configuration for tabbed/windowed MDI interface
  - Multiple document type support
  - Programmatic document management

- **Example 3**: Document with Undo/Redo Functionality
  - Undo manager configuration and usage
  - State comparison and dirty tracking
  - Implementing undo-aware documents

- **Example 4**: Document with Custom Meta Information
  - Standard metadata usage
  - Defining custom metadata types
  - Multi-page documents with page metadata

- **Example 5**: Help System Integration
  - Doxygen help integration
  - Static HTML help
  - Context-sensitive help implementation
  - Custom help file providers

- **Common Patterns and Best Practices**
  - Document dirty state checking
  - Saving all documents
  - Prompting to save on close

## Key Documented Components

### idoc Library (System-Independent)

**Interfaces:**
- `IDocumentManager` - Central document and view management
- `IDocumentTemplate` - Document and view factory
- `IDocumentMetaInfo` - Document metadata
- `IUndoManager` - Undo/redo functionality
- `ITextDocument` - Simple text document
- `IMultiPageDocument` - Structured multi-page documents
- `IDocumentTypesInfo` - Document type information
- `IDocumentStateComparator` - State comparison
- `IHelpViewer`, `IHelpInfoProvider`, `IHelpFileProvider` - Help system

**Components:**
- `CSingleDocumentTemplateComp` - Single document type template
- `CTextDocumentComp` - Text document implementation
- `CTextFileLoaderComp` - Text file persistence
- `CStandardDocumentMetaInfo` - Metadata implementation
- `CSerializedUndoManagerComp` - Serialization-based undo/redo

### iqtdoc Library (Qt-Specific)

**Components:**
- `CMainWindowGuiComp` - Complete main window with menus
- `CSingleDocumentWorkspaceGuiComp` - SDI workspace
- `CMultiDocumentWorkspaceGuiComp` - MDI workspace
- `CHtmlHelpGuiComp` - HTML help viewer
- `CStaticHelpFileProviderComp` - Static HTML help
- `CDoxygenHelpFileProviderComp` - Doxygen help integration

## Quick Start

### Creating a Simple Text Editor

1. **Define Document Template:**
```cpp
CSingleDocumentTemplateComp {
    DocumentTypeId = "TextDocument"
    DocumentTypeName = "Text Document"
    IsNewSupported = true
    IsEditSupported = true
    
    DocumentFactory = CTextDocumentComp
    ViewFactory = CTextEditorGuiComp
    DocumentLoader = CTextFileLoaderComp
    UndoManager = CSerializedUndoManagerComp
}
```

2. **Create Document Manager:**
```cpp
CSingleDocumentManagerBase {
    Template = TextDocumentTemplate
}
```

3. **Add Main Window:**
```cpp
CMainWindowGuiComp {
    DocumentManager = DocumentManager
    Workspace = CSingleDocumentWorkspaceGuiComp {
        DocumentTemplate = TextDocumentTemplate
    }
}
```

See `DocumentViewExamples.md` for complete, working examples.

## Documentation Standards

All documentation follows these conventions:

1. **Doxygen Style**: Uses `/**` block comments with `\param`, `\return`, `\sa` tags
2. **Examples**: Includes `\par Example` sections with `\code` blocks
3. **Cross-References**: Uses `\sa` to link related classes
4. **Grouping**: Classes tagged with `\ingroup DocumentBasedFramework`
5. **Formatting**: Consistent indentation and structure

## Building Documentation

To generate the complete API documentation:

```bash
# From repository root
cd Install
doxygen Doxyfile

# View documentation
firefox ../Docs/html/index.html
# or
open ../Docs/html/index.html
```

The documentation will include:
- Class hierarchy
- Alphabetical class index
- Namespace documentation
- File documentation
- All code examples
- Cross-referenced API

## Additional Resources

- **ACF Coding Guide**: `Docs/CodingConvention/`
- **Existing Tutorial**: `Docs/Tutorial/Impl/DocView/`
- **Design Documentation**: `Docs/Design/`

## Documentation Coverage

| Category | Items | Status |
|----------|-------|--------|
| idoc Interfaces | 12 files | ✅ Complete |
| idoc Components | 5 key files | ✅ Complete |
| iqtdoc Components | 4 key files | ✅ Complete |
| Namespace Docs | 2 files | ✅ Complete |
| Code Examples | 5 examples | ✅ Complete |
| Best Practices | Multiple patterns | ✅ Complete |

## Contributing

When adding new features to idoc/iqtdoc:

1. Document all public interfaces and methods
2. Include usage examples for complex functionality
3. Add cross-references to related classes
4. Update code examples if behavior changes
5. Follow existing documentation patterns

## Support

For questions about using the Document/View framework:

1. Check the code examples in `DocumentViewExamples.md`
2. Review the generated Doxygen documentation
3. Look at the existing tutorial in `Docs/Tutorial/Impl/DocView/`
4. Consult namespace-level documentation in `idoc.h` and `iqtdoc.h`
