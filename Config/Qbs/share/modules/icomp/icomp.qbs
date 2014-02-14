import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/icomp.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
