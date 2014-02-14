import qbs 1.0

Module{
	Depends{ name: 'imath' }
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/icmm.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
