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
	<xsl:text>package org.berkelium.impl;

</xsl:text>

	<xsl:for-each select="/api/group|/api/mapping[@type='java']/type[@import]">
		<xsl:sort select="@name"/>

		<xsl:variable name="name" select="@name"/>
		<xsl:variable name="entry" select="/api/group[@name=$class]/entry"/>

		<xsl:if test="$name=$class or count($entry[@ret=$name]|$entry/arg[@type=$name]) &gt; 0">
			<xsl:text>import org.berkelium.api.</xsl:text>

			<xsl:value-of select="$name"/>

			<xsl:text>;
</xsl:text>
		</xsl:if>
	</xsl:for-each>

	<xsl:text>
</xsl:text>

	<xsl:call-template name="comment-generated"/>

	<!-- class definition -->
	<xsl:text>public class </xsl:text>
	<xsl:value-of select="$class"/>
	<xsl:text>Impl </xsl:text>
	<xsl:choose>
		<xsl:when test="$group/@type='class'">
			<xsl:text>extends </xsl:text>
			<xsl:value-of select="$class"/>
			<xsl:text> implements BerkeliumObjectImpl</xsl:text>
		</xsl:when>
		<xsl:when test="$group/@type='interface'">
			<xsl:text>implements </xsl:text>
			<xsl:value-of select="$class"/>
			<xsl:text>, BerkeliumObjectImpl</xsl:text>
		</xsl:when>
		<xsl:otherwise>
			!!ERROR!!
		</xsl:otherwise>
	</xsl:choose>
	<xsl:text> {

	@Override
	public native void dispose();
</xsl:text>

	<!-- class implementation -->
	<xsl:value-of select="$group/internal[@type=$lang]"/>

	<!-- class members -->
	<xsl:apply-templates select="$group/entry" mode="class"/>

	<xsl:text>}
</xsl:text>

</xsl:template>

<!-- ============================================================= -->
<!-- Class Member                                                  -->
<!-- ============================================================= -->
<xsl:template match="group/entry" mode="class">

	<xsl:text>
	@Override
	public native </xsl:text>

	<!-- member return type -->
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
