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
#include &lt;berkelium.h&gt;

#include &lt;Berkelium/Impl/Impl.hpp&gt;
#include &lt;Berkelium/Impl/Manager.hpp&gt;

#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;

using Berkelium::RectRef;
</xsl:text>

<xsl:for-each select="/api/group[@type='interface']">
	<xsl:text>using Berkelium::</xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:text>Ref;
</xsl:text>
</xsl:for-each>

<xsl:for-each select="/api/group[@type='enum']">
	<xsl:text>using Berkelium::</xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:text>;
</xsl:text>
</xsl:for-each>

	<xsl:for-each select="/api/group[@delegate]">
		<xsl:text>
class Bk</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>Mapper : public Berkelium::</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>
{
private:
	BK_Env* env;
	BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text> delegate;
public:
	Bk</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>Mapper(BK_Env* env, BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text> delegate)
		: Berkelium::</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>(),
		env(BK_Env_clone(env)),
		delegate(delegate) {
	}

	virtual ~Bk</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>Mapper() {
		free(delegate);
		free(env);
	}</xsl:text>

	<xsl:for-each select="entry">
		<xsl:text>

	virtual </xsl:text>
		<xsl:call-template name="type">
			<xsl:with-param name="name" select="@ret"/>
			<xsl:with-param name="ret" select="'true'"/>
			<xsl:with-param name="lang" select="'cpp'"/>
		</xsl:call-template>
		<xsl:text> </xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>(</xsl:text>
		<xsl:call-template name="arguments">
			<xsl:with-param name="lang" select="'cpp'"/>
		</xsl:call-template>
		<xsl:text>);</xsl:text>
	</xsl:for-each>
	<xsl:text>
};

</xsl:text>
	</xsl:for-each>
<xsl:text>

#include "BerkeliumC.hpp"

// =========================================
// C / C++ converter functions
// =========================================

</xsl:text>

	<xsl:for-each select="/api/group[@delegate]">
		<xsl:text>

inline Berkelium::</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>Ref mapIn</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>Ref(BK_Env* env, bk_ext_obj extId)
{
	BERKELIUM_C_TRACE_STATIC();

	BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text> delegate = (BK_</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>)env->mapIn(</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>, extId, env);
	if(delegate == NULL) {
		bk_error("error: '%s' returned NULL!", __FUNCTION__);
		return Berkelium::</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>Ref();
	}

	BERKELIUM_C_TRACE_RETURN(delegate);

	return Berkelium::</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>Ref(new Bk</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>Mapper(env, delegate));
}
</xsl:text>
	</xsl:for-each>

	<xsl:for-each select="/api/group[not(@delegate) and @type='interface']">
		<xsl:call-template name="mapIn"/>

		<xsl:variable name="name">
			<xsl:value-of select="@name"/>
		</xsl:variable>

		<xsl:if test="/api/group/entry[@ret=$name]">
			<xsl:call-template name="mapOut"/>
		</xsl:if>
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

	<xsl:for-each select="/api/group[@type='interface']">
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

	<xsl:apply-templates select="/api/group[not(@delegate) and @type!='enum']"/>

	<xsl:for-each select="/api/group[@delegate]">
		<xsl:variable name="name" select="@name"/>
		<xsl:for-each select="entry">
			<xsl:text>
