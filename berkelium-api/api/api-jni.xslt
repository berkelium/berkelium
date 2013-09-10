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

#include "berkelium.h"

#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;

#include &lt;sstream&gt;

jclass IllegalArgumentException;

jclass BerkeliumJavaImpl;
jmethodID BerkeliumJavaImpl_mapIn;
jmethodID BerkeliumJavaImpl_mapOut;
jmethodID BerkeliumJavaImpl_mapNew;

jclass BK_Java_Classes[BK_Env_Enum_MAX];
jmethodID BK_Java_Class_ctors[BK_Env_Enum_MAX];
jfieldID BK_Java_Class_IDs[BK_Env_Enum_MAX];

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

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved)
{
	JNIEnv* env;

	if(jvm-&gt;GetEnv((void**)&amp;env, JNI_VERSION_1_1)) {
		return JNI_ERR;
	}

	jclass c = env-&gt;FindClass("org/berkelium/impl/BerkeliumJavaImpl");
	if(c == NULL) {
		return JNI_ERR;
	}
	c = (jclass)env-&gt;NewGlobalRef(c);
	jmethodID mi = env-&gt;GetStaticMethodID(c, "mapIn", "(ILorg/berkelium/impl/BerkeliumObjectImpl;)J");
	if(mi == NULL) {
		return JNI_ERR;
	}
	jmethodID mo = env-&gt;GetStaticMethodID(c, "mapOut", "(IJ)Lorg/berkelium/impl/BerkeliumObjectImpl;");
	if(mo == NULL) {
		return JNI_ERR;
	}
	jmethodID mn = env-&gt;GetStaticMethodID(c, "mapNew", "(IJLorg/berkelium/impl/BerkeliumObjectImpl;)V");
	if(mn == NULL) {
		return JNI_ERR;
	}
	BerkeliumJavaImpl = c;
	BerkeliumJavaImpl_mapIn = mi;
	BerkeliumJavaImpl_mapOut = mo;
	BerkeliumJavaImpl_mapNew = mn;

	c = env-&gt;FindClass("java/lang/IllegalArgumentException");
	if(c == NULL) {
		return JNI_ERR;
	}
	IllegalArgumentException = c;

	for(int i = 0; i &lt; BK_Env_Enum_MAX; i++) {
		c = env-&gt;FindClass(BK_Java_Class_Names[i]);
		if(c == NULL) {
			return JNI_ERR;
		}
		c = (jclass)env-&gt;NewGlobalRef(c);
		BK_Java_Classes[i] = c;
		jmethodID m = env-&gt;GetMethodID(c, "&lt;init&gt;", "()V");
		if(m == NULL) {
			return JNI_ERR;
		}
		BK_Java_Class_ctors[i] = m;
		jfieldID f = env->GetFieldID(c, "id", "I");
		if(f == NULL) {
			return JNI_ERR;
		}
		BK_Java_Class_IDs[i] = f;
	}

	return JNI_VERSION_1_1;
}

// =========================================
//
//	Berkelium to JNI #defines
//
// =========================================

#define BK_TO_JOBJECT(X) (jobject)(X)
#define BK_TO_JBOOLEAN(X) ((X) ? JNI_TRUE : JNI_FALSE)

bk_bk_obj Berkelium_Java_MapIn(BK_Env_Enum type, bk_ext_obj bkJavaId, void* data)
{
	//fprintf(stderr, "Berkelium_Java_MapIn\n");
	JNIEnv* env = (JNIEnv*)data;
	bk_bk_obj ret((bk_bk_obj)env->CallStaticLongMethod(BerkeliumJavaImpl, BerkeliumJavaImpl_mapIn, type, bkJavaId));
	//fprintf(stderr, "MapIn: %p\n", ret);
	return ret;
}

bk_ext_obj Berkelium_Java_MapOut(BK_Env_Enum type, bk_bk_obj bkNativeId, void* data)
{
	//fprintf(stderr, "Berkelium_Java_MapOut\n");
	JNIEnv* env = (JNIEnv*)data;
	return env->CallStaticObjectMethod(BerkeliumJavaImpl, BerkeliumJavaImpl_mapOut, type, bkNativeId);
}

bk_ext_obj Berkelium_Java_MapNew(BK_Env_Enum type, bk_bk_obj bkNativeId, void* data)
{
	//fprintf(stderr, "Berkelium_Java_MapNew\n");
	JNIEnv* env = (JNIEnv*)data;
	if(type &lt; 0 || type &gt;= BK_Env_Enum_MAX) {
		return NULL;
	}
	bk_ext_obj bkJavaId = env->NewObject(BK_Java_Classes[type], BK_Java_Class_ctors[type]);
	env->CallStaticVoidMethod(BerkeliumJavaImpl, BerkeliumJavaImpl_mapNew, type, bkNativeId, bkJavaId);
	return bkJavaId;
}

void Berkelium_Java_MapInError(BK_Env_Enum expected, BK_Env_Enum actual, bk_ext_obj id, void* data)
{
	std::stringstream ss;

	ss &lt;&lt; "Expected " &lt;&lt; BK_Env_Enum_To_String_Or_Err(expected) &lt;&lt; "(" &lt;&lt; expected &lt;&lt; ")";
	ss &lt;&lt; " but got " &lt;&lt; BK_Env_Enum_To_String_Or_Err(actual) &lt;&lt; "(" &lt;&lt; actual &lt;&lt; ")";
	ss &lt;&lt; " for id " &lt;&lt; id;

	std::string str(ss.str());
	fprintf(stderr, "Berkelium_Java_MapInError %s\n", str.c_str());
	JNIEnv* env = (JNIEnv*)data;
	env-&gt;ThrowNew(IllegalArgumentException, str.c_str());
}

inline void setupBkEnv(BK_Env&amp; bkenv, JNIEnv* env)
{
	bkenv.mapIn = Berkelium_Java_MapIn;
	bkenv.mapOut = Berkelium_Java_MapOut;
	bkenv.mapNew = Berkelium_Java_MapNew;
	bkenv.mapInError = Berkelium_Java_MapInError;
	bkenv.data = env;
}

inline jstring BK_TO_JSTRING(JNIEnv* env, char* str)
{
	if(str == NULL) {
		return NULL;
	}
	jstring ret = (jstring)env-&gt;NewGlobalRef(env-&gt;NewStringUTF(str));
	free(str);
	return ret;
}

inline char* JSTRING_TO_BK(JNIEnv* env, jstring str)
{
	if(str == NULL) {
		return NULL;
	}
	jboolean iscopy = false;
	const char* tmp = env-&gt;GetStringUTFChars(str, &amp;iscopy);
	jint len = env-&gt;GetStringUTFLength(str);
	char* ret = (char*)malloc(len);
	memcpy(ret, tmp, len);
	env-&gt;ReleaseStringUTFChars(str, tmp);
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
	<xsl:if test="not(@static='true')">
		<xsl:text>void* self((void*)_this);
	</xsl:text>
	</xsl:if>

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
				<xsl:text>, self</xsl:text>
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
