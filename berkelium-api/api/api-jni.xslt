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

	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:text>#include "org_berkelium_impl_</xsl:text>

		<xsl:value-of select="@name"/>

		<xsl:text>Impl.h"
</xsl:text>
	</xsl:for-each>

	<xsl:text>
#include "berkelium.h"

#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;

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

	<xsl:text>// =========================================
//
//	Berkelium to JNI #defines
//
// =========================================

#define BK_TO_JOBECT(X) (jobject)(X)
#define BK_TO_JBOOLEAN(X) ((X) ? JNI_TRUE : JNI_FALSE)

inline jstring BK_TO_JSTRING(JNIEnv* env, char* str)
{
	if(str == NULL) {
		return NULL;
	}
	jstring ret = (jstring)env->NewGlobalRef(env->NewStringUTF(str));
	free(str);
	return ret;
}

inline char* JSTRING_TO_BK(JNIEnv* env, jstring str)
{
	if(str == NULL) {
		return NULL;
	}
	jboolean iscopy = false;
	const char* tmp = env->GetStringUTFChars(str, &amp;iscopy);
	jint len = env->GetStringUTFLength(str);
	char* ret = (char*)malloc(len);
	memcpy(ret, tmp, len);
	env->ReleaseStringUTFChars(str, tmp);
	return ret;
}

</xsl:text>

	<xsl:for-each select="/api/group[@type='enum']">
		<xsl:text>inline BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text> </xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>_TO_BK(JNIEnv* env, jobject instance)
{
	jclass cls = env-&gt;FindClass("org/berkelium/api/</xsl:text>

		<xsl:value-of select="@name"/>

		<xsl:text>");

	jmethodID ordinal(env-&gt;GetMethodID(cls, "ordinal", "()I"));
	return (BK_</xsl:text>

		<xsl:value-of select="@name"/>

		<xsl:text>)env-&gt;CallIntMethod(instance, ordinal);
}

</xsl:text>
	</xsl:for-each>


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
</xsl:text>

			<xsl:text>	</xsl:text>

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
					<xsl:text>return BK_TO_JOBECT(</xsl:text>
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
			<xsl:text>(</xsl:text>

			<xsl:if test="not(@static='true')">
				<xsl:text>(</xsl:text>
				<xsl:call-template name="type">
					<xsl:with-param name="name" select="$class"/>
					<xsl:with-param name="lang" select="'c'"/>
				</xsl:call-template>
				<xsl:text>)_this</xsl:text>
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
