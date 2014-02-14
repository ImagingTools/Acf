import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'icomp' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }

	readonly property path componentFilePath: path + '/../../../.././PackagePck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
