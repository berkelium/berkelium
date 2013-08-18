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

typedef enum {
</xsl:text>
	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:text>	</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:if test="position() != last()">
			<xsl:text>,</xsl:text>
		</xsl:if>
		<xsl:text>
</xsl:text>
	</xsl:for-each>
	<xsl:text>} BK_Env_Enum;

const char* BK_Env_Enum_To_String_Or_Null(BK_Env_Enum type);
const char* BK_Env_Enum_To_String_Or_Err(BK_Env_Enum type);

#define BK_Env_Enum_MAX </xsl:text>
	<xsl:value-of select="count(/api/group[@type!='enum'])"/>
	<xsl:text>

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

	<xsl:text>typedef void* bk_bk_obj;
typedef void* bk_ext_obj;

typedef bk_bk_obj BK_Env_mapIn(BK_Env_Enum type, bk_ext_obj id, void* data);

typedef bk_ext_obj BK_Env_mapOut(BK_Env_Enum type, bk_bk_obj id, void* data);

typedef bk_ext_obj BK_Env_mapNew(BK_Env_Enum type, bk_bk_obj id, void* data);

typedef void BK_Env_mapInError(BK_Env_Enum expected, BK_Env_Enum actual, bk_ext_obj id, void* data);

typedef struct {
	BK_Env_mapIn* mapIn;
	BK_Env_mapOut* mapOut;
	BK_Env_mapNew* mapNew;
	BK_Env_mapInError* mapInError;
	void* data;
} BK_Env;

</xsl:text>

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

	<xsl:if test="@type='interface' and not(@delegate)">
		<xsl:text>void BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>_free(BK_Env*, BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text> self);
</xsl:text>
	</xsl:if>
</xsl:template>

<!-- ============================================================= -->
<!-- Arguments                                                     -->
<!-- ============================================================= -->
<xsl:template name="arguments-self">
	<xsl:if test="not(@static='true')">
		<xsl:text>, BK_</xsl:text>
		<xsl:value-of select="../@name"/>
		<xsl:text> self</xsl:text>
		<xsl:if test="arg">
			<xsl:text>, </xsl:text>
		</xsl:if>
	</xsl:if>
	<xsl:if test="@static and arg">
		<xsl:text>, </xsl:text>
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
	<xsl:text>(BK_Env* env</xsl:text>
	<xsl:call-template name="arguments-self"/>
	<xsl:text>);
</xsl:text>
</xsl:template>

</xsl:stylesheet>
