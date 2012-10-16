<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:variable name="lang">java</xsl:variable>

<xsl:output method="text"/>
<xsl:strip-space elements="*"/>

<xsl:include href="include.xslt"/>

<!-- ============================================================= -->
<!-- Java File                                                     -->
<!-- ============================================================= -->
<xsl:template match="/api">
	<xsl:variable name="group" select="group[@name=$class]"/>

	<!-- header -->
	<xsl:call-template name="comment-header"/>
	<xsl:text>package org.berkelium.api;

</xsl:text>
	<xsl:call-template name="comment-generated"/>

	<!-- class comment -->
	<xsl:if test="$group/short">
		<xsl:text>// </xsl:text>
		<xsl:value-of select="$group/short"/>
		<xsl:text>
</xsl:text>
	</xsl:if>

	<!-- class definition -->
	<xsl:text>public </xsl:text>
	<xsl:value-of select="$group/@type"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="$class"/>
	<xsl:text> {
</xsl:text>

	<!-- class implementation -->
	<xsl:value-of select="$group/implementation[@type=$lang]"/>

	<!-- class members -->
	<xsl:apply-templates select="$group/entry"/>

	<xsl:text>}
</xsl:text>

</xsl:template>

<!-- ============================================================= -->
<!-- Class Member                                                  -->
<!-- ============================================================= -->
<xsl:template match="group/entry">

	<xsl:text>
</xsl:text>

	<!-- member comment -->
	<xsl:if test="short">
		<xsl:text>	// </xsl:text>
		<xsl:value-of select="short"/>
		<xsl:text>
</xsl:text>
	</xsl:if>

	<!-- member return type -->
	<xsl:text>	</xsl:text>
	<xsl:call-template name="type">
		<xsl:with-param name="name" select="@ret"/>
	</xsl:call-template>

	<!-- member name -->
	<xsl:text> </xsl:text>
	<xsl:value-of select="@name"/>

	<!-- member arguments -->
	<xsl:text>(</xsl:text>
	<xsl:call-template name="arguments"/>
	<xsl:text>);
</xsl:text>

</xsl:template>

</xsl:stylesheet>
