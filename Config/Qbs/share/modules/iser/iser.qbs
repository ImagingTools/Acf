import qbs 1.0

Module{
	Depends{ name: 'istd' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/iser.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
