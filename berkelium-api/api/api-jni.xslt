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

	<xsl:call-template name="comment-generated"/>

	<xsl:variable name="all" select="/api/group[@type!='enum']"/>

	<xsl:for-each select="$all">
		<xsl:text>#include "org_berkelium_impl_</xsl:text>

		<xsl:value-of select="@name"/>

		<xsl:text>Impl.h"
</xsl:text>
	</xsl:for-each>

	<xsl:text>#include "org_berkelium_impl_BerkeliumJavaImpl.h"

#include "berkelium-jni.hpp"

const char* BK_Java_Class_Names[] = {
</xsl:text>

	<xsl:for-each select="$all">
		<xsl:text>      "org/berkelium/impl/</xsl:text>

		<xsl:value-of select="@name"/>

		<xsl:text>Impl"</xsl:text>
		<xsl:text>,
</xsl:text>
	</xsl:for-each>

	<xsl:text>};

</xsl:text>

	<xsl:for-each select="/api/group[not(@type='enum') and not(@delegate='true')]">
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

		<xsl:if test="@type='interface'">
			<xsl:text>
JNIEXPORT void JNICALL Java_org_berkelium_impl_</xsl:text>

			<xsl:value-of select="$class"/>

			<xsl:text>Impl_dispose(JNIEnv* env, jobject _this)
{
	BK_Env bkenv;
	setupBkEnv(bkenv, env);
	BK_</xsl:text>

			<xsl:value-of select="$class"/>

			<xsl:text>_free(&amp;bkenv, (void*)_this);
}

</xsl:text>
		</xsl:if>

		<xsl:for-each select="entry[not(@type)]">
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
	BK_Env bkenv;
	setupBkEnv(bkenv, env);
	</xsl:text>

			<xsl:choose>
				<xsl:when test="$ret='void'">
				</xsl:when>
				<xsl:when test="$ret='jint'">
					<xsl:text>return </xsl:text>
				</xsl:when>
				<xsl:when test="$ret='jboolean'">
					<xsl:text>return BK_TO_JBOOLEAN(</xsl:text>
				</xsl:when>
				<xsl:when test="$ret='jstring'">
					<xsl:text>return BK_TO_JSTRING(env, </xsl:text>
				</xsl:when>
				<xsl:when test="$ret='jobject'">
					<xsl:text>return BK_TO_JOBJECT(</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>!!ERROR: Return Statement of '</xsl:text>
					<xsl:value-of select="$ret"/>
					<xsl:text>' not known!!</xsl:text>
				</xsl:otherwise>
			</xsl:choose>

			<xsl:text>BK_</xsl:text>
			<xsl:value-of select="$class"/>
			<xsl:text>_</xsl:text>
			<xsl:value-of select="@name"/>
			<xsl:value-of select="@c-suffix"/>
			<xsl:text>(&amp;bkenv</xsl:text>

			<xsl:if test="not(@static='true')">
				<xsl:text>, (void*)_this</xsl:text>
			</xsl:if>

			<xsl:for-each select="arg">
				<xsl:text>, </xsl:text>
				<xsl:variable name="t">
					<xsl:value-of select="@type"/>
				</xsl:variable>
				<xsl:variable name="type">
					<xsl:call-template name="type">
						<xsl:with-param name="name" select="@type"/>
						<xsl:with-param name="lang" select="'jni'"/>
					</xsl:call-template>
				</xsl:variable>

				<xsl:choose>
					<xsl:when test="/api/group[@type='enum' and @name=$t]">
						<xsl:value-of select="$t"/>
						<xsl:text>_TO_BK(env, </xsl:text>
						<xsl:value-of select="@name"/>
						<xsl:text>)</xsl:text>
					</xsl:when>
					<xsl:when test="$type='jobject'">
						<xsl:text>(</xsl:text>
						<xsl:call-template name="type">
							<xsl:with-param name="name" select="@type"/>
							<xsl:with-param name="lang" select="'c'"/>
						</xsl:call-template>
						<xsl:text>)</xsl:text>
						<xsl:value-of select="@name"/>
					</xsl:when>
					<xsl:when test="$type='jstring'">
						<xsl:text>JSTRING_TO_BK(env, </xsl:text>
						<xsl:value-of select="@name"/>
						<xsl:text>)</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="@name"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>

			<xsl:text>)</xsl:text>

			<xsl:if test="$ret!='void' and $ret!='jint'">
				<xsl:text>)</xsl:text>
			</xsl:if>

			<xsl:text>;
</xsl:text>

			<xsl:text>}
</xsl:text>
		</xsl:for-each>

		<xsl:text>
</xsl:text>

	</xsl:for-each>

</xsl:template>

</xsl:stylesheet>
