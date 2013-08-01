<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:variable name="lang">jni</xsl:variable>

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
	jmethodID ctor = env->GetMethodID(cls, "&lt;init&gt;", "()V");
	return env->NewObject(cls, ctor);
}

</xsl:text>
	</xsl:for-each>

	<xsl:for-each select="/api/group[not(@type='enum') and not(@jni='false')]">
		<xsl:sort select="@name"/>
		<xsl:variable name="class" select="@name"/>

		<xsl:text>// =========================================
//
//	</xsl:text>

		<xsl:value-of select="$class"/>
		<xsl:text>
//
// =========================================
</xsl:text>

		<xsl:for-each select="entry">
			<xsl:sort select="@name"/>

			<xsl:text>
</xsl:text>

			<xsl:text>JNIEXPORT </xsl:text>

			<xsl:variable name="ret">
				<xsl:call-template name="type">
					<xsl:with-param name="name" select="@ret"/>
					<xsl:with-param name="lang" select="'jni'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:value-of select="$ret"/>

			<xsl:text> JNICALL Java_org_berkelium_impl_</xsl:text>
			<xsl:value-of select="$class"/>
			<xsl:text>Impl_</xsl:text>
			<xsl:value-of select="@name"/>
			<xsl:value-of select="@jni-suffix"/>
			<xsl:text>(JNIEnv* env, jobject _this</xsl:text>

			<xsl:if test="arg">
				<xsl:text>, </xsl:text>
				<xsl:call-template name="arguments">
					<xsl:with-param name="lang" select="'jni'"/>
				</xsl:call-template>
			</xsl:if>

			<xsl:text>)</xsl:text>

			<xsl:text>
{
</xsl:text>

			<xsl:text>	</xsl:text>

			<xsl:choose>
				<xsl:when test="$ret='void'">
					<xsl:text>// return void...</xsl:text>
				</xsl:when>
				<xsl:when test="$ret='jint'">
					<xsl:text>return 0;</xsl:text>
				</xsl:when>
				<xsl:when test="$ret='jboolean'">
					<xsl:text>return false;</xsl:text>
				</xsl:when>
				<xsl:when test="$ret='jstring'">
					<xsl:text>return env->NewStringUTF("");</xsl:text>
				</xsl:when>
				<xsl:when test="$ret='jobject'">
					<xsl:text>return NULL;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>!!ERROR: Return Statement of '</xsl:text>
					<xsl:value-of select="$ret"/>
					<xsl:text>' not known!!</xsl:text>
				</xsl:otherwise>
			</xsl:choose>


			<xsl:text>
}
</xsl:text>
		</xsl:for-each>

		<xsl:text>
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
