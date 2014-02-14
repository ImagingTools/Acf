import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'icomp' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/ilog.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
