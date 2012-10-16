<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:variable name="lang">c</xsl:variable>

<xsl:output method="text"/>
<xsl:strip-space elements="*"/>

<xsl:include href="include.xslt"/>

<xsl:template match="/api">

	<xsl:call-template name="comment-header"/>

	<xsl:text>#ifndef BERKELIUM_API_H_
#define BERKELIUM_API_H_
#pragma once

</xsl:text>

	<xsl:call-template name="comment-generated"/>

	<xsl:text>#include &lt;stdint.h&gt;

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t bk_int32;
typedef int32_t bk_bool;
typedef const char* bk_string;

</xsl:text>

	<xsl:for-each select="/api/group[@type='class' or @type='interface']">
		<xsl:text>typedef bk_int32 BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>;
</xsl:text>
	</xsl:for-each>

	<xsl:apply-templates select="group"/>
	<xsl:text>
#ifdef __cplusplus
}
#endif

#endif // BERKELIUM_API_H_
</xsl:text>
</xsl:template>

<!-- ============================================================= -->
<!-- Group                                                         -->
<!-- ============================================================= -->
<xsl:template match="group">
// =========================================
// <xsl:value-of select="@type"/><xsl:text> </xsl:text><xsl:value-of select="@name"/>
<xsl:if test="short">
//
// <xsl:value-of select="short"/>
</xsl:if>
// =========================================
<xsl:apply-templates select="entry"/>
</xsl:template>

<!-- ============================================================= -->
<!-- Arguments                                                     -->
<!-- ============================================================= -->
<xsl:template name="arguments-self">
	<xsl:if test="not(@static='true')">
		<xsl:text>BK_</xsl:text>
		<xsl:value-of select="../@name"/>
		<xsl:text> self</xsl:text>
		<xsl:if test="arg">
			<xsl:text>, </xsl:text>
		</xsl:if>
	</xsl:if>
	<xsl:call-template name="arguments"/>
</xsl:template>

<!-- ============================================================= -->
<!-- Functions                                                     -->
<!-- ============================================================= -->
<xsl:template match="entry" >
<xsl:if test="short">
	<xsl:text>
</xsl:text>
	<xsl:text>// </xsl:text>
	<xsl:value-of select="short"/>
	<xsl:text>
</xsl:text>
</xsl:if>
	<xsl:call-template name="type">
		<xsl:with-param name="name" select="@ret"/>
	</xsl:call-template>
	<xsl:text> BK_</xsl:text>
	<xsl:value-of select="../@name"/>_<xsl:value-of select="@name"/>(<xsl:call-template name="arguments-self"/>);
</xsl:template>

</xsl:stylesheet>
