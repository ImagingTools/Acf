<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl = "http://www.w3.org/1999/XSL/Transform" version = "1.0" >
	<xsl:output method = "xml" indent="yes" encoding="utf-8"/>
	<xsl:strip-space elements="*"/>

	<xsl:param name = "SourcePath">..</xsl:param>

	<xsl:template match = "*">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match = "VisualStudioProject">
		<projectDescription>
			<name>iqtdoc</name>
			<comment></comment>
			<projects>
			</projects>
			<buildSpec>
				<buildCommand>
					<name>org.eclipse.cdt.managedbuilder.core.genmakebuilder</name>
					<triggers>clean,full,incremental,</triggers>
					<arguments>
						<dictionary>
							<key>?name?</key>
							<value></value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.append_environment</key>
							<value>true</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.buildArguments</key>
							<value></value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.buildCommand</key>
							<value>make</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.buildLocation</key>
							<value>${workspace_loc:/<xsl:value-of select="//VisualStudioProject/@Name"/>/Debug}</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.contents</key>
							<value>org.eclipse.cdt.make.core.activeConfigSettings</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.enableAutoBuild</key>
							<value>false</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.enableCleanBuild</key>
							<value>true</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.enableFullBuild</key>
							<value>true</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.stopOnError</key>
							<value>true</value>
						</dictionary>
						<dictionary>
							<key>org.eclipse.cdt.make.core.useDefaultBuildCmd</key>
							<value>true</value>
						</dictionary>
					</arguments>
				</buildCommand>
				<buildCommand>
					<name>org.eclipse.cdt.managedbuilder.core.ScannerConfigBuilder</name>
					<arguments>
					</arguments>
				</buildCommand>
			</buildSpec>
			<natures>
				<nature>org.eclipse.cdt.core.ccnature</nature>
				<nature>org.eclipse.cdt.managedbuilder.core.ScannerConfigNature</nature>
				<nature>org.eclipse.cdt.managedbuilder.core.managedBuildNature</nature>
				<nature>org.eclipse.cdt.core.cnature</nature>
			</natures>
			<linkedResources>
				<xsl:apply-templates/>
			</linkedResources>
		</projectDescription>
	</xsl:template>

	<xsl:template match = "File">
		<xsl:variable name = "FilePath"><xsl:value-of select = "substring-after(@RelativePath,'..\')"/></xsl:variable>
		<xsl:variable name = "FileName">
			<xsl:choose>
				<xsl:when test = "contains(@RelativePath,'generated\')">
					<xsl:value-of select = "substring-after(@RelativePath,'generated\')"/>
				</xsl:when>
				<xsl:when test = "starts-with(@RelativePath,'..\')">
					<xsl:value-of select = "substring-after(@RelativePath,'..\')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select = "@RelativePath"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name = "GroupName">
			<xsl:choose>
				<xsl:when test = "contains(@RelativePath,'generated\')">
					Generated
				</xsl:when>
				<xsl:when test = "contains(@RelativePath,'.h')">
					Headers
				</xsl:when>
				<xsl:when test = "contains(@RelativePath,'.cpp')">
					Sources
				</xsl:when>
				<xsl:otherwise>
					Garbages
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test = "starts-with(@RelativePath,'..\')">
			<xsl:element name = "link">
				<xsl:element name = "name">
					<xsl:value-of select = "normalize-space($GroupName)"/><xsl:text>/</xsl:text><xsl:value-of select = "$FileName"/>
				</xsl:element>
				<xsl:element name = "type">1</xsl:element>
				<xsl:element name = "locationURI">
					<xsl:value-of select = "$SourcePath"/>
					<xsl:text>/</xsl:text>
					<xsl:value-of select = "translate($FilePath,'\','/')"/>
				</xsl:element>
			</xsl:element>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet> 
