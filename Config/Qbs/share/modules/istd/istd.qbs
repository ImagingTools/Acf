import qbs 1.0

Module{
	Depends{ name: 'Qt.core' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/istd.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
