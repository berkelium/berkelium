<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>
<xsl:strip-space elements="*"/>

<xsl:template match="/">
	<xsl:choose>
		<xsl:when test="$list='cpp'">
			<xsl:call-template name="cpp"/>
		</xsl:when>
		<xsl:when test="$list='java'">
			<xsl:call-template name="java"/>
		</xsl:when>
		<xsl:when test="$list='java-impl'">
			<xsl:call-template name="java-impl"/>
		</xsl:when>
		<xsl:when test="$list='jni'">
			<xsl:call-template name="jni"/>
		</xsl:when>
	</xsl:choose>
</xsl:template>

<xsl:template name="cpp">
	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:text>include/Berkelium/API/</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>.hpp
</xsl:text>
	</xsl:for-each>
</xsl:template>

<xsl:template name="java">
	<xsl:for-each select="/api/group">
		<xsl:value-of select="@name"/>
		<xsl:text>
</xsl:text>
	</xsl:for-each>
</xsl:template>

<xsl:template name="java-impl">
	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:value-of select="@name"/>
		<xsl:text>
</xsl:text>
	</xsl:for-each>
</xsl:template>

<xsl:template name="jni">
	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:text>org/berkelium/impl/</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>Impl.java
</xsl:text>
	</xsl:for-each>
	<xsl:text>org/berkelium/impl/BerkeliumJavaImpl.java
</xsl:text>
</xsl:template>

</xsl:stylesheet>
