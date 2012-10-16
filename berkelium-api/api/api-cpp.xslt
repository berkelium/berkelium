<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:variable name="lang">cpp</xsl:variable>

<xsl:output method="text"/>
<xsl:strip-space elements="*"/>

<xsl:include href="include.xslt"/>

<!-- ============================================================= -->
<!-- Header File                                                   -->
<!-- ============================================================= -->
<xsl:template match="/api">
	<xsl:variable name="group" select="group[@name=$class]"/>
	<!-- TODO: not very important, but some underscores are missing ;-) -->
	<xsl:variable name="guard" select="concat('BERKELIUM_', translate($class, $smallcase, $uppercase),'_HPP_')"/>

	<!-- header -->
	<xsl:call-template name="comment-header"/>
	<xsl:text>#ifndef </xsl:text>
	<xsl:value-of select="$guard"/>
	<xsl:text>
#define </xsl:text>
	<xsl:value-of select="$guard"/>
	<xsl:text>
#pragma once

</xsl:text>
	<xsl:call-template name="comment-generated"/>

	<xsl:text>#include &lt;string&gt;

namespace Berkelium {

</xsl:text>

	<!-- forward defs -->
	<xsl:for-each select="/api/group">
		<xsl:choose>
			<xsl:when test="@type='class'">
				<xsl:text>class </xsl:text>
			</xsl:when>
			<xsl:when test="@type='interface'">
				<xsl:text>class </xsl:text>
			</xsl:when>
			<xsl:otherwise>
				!!ERROR!!
			</xsl:otherwise>
		</xsl:choose>
		<xsl:value-of select="@name"/>
		<xsl:text>;
</xsl:text>
	</xsl:for-each>
	<xsl:text>
</xsl:text>

	<!-- class comment -->
	<xsl:if test="$group/short">
		<xsl:text>// </xsl:text>
		<xsl:value-of select="$group/short"/>
		<xsl:text>
</xsl:text>
	</xsl:if>

	<!-- class definition -->
	<xsl:text>public </xsl:text>
	<xsl:choose>
		<xsl:when test="$group/@type='class'">
			<xsl:text>abstract class </xsl:text>
		</xsl:when>
		<xsl:when test="$group/@type='interface'">
			<xsl:text>interface </xsl:text>
		</xsl:when>
		<xsl:otherwise>
			!!ERROR!!
		</xsl:otherwise>
	</xsl:choose>
	<xsl:value-of select="$class"/>
	<xsl:text> {
</xsl:text>

	<!-- class implementation -->
	<xsl:value-of select="$group/implementation[@type=$lang]"/>

	<!-- class members -->
	<xsl:apply-templates select="$group/entry"/>

	<xsl:text>};

} // namespace Berkelium

#endif // </xsl:text>
	<xsl:value-of select="$guard"/>
	<xsl:text>
</xsl:text>

</xsl:template>

<!-- ============================================================= -->
<!-- Class Member                                                  -->
<!-- ============================================================= -->
<xsl:template match="group/entry">
	<!-- TODO -->
</xsl:template>

</xsl:stylesheet>
