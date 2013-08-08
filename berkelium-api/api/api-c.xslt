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

</xsl:text>

	<xsl:for-each select="/api/mapping[@type='c']/type[@impl]">
		<xsl:text>typedef </xsl:text>
		<xsl:value-of select="@impl"/>
		<xsl:text> </xsl:text>
		<xsl:value-of select="@value"/>
		<xsl:text>;
</xsl:text>
	</xsl:for-each>

	<xsl:text>
</xsl:text>
	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:text>typedef void* BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>;
</xsl:text>
	</xsl:for-each>

	<xsl:text>
</xsl:text>

	<xsl:for-each select="/api/group[@type='enum']">
		<xsl:text>typedef enum {
</xsl:text>

		<xsl:for-each select="entry">
			<xsl:text>	</xsl:text>
			<xsl:value-of select="@name"/>
			<xsl:if test="position() != last()">
				<xsl:text>,</xsl:text>
			</xsl:if>
			<xsl:text>
</xsl:text>
		</xsl:for-each>

		<xsl:text>} BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>;

</xsl:text>
	</xsl:for-each>

	<xsl:for-each select="/api/mapping[@type='c']/type[@list]">
		<xsl:text>typedef struct {
	bk_int32 size;
	BK_</xsl:text>
		<xsl:value-of select="@list"/>
	<xsl:text>* entrys;
} BK_</xsl:text>
		<xsl:value-of select="@list"/>
		<xsl:text>List;

</xsl:text>
	</xsl:for-each>

	<xsl:apply-templates select="group[@type!='enum']"/>
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
	<xsl:text>
// =========================================
// </xsl:text>
	<xsl:value-of select="@type"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:if test="short">
		<xsl:text>
//
// </xsl:text>
		<xsl:value-of select="short"/>
	</xsl:if>
	<xsl:text>
// =========================================
</xsl:text>
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
<!-- Ignore Functions for other languages                          -->
<!-- ============================================================= -->
<xsl:template match="entry"/>

<!-- ============================================================= -->
<!-- Functions for c and all languages                             -->
<!-- ============================================================= -->
<xsl:template match="entry[@type='c']|entry[not(@type)]">
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
	<xsl:value-of select="../@name"/>
	<xsl:text>_</xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:value-of select="@c-suffix"/>
	<xsl:text>(</xsl:text>
	<xsl:call-template name="arguments-self"/>
	<xsl:text>);
</xsl:text>
</xsl:template>

</xsl:stylesheet>
