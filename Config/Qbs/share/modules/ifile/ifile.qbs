import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'icomp' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.widgets' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.gui' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/ifile.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
