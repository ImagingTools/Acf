import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ibase' }
	Depends{ name: 'ifile' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'icomp' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/ipackage.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
