<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>
<xsl:strip-space elements="*"/>

<xsl:template match="/api">
	<xsl:call-template name="c"/>
	<xsl:call-template name="cpp"/>
	<xsl:call-template name="java"/>
</xsl:template>

<xsl:template name="c">
	<xsl:text>#
# C API
#
</xsl:text>
	<xsl:text>/usr/bin/xsltproc -o ../berkelium-c/include/berkelium.h api/api-c.xslt api/api.xml</xsl:text>
	<xsl:text>

</xsl:text>
</xsl:template>

<xsl:template name="cpp">
	<xsl:text>#
# C++ API
#
/usr/bin/xsltproc -o ../berkelium-cpp/src/include/berkelium/Berkelium.hpp api/api-cpp-global.xslt api/api.xml
</xsl:text>
	<xsl:for-each select="/api/group">
		<xsl:text>/usr/bin/xsltproc --stringparam class </xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text> -o ../berkelium-cpp/src/include/berkelium/</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>.hpp api/api-cpp.xslt api/api.xml</xsl:text>
	<xsl:text>
</xsl:text>
	</xsl:for-each>
	<xsl:text>

</xsl:text>
</xsl:template>

<xsl:template name="java">
	<xsl:text>#
# Java API
#
</xsl:text>
	<xsl:for-each select="/api/group">
		<xsl:text>/usr/bin/xsltproc --stringparam class </xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text> -o ../berkelium-java/src/main/java/org/berkelium/api/</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>.java api/api-java.xslt api/api.xml</xsl:text>
	<xsl:text>
</xsl:text>
	</xsl:for-each>
	<xsl:text>

</xsl:text>
</xsl:template>

</xsl:stylesheet>
