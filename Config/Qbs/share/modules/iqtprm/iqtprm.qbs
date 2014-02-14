import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imath' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ibase' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'iqt' }
	Depends{ name: 'icomp' }
	Depends{ name: 'i2d' }
	Depends{ name: 'Qt.widgets' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.gui' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/iqtprm.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
