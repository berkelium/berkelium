<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:variable name="lang">java</xsl:variable>

<xsl:output method="text"/>
<xsl:strip-space elements="*"/>

<xsl:include href="include.xslt"/>

<!-- ============================================================= -->
<!-- JNI File                                                      -->
<!-- ============================================================= -->
<xsl:template match="/">
	<!-- header -->
	<xsl:call-template name="comment-header"/>

	<xsl:text>#ifndef BERKELIUM_JNI_HPP_
#define BERKELIUM_JNI_HPP_

</xsl:text>

	<xsl:call-template name="comment-generated"/>

	<xsl:text>#ifdef __cplusplus
extern "C" {
#endif

</xsl:text>

	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:text>#include "org_berkelium_impl_</xsl:text>

		<xsl:value-of select="@name"/>

		<xsl:text>Impl.h"
</xsl:text>
	</xsl:for-each>

		<xsl:text>
</xsl:text>

	<xsl:for-each select="/api/group|/api/mapping[@type='java']/type[@import]">
		<xsl:sort select="@name"/>

		<xsl:variable name="name" select="@name"/>

		<xsl:text>inline jobject bk_new_JNI_</xsl:text>

		<xsl:value-of select="$name"/>

		<xsl:text>(JNIEnv* env)
{
	jclass cls = env-&gt;FindClass("org/berkelium/impl/</xsl:text>

		<xsl:value-of select="$name"/>

		<xsl:text>Impl");
	jmethodID ctor = env->GetMethodID(cls, "&lt;init&gt;", "V");
	return env->NewObject(cls, ctor);
}

</xsl:text>
	</xsl:for-each>

	<xsl:text>
#ifdef __cplusplus
}
#endif
#endif // BERKELIUM_JNI_HPP_
</xsl:text>
</xsl:template>

</xsl:stylesheet>
