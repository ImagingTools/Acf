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
	Depends{ name: 'ipackage' }
	Depends{ name: 'i2d' }
	Depends{ name: 'iqt' }
	Depends{ name: 'icomp' }
	Depends{ name: 'icmm' }
	Depends{ name: 'iimg' }
	Depends{ name: 'idoc' }
	Depends{ name: 'iqt2d' }
	Depends{ name: 'iview' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	readonly property path componentFilePath: path + '/../../../.././QtViewPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
