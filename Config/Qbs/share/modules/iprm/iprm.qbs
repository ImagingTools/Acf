import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/iprm.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
