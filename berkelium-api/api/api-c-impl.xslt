<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:variable name="lang">c</xsl:variable>

<xsl:output method="text"/>
<xsl:strip-space elements="*"/>

<xsl:include href="include.xslt"/>

<xsl:template match="/">

	<xsl:call-template name="comment-header"/>

	<xsl:call-template name="comment-generated"/>

	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:text>#include &lt;Berkelium/API/</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>.hpp&gt;
</xsl:text>
	</xsl:for-each>

	<xsl:text>
#include "berkelium.h"

#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;

// =========================================
// C / C++ converter functions
// =========================================

inline char* newString(const std::string&amp; str)
{
	int len = str.length();
	char* ret = (char*)malloc(len);
	memcpy(ret, str.c_str(), len);
	return ret;
}
</xsl:text>

	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:call-template name="mapId2Bk"/>
		<xsl:call-template name="mapBk2Id"/>
	</xsl:for-each>

	<xsl:for-each select="/api/mapping[@type=$lang]/type[@map]">
		<xsl:call-template name="mapId2Bk"/>
		<xsl:call-template name="mapBk2Id"/>
	</xsl:for-each>

	<xsl:for-each select="/api/mapping[@type='c']/type[@list]">
		<xsl:call-template name="mapList"/>
	</xsl:for-each>

	<xsl:apply-templates select="/api/group[@type!='enum']"/>
</xsl:template>

<!-- ============================================================= -->
<!-- Group                                                         -->
<!-- ============================================================= -->
<xsl:template match="group[not(@type='enum') and not(@delegate='true')]">
	<xsl:variable name="gen">
		<xsl:apply-templates select="entry[not(@static)]"/>
	</xsl:variable>

	<xsl:if test="$gen!=''">
		<xsl:text>
// =========================================
// </xsl:text>
		<xsl:value-of select="@type"/>
		<xsl:text> </xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>
// =========================================
</xsl:text>
		<xsl:value-of select="$gen"/>
	</xsl:if>
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
<!-- Skip doc                                                      -->
<!-- ============================================================= -->
<xsl:template match="group/*|entry/*">
</xsl:template>

<!-- ============================================================= -->
<!-- Skip doc                                                      -->
<!-- ============================================================= -->
<xsl:template name="invoke">
	<xsl:text>_this-></xsl:text>
	<xsl:value-of select="@name"/>

	<xsl:text>(</xsl:text>

	<xsl:for-each select="arg">
		<xsl:variable name="type">
			<xsl:value-of select="@type"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="@type='int32' or @type='bool' or @type='string'">
				<xsl:value-of select="@name"/>
			</xsl:when>
			<xsl:when test="/api/group[@type='enum' and @name=$type]">
				<xsl:text>(Berkelium::</xsl:text>
				<xsl:value-of select="@type"/>
				<xsl:text>)</xsl:text>
				<xsl:value-of select="@name"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>mapId2</xsl:text>
				<xsl:value-of select="@type"/>
				<xsl:text>Ref(</xsl:text>
				<xsl:value-of select="@name"/>
				<xsl:text>)</xsl:text>
			</xsl:otherwise>
		</xsl:choose>

		<xsl:if test="position() != last()">
			<xsl:text>, </xsl:text>
		</xsl:if>
	</xsl:for-each>

	<xsl:text>)</xsl:text>
</xsl:template>

<!-- ============================================================= -->
<!-- Functions for c and all languages                             -->
<!-- ============================================================= -->
<xsl:template match="entry[@type='c']|entry[not(@type)]">
	<xsl:variable name="ret">
		<xsl:value-of select="@ret"/>
	</xsl:variable>

	<xsl:text>
</xsl:text>
	<xsl:text>extern "C" </xsl:text>
	<xsl:call-template name="type">
		<xsl:with-param name="name" select="@ret"/>
	</xsl:call-template>
	<xsl:text> BK_</xsl:text>
	<xsl:value-of select="../@name"/>
	<xsl:text>_</xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:value-of select="@c-suffix"/>
	<xsl:text>(BK_Env* env, </xsl:text>
	<xsl:call-template name="arguments-self"/>
	<xsl:text>)
{
	Berkelium::</xsl:text>

	<xsl:value-of select="../@name"/>

	<xsl:text>Ref _this(mapId2</xsl:text>

	<xsl:value-of select="../@name"/>

	<xsl:text>Ref(self));
</xsl:text>

	<xsl:choose>
		<xsl:when test="not(@ret)">
			<xsl:text>	</xsl:text>
			<xsl:call-template name="invoke"/>
			<xsl:text>;</xsl:text>
		</xsl:when>
		<xsl:otherwise>
			<xsl:text>	return </xsl:text>

			<xsl:choose>
				<xsl:when test="@ret='int32' or @ret='bool'">
					<xsl:call-template name="invoke"/>
					<xsl:text>;</xsl:text>
				</xsl:when>
				<xsl:when test="@ret='string'">
					<xsl:text>newString(</xsl:text>
					<xsl:call-template name="invoke"/>
					<xsl:text>);</xsl:text>
				</xsl:when>
				<xsl:when test="/api/mapping[@type='c']/type[@name=$ret]">
					<xsl:text>map</xsl:text>
					<xsl:value-of select="@ret"/>
					<xsl:text>2Id(</xsl:text>
					<xsl:call-template name="invoke"/>
					<xsl:text>);</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>map</xsl:text>
					<xsl:value-of select="@ret"/>
					<xsl:text>Ref2Id(</xsl:text>
					<xsl:call-template name="invoke"/>
					<xsl:text>);</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:otherwise>
	</xsl:choose>

	<xsl:text>
}
</xsl:text>
</xsl:template>

<!-- ============================================================= -->
<!-- Mappings                                                      -->
<!-- ============================================================= -->
<xsl:template name="mapBk2Id">
	<xsl:text>
inline void* map</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref2Id(Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref bk)
{
	return NULL;
}
</xsl:text>
</xsl:template>

<xsl:template name="mapId2Bk">
	<xsl:text>
inline Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref mapId2</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref(void* id)
{
	return Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref();
}
</xsl:text>
</xsl:template>

<xsl:template name="mapList">
	<xsl:variable name="name">
		<xsl:text>BK_</xsl:text>
		<xsl:value-of select="@list"/>
	</xsl:variable>

	<xsl:text>
inline </xsl:text>
	<xsl:value-of select="$name"/>
	<xsl:text>List* map</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>2Id(Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref list)
{
	</xsl:text>
	<xsl:value-of select="$name"/>
	<xsl:text>List* ret = (</xsl:text>
	<xsl:value-of select="$name"/>
	<xsl:text>List*)malloc(sizeof(</xsl:text>
	<xsl:value-of select="$name"/>
	<xsl:text>List));

	ret->size = list-&gt;size();
	ret->entrys = (</xsl:text>
	<xsl:value-of select="$name"/>
	<xsl:text>*)malloc(sizeof(</xsl:text>
	<xsl:value-of select="$name"/>
	<xsl:text>) * ret-&gt;size);

	int i = 0;
	for(Berkelium::</xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:text>::iterator it(list-&gt;begin()); it != list-&gt;end(); i++, it++) {
		ret-&gt;entrys[i] = map</xsl:text>
	<xsl:value-of select="@list"/>
	<xsl:text>Ref2Id(*it);
	}

	return ret;
}
</xsl:text>
</xsl:template>

</xsl:stylesheet>
