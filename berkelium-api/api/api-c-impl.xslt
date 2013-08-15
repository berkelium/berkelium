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

//#include &lt;map&gt;

// =========================================
// C / C++ converter functions
// =========================================

//typedef std::pair&lt;bk_ext_obj, BK_Env_Enum&gt; bk_type_pair;
//typedef std::map&lt;bk_ext_obj, BK_Env_Enum&gt; bk_type_map;
//bk_type_map bk_types;

inline char* newString(const std::string&amp; str)
{
	int len = str.length() + 1;
	char* ret = (char*)malloc(len);
	memcpy(ret, str.c_str(), len);
	return ret;
}

inline Berkelium::RectRef mapInRectRef(BK_Env* env, BK_Rect&amp; rect)
{
	// TODO
	return Berkelium::RectRef();
}

</xsl:text>

	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:call-template name="mapIn"/>
		<xsl:call-template name="mapOut"/>
	</xsl:for-each>

	<xsl:for-each select="/api/mapping[@type='c']/type[@list]">
		<xsl:call-template name="mapList"/>
	</xsl:for-each>

	<xsl:text>
// =========================================
// BK_Env_Enum to String
// =========================================

const char* BK_Env_Enum_To_String_Or_Null(BK_Env_Enum type)
{
	switch(type) {
</xsl:text>

	<xsl:for-each select="/api/group[@type!='enum']">
		<xsl:text>		case </xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>:
			return "</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>";

</xsl:text>
	</xsl:for-each>

		<xsl:text>		default:
			return NULL;
	}
}

const char* BK_Env_Enum_To_String_Or_Err(BK_Env_Enum type)
{
	const char* ret = BK_Env_Enum_To_String_Or_Null(type);
	if(ret == NULL) {
		return "Error";
	}
	return ret;
}
</xsl:text>

	<xsl:apply-templates select="/api/group[@type!='enum']"/>
</xsl:template>

<!-- ============================================================= -->
<!-- Group                                                         -->
<!-- ============================================================= -->
<xsl:template match="group[not(@type='enum') and not(@delegate='true')]">
	<xsl:variable name="gen">
		<xsl:apply-templates select="entry"/>
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
	<xsl:choose>
		<xsl:when test="@static">
			<xsl:text>Berkelium::</xsl:text>
			<xsl:value-of select="../@name"/>
			<xsl:text>::</xsl:text>
		</xsl:when>
		<xsl:otherwise>
			<xsl:text>_this-></xsl:text>
		</xsl:otherwise>
	</xsl:choose>
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
				<xsl:text>mapIn</xsl:text>
				<xsl:value-of select="@type"/>
				<xsl:text>Ref(env, </xsl:text>
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
	<xsl:text>(BK_Env* env</xsl:text>
	<xsl:if test="not(@static)">
		<xsl:text>, </xsl:text>
	</xsl:if>
	<xsl:call-template name="arguments-self"/>
	<xsl:text>)
{
</xsl:text>
	<xsl:if test="not(@static)">
		<xsl:text>
	Berkelium::</xsl:text>

		<xsl:value-of select="../@name"/>

		<xsl:text>Ref _this(mapIn</xsl:text>

		<xsl:value-of select="../@name"/>

		<xsl:text>Ref(env, self));

	if(!_this) {
		return</xsl:text>

		<xsl:choose>
			<xsl:when test="not(@ret)">
			</xsl:when>
			<xsl:when test="@ret='int32'">
				<xsl:text> 0</xsl:text>
			</xsl:when>
			<xsl:when test="@ret='int32' or @ret='bool'">
				<xsl:text> false</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text> NULL</xsl:text>
			</xsl:otherwise>
		</xsl:choose>

		<xsl:text>;
	}

</xsl:text>
	</xsl:if>

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
					<xsl:text>mapOut</xsl:text>
					<xsl:value-of select="@ret"/>
					<xsl:text>(env, </xsl:text>
					<xsl:call-template name="invoke"/>
					<xsl:text>);</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>mapOut</xsl:text>
					<xsl:value-of select="@ret"/>
					<xsl:text>Ref(env, </xsl:text>
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
<xsl:template name="mapOut">
	<xsl:text>
inline bk_ext_obj mapOut</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref(BK_Env* env, Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref bk)
{
	BK_Env_Enum type(</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>);

	bk_ext_obj ret = env-&gt;mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref(bk);
		ret = env-&gt;mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut </xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text> bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}
</xsl:text>
</xsl:template>

<xsl:template name="mapIn">
	<xsl:text>
inline Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref mapIn</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn </xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text> ext:%p not found\n", extId);
		return Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref();
	} else if(it->second != type) {
		env-&gt;mapInError(type, it-&gt;second, extId, env-&gt;data);
		return Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref* ret((Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref*)tmp);

	return Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref(*ret);
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
	<xsl:text>List* mapOut</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>(BK_Env* env, Berkelium::</xsl:text>

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
		ret-&gt;entrys[i] = mapOut</xsl:text>
	<xsl:value-of select="@list"/>
	<xsl:text>Ref(env, *it);
	}

	return ret;
}
</xsl:text>
</xsl:template>

</xsl:stylesheet>
