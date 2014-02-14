import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imath' }
	Depends{ name: 'imod' }
	Depends{ name: 'ibase' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ifile' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'iqt' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	readonly property path acfBinDirectory: path + '/../../../../.'
}
