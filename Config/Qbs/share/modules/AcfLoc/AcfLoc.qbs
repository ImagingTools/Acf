import qbs 1.0

Module{
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/AcfLoc.lib'

	cpp.includePaths: [
		path + '/../../../../../Impl'
	]
}
