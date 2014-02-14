import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ibase' }
	Depends{ name: 'ifile' }
	Depends{ name: 'ilog' }
	Depends{ name: 'istd' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'icomp' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }

	readonly property path componentFilePath: path + '/../../../.././FilePck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
