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
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.widgets' }

	cpp.staticLibraries: path + '/../../../../../Bin/" + acf.compilerDir + "/iloggui.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
