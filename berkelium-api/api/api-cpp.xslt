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

	<!-- header -->
	<xsl:call-template name="comment-header"/>
	<xsl:call-template name="include-guard-start"/>
	<xsl:call-template name="comment-generated"/>

	<xsl:text>#include "berkelium/berkelium.hpp"

namespace Berkelium {

</xsl:text>

	<!-- class comment -->
	<xsl:if test="$group/short">
		<xsl:text>// </xsl:text>
		<xsl:value-of select="$group/short"/>
		<xsl:text>
</xsl:text>
	</xsl:if>

	<!-- class definition -->
	<xsl:text>class </xsl:text>
	<xsl:value-of select="$class"/>
	<xsl:text> {
</xsl:text>

	<!-- interface -->
	<xsl:if test="$group/@type='interface'">
		<xsl:text>protected:
	inline </xsl:text>
		<xsl:value-of select="$class"/>
		<xsl:text>() {}
	inline virtual ~</xsl:text>
		<xsl:value-of select="$class"/>
		<xsl:text>() {}

</xsl:text>

	</xsl:if>

	<!-- class implementation -->
	<xsl:value-of select="$group/implementation[@type=$lang]"/>

	<xsl:text>public:</xsl:text>

	<!-- class members -->
	<xsl:apply-templates select="$group/entry"/>

	<xsl:text>};

} // namespace Berkelium

</xsl:text>
	<xsl:call-template name="include-guard-end"/>
</xsl:template>

<!-- ============================================================= -->
<!-- Class Member                                                  -->
<!-- ============================================================= -->
<xsl:template match="group/entry">

	<xsl:if test="short">
		<xsl:text>
	// </xsl:text>
		<xsl:value-of select="short"/>
		<xsl:text>
</xsl:text>
	</xsl:if>

	<xsl:text>	</xsl:text>
	<xsl:choose>
		<xsl:when test="@static='true'">
			<xsl:text>static </xsl:text>
		</xsl:when>
		<xsl:otherwise>
			<xsl:text>virtual </xsl:text>
		</xsl:otherwise>
	</xsl:choose>
	<xsl:call-template name="type">
		<xsl:with-param name="name" select="@ret"/>
	</xsl:call-template>
	<xsl:text> </xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:text>(</xsl:text>
	<xsl:call-template name="arguments"/>
	<xsl:text>);
</xsl:text>
</xsl:template>

</xsl:stylesheet>