</xsl:text>
			<xsl:call-template name="type">
				<xsl:with-param name="name" select="@ret"/>
				<xsl:with-param name="ret" select="'true'"/>
				<xsl:with-param name="lang" select="'cpp'"/>
			</xsl:call-template>
			<xsl:text> Bk</xsl:text>
			<xsl:value-of select="$name"/>
			<xsl:text>Mapper::</xsl:text>
			<xsl:value-of select="@name"/>
			<xsl:text>(</xsl:text>
			<xsl:call-template name="arguments">
				<xsl:with-param name="lang" select="'cpp'"/>
			</xsl:call-template>
			<xsl:text>)
{
	delegate-></xsl:text>
			<xsl:value-of select="@name"/>
			<xsl:text>(env, delegate</xsl:text>

			<xsl:for-each select="arg">
				<xsl:variable name="type" select="@type"/>
				<xsl:text>, </xsl:text>
				<xsl:choose>
					<xsl:when test="@type='string'">
						<xsl:text>newString(</xsl:text>
						<xsl:value-of select="@name"/>
						<xsl:text>)</xsl:text>
					</xsl:when>
					<xsl:when test="/api/group[@name = $type and @type='enum']">
						<xsl:text>(BK_</xsl:text>
						<xsl:value-of select="$type"/>
						<xsl:text>)0/*TODO*/</xsl:text>
<!--
						<xsl:text>map</xsl:text>
						<xsl:value-of select="$type"/>
						<xsl:text>(</xsl:text>
						<xsl:value-of select="@name"/>
						<xsl:text>)</xsl:text>
-->
					</xsl:when>
					<xsl:when test="/api/group[@name = $type] or @type='Rect'">
						<xsl:text>mapOut</xsl:text>
						<xsl:value-of select="$type"/>
						<xsl:text>Ref(env, </xsl:text>
						<xsl:value-of select="@name"/>
						<xsl:text>)</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="@name"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>


			<xsl:text>);
}
</xsl:text>
		</xsl:for-each>
	</xsl:for-each>
</xsl:template>

<!-- ============================================================= -->
<!-- Group                                                         -->
<!-- ============================================================= -->
<xsl:template match="group[not(@type='enum') and not(@delegate='true')]">
	<xsl:variable name="gen">
		<xsl:apply-templates select="entry"/>

		<xsl:if test="@type='interface' and not(@delegate)">
			<xsl:text>
