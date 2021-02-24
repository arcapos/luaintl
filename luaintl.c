/*
 * Copyright (c) 2013 - 2021 by Micro Systems Marc Balmer,
 * CH-5073 Gipf-Oberfrick, Switzerland. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Micro Systems Marc Balmer nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Lua binding for libintl */

#include <libintl.h>
#include <locale.h>
#include <lauxlib.h>

static int
lua_textdomain(lua_State *L)
{
	lua_pushstring(L, textdomain(lua_tolstring(L, 1, NULL)));
	return 1;
}

static int
lua_bind_textdomain_codeset(lua_State *L)
{
	const char *domain = luaL_checkstring(L, 1);

	if (domain[0] == '\0')
		return luaL_error(L,
		    "bind_textdomain_codeset with empty domain");

	lua_pushstring(L, bind_textdomain_codeset(domain,
	    lua_tolstring(L, 2, NULL)));
	return 1;
}

static int
lua_bindtextdomain(lua_State *L)
{
	const char *domain = luaL_checkstring(L, 1);

	if (domain[0] == '\0')
		return luaL_error(L, "bindtextdomain with empty domain");

	lua_pushstring(L, bindtextdomain(domain, lua_tolstring(L, 2, NULL)));
	return 1;
}

static int category[] = {
	LC_CTYPE, LC_NUMERIC, LC_TIME, LC_COLLATE, LC_MONETARY, LC_MESSAGES,
	LC_ALL,
#ifdef __linux__
	LC_PAPER, LC_NAME, LC_ADDRESS, LC_TELEPHONE, LC_MEASUREMENT,
	LC_IDENTIFICATION
#endif
};

static const char *category_names[] = {
	"LC_CTYPE", "LC_NUMERIC", "LC_TIME", "LC_COLLATE", "LC_MONETARY",
	"LC_MESSAGES", "LC_ALL",
#ifdef __linux__
	"LC_PAPER", "LC_NAME", "LC_ADDRESS", "LC_TELEPHONE", "LC_MEASUREMENT",
	"LC_IDENTIFICATION",
#endif
	NULL
};

static int
lua_gettext(lua_State *L)
{
	lua_pushstring(L, gettext(luaL_checkstring(L, 1)));
	return 1;
}

static int
lua_dgettext(lua_State *L)
{
	lua_pushstring(L, dgettext(lua_tolstring(L, 1, NULL),
	    luaL_checkstring(L, 2)));
	return 1;
}

static int
lua_dcgettext(lua_State *L)
{
	lua_pushstring(L, dcgettext(lua_tolstring(L, 1, NULL),
	    luaL_checkstring(L, 2),
	    category[luaL_checkoption(L, 3, NULL, category_names)]));
	return 1;
}

static int
lua_ngettext(lua_State *L)
{
	lua_pushstring(L, dngettext(luaL_checkstring(L, 1),
	    luaL_checkstring(L, 2), luaL_checkstring(L, 3),
	    luaL_checkinteger(L, 4)));
	return 1;
}

static int
lua_dngettext(lua_State *L)
{
	lua_pushstring(L, dngettext(lua_tolstring(L, 1, NULL),
	    luaL_checkstring(L, 2), luaL_checkstring(L, 3),
	    luaL_checkinteger(L, 4)));
	return 1;
}


static int
lua_dcngettext(lua_State *L)
{
	lua_pushstring(L, dcngettext(lua_tolstring(L, 1, NULL),
	    luaL_checkstring(L, 2), luaL_checkstring(L, 3),
	    luaL_checkinteger(L, 4),
	    category[luaL_checkoption(L, 5, NULL, category_names)]));
	return 1;
}

#ifdef HAVE_PGETTEXT
static int
lua_pgettext(lua_State *L)
{
	lua_pushstring(L, pgettext(luaL_checkstring(L, 1),
	    luaL_checkstring(L, 2));
	return 1;
}

static int
lua_dpgettext(lua_State *L)
{
	lua_pushstring(L, dpgettext(luaL_checkstring(L, 1),
	    luaL_checkstring(L, 2), luaL_checkstring(L, 3));
	return 1;
}


static int
lua_dcpgettext(lua_State *L)
{
	lua_pushstring(L, dcpgettext(lua_tolstring(L, 1, NULL),
	    luaL_checkstring(L, 2), luaL_checkstring(L, 3),
	    category[luaL_checkoption(L, 4, NULL, category_names)]));
	return 1;
}
#endif

static int
lua_setlocale(lua_State *L)
{
	char *locale;

	if (lua_gettop(L) < 2)
		locale = setlocale(category[luaL_checkoption(L, 1, NULL,
		    category_names)], NULL);
	else
		locale = setlocale(category[luaL_checkoption(L, 1, NULL,
		    category_names)], luaL_checkstring(L, 2));

	if (locale == NULL)
		lua_pushnil(L);
	else
		lua_pushstring(L, locale);
	return 1;
}

static luaL_Reg gettext_functions[] = {
	{ "bindtextdomain",		lua_bindtextdomain },
	{ "bind_textdomain_codeset",	lua_bind_textdomain_codeset },
	{ "textdomain",			lua_textdomain },
	{ "gettext",			lua_gettext },
	{ "dgettext",			lua_dgettext },
	{ "dcgettext",			lua_dcgettext },
	{ "ngettext",			lua_ngettext },
	{ "dngettext",			lua_dngettext },
	{ "dcngettext",			lua_dcngettext },
#ifdef HAVE_PGETTEXT
	{ "pgettext",			lua_pgettext },
	{ "dpgettext",			lua_dpgettext },
	{ "dcpgettext",			lua_dcpgettext },
#endif
	{ "setlocale",			lua_setlocale },
	{ NULL,				NULL }
};

int
luaopen_intl(lua_State *L)
{
	setlocale(LC_ALL, "");
#if LUA_VERSION_NUM >= 502
	luaL_newlib(L, gettext_functions);
#else
	luaL_register(L, "intl", gettext_functions);
#endif
	lua_pushliteral(L, "_COPYRIGHT");
	lua_pushliteral(L, "Copyright (C) 2013 - 2021 micro systems marc "
	    "balmer.  All rights reserved.");
	lua_settable(L, -3);
	lua_pushliteral(L, "_DESCRIPTION");
	lua_pushliteral(L, "Lua binding for libintl");
	lua_settable(L, -3);
	lua_pushliteral(L, "_VERSION");
	lua_pushliteral(L, "intl 1.3.1		");
	lua_settable(L, -3);

	return 1;
}
