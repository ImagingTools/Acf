<?xml version="1.0" standalone="no"?>
<xsl:stylesheet xmlns:xsl = "http://www.w3.org/1999/XSL/Transform" version = "1.0" >
	<xsl:template match = "Tool" mode="IncludePath">
		<xsl:call-template name = "ParsePathList">
			<xsl:with-param name = "ToParse" select="@AdditionalIncludeDirectories"/>
			<xsl:with-param name = "Mode" select="'EnvOnly'"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match = "Tool" mode="LinkerPath">
		<xsl:call-template name = "ParsePathList">
			<xsl:with-param name = "ToParse" select="@AdditionalLibraryDirectories"/>
			<xsl:with-param name = "Mode" select="'EnvOnly'"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match = "Tool" mode="LinkerLib">
		<xsl:call-template name = "ParsePathList">
			<xsl:with-param name = "ToParse" select="@AdditionalDependencies"/>
			<xsl:with-param name = "Mode" select="'Lib'"/>
			<xsl:with-param name = "Separator" select="' '"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name = "ParsePathList">
		<!--Three modes are supported: 'Normal', 'EnvOnly' and 'Lib'-->
		<xsl:param name = "ToParse"/>
		<xsl:param name = "Mode" select="'Normal'"/>
		<xsl:param name = "Separator" select="';'"/>

		<xsl:if test="string-length($ToParse)>1">
			<xsl:choose>
				<xsl:when test="contains($ToParse, $Separator)">
					<xsl:variable name = "incString" select="substring-before($ToParse, $Separator)"/>
					<xsl:call-template name = "InsertListValue">
						<xsl:with-param name = "ToInsert" select="$incString"/>
						<xsl:with-param name = "Mode" select="$Mode"/>
					</xsl:call-template>
					<xsl:call-template name = "ParsePathList">
						<xsl:with-param name = "ToParse" select="substring-after($ToParse, $Separator)"/>
						<xsl:with-param name = "Mode" select="$Mode"/>
						<xsl:with-param name = "Separator" select="$Separator"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name = "InsertListValue">
						<xsl:with-param name = "ToInsert" select="$ToParse"/>
						<xsl:with-param name = "Mode" select="$Mode"/>
					</xsl:call-template>
			</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template name = "InsertListValue">
		<xsl:param name = "ToInsert"/>
		<xsl:param name = "Mode" select="'Normal'"/>

		<xsl:variable name = "CorrectedPath">
			<xsl:choose>
				<xsl:when test = "$Mode='Lib'">
					<xsl:choose>
						<xsl:when test="contains($ToInsert, '.lib')">
							<xsl:value-of select="translate(concat('lib', substring-before($ToInsert, '.lib'), '.a'), '()\', '{}/')"/>
						</xsl:when>
						<xsl:when test="contains($ToInsert, '.dll')">
							<xsl:value-of select="translate(concat('lib', substring-before($ToInsert, '.lib'), '.prl'), '()\', '{}/')"/>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="contains($ToInsert, 'VC8')">
							<xsl:value-of select="translate(concat(substring-before($ToInsert, 'VC8'), 'Ecl' ,substring-after($ToInsert, 'VC8')), '()\', '{}/')"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="translate($ToInsert, '()\', '{}/')"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:if test="(string-length($CorrectedPath) > 0) and ($Mode!='EnvOnly' or contains($CorrectedPath, '$'))">
			<xsl:call-template name = "InsertValue">
				<xsl:with-param name = "Path" select="$CorrectedPath"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet> 