extern "C" void BK_</xsl:text>
			<xsl:value-of select="@name"/>
			<xsl:text>_free(BK_Env* env, BK_</xsl:text>
			<xsl:value-of select="@name"/>
			<xsl:text> self)
{
</xsl:text>
	<xsl:call-template name="defaultEnv"/>
<xsl:text>
	Berkelium::</xsl:text>

			<xsl:value-of select="@name"/>

			<xsl:text>Ref _this(mapIn</xsl:text>

			<xsl:value-of select="@name"/>

			<xsl:text>Ref(env, self));
	if(!_this) {
		bk_error("already freed </xsl:text>

			<xsl:value-of select="@name"/>

			<xsl:text> %p!", self);
		return;
	}
	env->free(_this.get(), env);

	Berkelium::impl::ManagerRef manager(Berkelium::impl::getManager(_this));
	if(!manager) {
		bk_error("can't find manager for </xsl:text>

			<xsl:value-of select="@name"/>

			<xsl:text> %p!", self);
		return;
	}
	void* result = manager->unlock(_this.get());

	if(result == NULL) {
		bk_error("can't free </xsl:text>

			<xsl:value-of select="@name"/>

			<xsl:text> %p!", _this.get());
	} else {
		delete (Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref*)result;
	}

</xsl:text>
			<xsl:if test="@name='Runtime'">
				<xsl:text>	manager.reset();
</xsl:text>
			</xsl:if>
			<xsl:text>}
</xsl:text>
		</xsl:if>
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
<!-- Use Default Env                                               -->
<!-- ============================================================= -->
<xsl:template name="defaultEnv">
	<xsl:text>	if(env == NULL) {
		env = &amp;simpleBerkeliumEnv::env;
	}
</xsl:text>
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
	<xsl:if test="not(@static) or arg">
		<xsl:text>, </xsl:text>
	</xsl:if>
	<xsl:call-template name="arguments-self"/>
	<xsl:text>)
{
	BERKELIUM_C_TRACE</xsl:text>

	<xsl:if test="@static">
		<xsl:text>_STATIC</xsl:text>
	</xsl:if>

	<xsl:text>();
</xsl:text>

	<xsl:call-template name="defaultEnv"/>

	<xsl:if test="arg/@type='string'">

	<xsl:text>
	// NPE check
</xsl:text>
	<xsl:variable name="return">
		<xsl:call-template name="returnDefault"/>
	</xsl:variable>

	<xsl:for-each select="arg[@type='string']">
		<xsl:text>	if(</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text> == NULL</xsl:text>
		<xsl:text>) {
		env->NPE(strdup(__FUNCTION__), strdup("</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>"));
		</xsl:text>
		<xsl:value-of select="$return"/>
		<xsl:text>
	}
</xsl:text>
	</xsl:for-each>

	</xsl:if>

	<xsl:text>
</xsl:text>

	<xsl:if test="not(@static)">
		<xsl:text>	Berkelium::</xsl:text>

		<xsl:value-of select="../@name"/>

		<xsl:text>Ref _this(mapIn</xsl:text>

		<xsl:value-of select="../@name"/>

		<xsl:text>Ref(env, self));

	if(!_this) {
		bk_error("error: _this in '%s' %p not found!", __FUNCTION__, self);
		</xsl:text>

		<xsl:call-template name="returnDefault"/>

		<xsl:text>
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
	BERKELIUM_C_TRACE_STATIC();

	Berkelium::impl::ManagerRef manager(Berkelium::impl::getManager(bk));

	if(!manager) {
		bk_error("error: manager in '%s' %p not found!", __FUNCTION__, bk.get());
		return NULL;
	}

	if(!manager->locked(bk.get())) {
		manager->lock(bk.get(), new Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>Ref(bk));
	}
</xsl:text>

	<xsl:text>

	BK_Env_Enum type(</xsl:text>

	<xsl:value-of select="@name"/>

	<xsl:text>);

	bk_ext_obj ret = env-&gt;mapOut(type, bk.get(), env);

	if(ret == NULL) {
		ret = env-&gt;mapNew(type, bk.get(), NULL, env);
	}

	if(ret == NULL) {
		bk_error("error: '%s' returned NULL!", __FUNCTION__);
	}

	BERKELIUM_C_TRACE_RETURN(ret);

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
	BERKELIUM_C_TRACE();

	</xsl:text>

	<xsl:choose>
		<xsl:when test="@delegate">

			<xsl:text>BK_Env_Enum type(</xsl:text>

			<xsl:value-of select="@name"/>

			<xsl:text>);

	void* tmp = env->mapIn(type, extId, env);

	BERKELIUM_C_TRACE_RETURN(tmp);

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
		</xsl:when>
		<xsl:otherwise>
			<xsl:text>Berkelium::</xsl:text>
			<xsl:value-of select="@name"/>
			<xsl:text>* intId = (Berkelium::</xsl:text>
			<xsl:value-of select="@name"/>
			<xsl:text>*)env->mapIn(</xsl:text>
			<xsl:value-of select="@name"/>

			<xsl:text>, extId, env);
	Berkelium::impl::ManagerRef manager(Berkelium::impl::getManager(intId));

	if(!manager) {
		bk_error("error: manager in '%s' %p not found!", __FUNCTION__, intId);
		return Berkelium::</xsl:text>

		<xsl:value-of select="@name"/>

		<xsl:text>Ref();
	}

	Berkelium::</xsl:text>

	<xsl:value-of select="@name"/>
	<xsl:text>Ref ret(manager->get</xsl:text>
	<xsl:value-of select="@name"/>

	<xsl:text>(intId));

	BERKELIUM_C_TRACE_RETURN(ret.get());

	return ret;
}
</xsl:text>
		</xsl:otherwise>
	</xsl:choose>

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


<xsl:template name="returnDefault">
	<xsl:text>return</xsl:text>
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
	<xsl:text>;</xsl:text>
</xsl:template>

</xsl:stylesheet>
