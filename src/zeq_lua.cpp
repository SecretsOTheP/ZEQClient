
#include "zeq_lua.h"

static lua_State* L;

static void lua_newtable_tolowerkey(lua_State* L)
{
	lua_newtable(L);
	luaL_getmetatable(L, "tolowerkey_meta");
	lua_setmetatable(L, -2);
}

namespace Lua
{
	void initialize()
	{
		L = luaL_newstate();
		if (L)
			luaL_openlibs(L);

		//create tolowerkey metatable
		luaL_newmetatable(L, "tolowerkey_meta");
		luaL_dostring(L,
			"local type = type\n"
			"local rawset = rawset\n"
			"local function set(t, k, v)\n"
			"	if type(k) == 'string' then\n"
			"		k = k:lower()\n"
			"	end\n"
			"	rawset(t, k, v)\n"
			"end\n"
			"return set\n"
		);
		lua_setfield(L, -2, "__newindex");
		lua_pop(L, 1);
	}

	void close()
	{
		if (L)
			lua_close(L);
	}

	void fileToTable(const char* path, const char* tbl_name, bool lowercase_keys)
	{
		if (lowercase_keys)
			lua_newtable_tolowerkey(L);
		else
			lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setfield(L, LUA_REGISTRYINDEX, tbl_name);
		if (luaL_loadfile(L, path) == 0)
		{
			lua_pushvalue(L, -2);
			lua_setfenv(L, -2);
			if (lua_pcall(L, 0, 0, 0) == 0)
			{
				return;
			}
		}
		const char* err = lua_tostring(L, -1);
		printf("Error running '%s': %s\n", path, err);
		lua_pop(L, 1);
	}

	void fileToHashTable(const char* path, std::unordered_map<std::string, int, std::hash<std::string>>& table)
	{
		lua_newtable(L);
		if (luaL_loadfile(L, path) == 0)
		{
			lua_pushvalue(L, -2);
			lua_setfenv(L, -2);
			if (lua_pcall(L, 0, 0, 0) == 0)
			{
				lua_pushnil(L);
				while (lua_next(L, -2))
				{
					//key at -2, value at -1
					table[lua_tostring(L, -2)] = lua_tointeger(L, -1);
					lua_pop(L, 1);
				}
				lua_pop(L, 1);
				return;
			}
		}
		const char* err = lua_tostring(L, -1);
		printf("Error running '%s': %s\n", path, err);
		lua_pop(L, 2);
	}

	int getInt(const char* varname, const char* tbl)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, tbl);
		lua_getfield(L, -1, varname);
		int ret = lua_tointeger(L, -1);
		lua_pop(L, 2);
		return ret;
	}

	int getInt(const char* varname, const char* tbl, int default)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, tbl);
		lua_getfield(L, -1, varname);
		int ret;
		if (lua_isnil(L, -1))
			ret = default;
		else
			ret = lua_tointeger(L, -1);
		lua_pop(L, 2);
		return ret;
	}

	std::string getString(const char* varname, const char* tbl)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, tbl);
		lua_getfield(L, -1, varname);
		std::string ret;
		if (lua_tostring(L, -1))
			ret = lua_tostring(L, -1);
		lua_pop(L, 2);
		return ret;
	}

	std::string getString(const char* varname, const char* tbl, const char* default)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, tbl);
		lua_getfield(L, -1, varname);
		std::string ret;
		if (lua_isnil(L, -1))
			ret = default;
		else
			ret = lua_tostring(L, -1);
		lua_pop(L, 2);
		return ret;
	}

	bool getBool(const char* varname, const char* tbl)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, tbl);
		lua_getfield(L, -1,varname);
		bool ret = (bool)lua_toboolean(L, -1);
		lua_pop(L, 2);
		return ret;
	}

	bool getBool(const char* varname, const char* tbl, bool default)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, tbl);
		lua_getfield(L, -1, varname);
		bool ret;
		if (lua_isnil(L, -1))
			ret = default;
		else
			ret = (bool)lua_toboolean(L, -1);
		lua_pop(L, 2);
		return ret;
	}

	int getConfigInt(const char* varname)
	{
		return getInt(varname, CONFIG_TABLE);
	}

	int getConfigInt(const char* varname, int default)
	{
		return getInt(varname, CONFIG_TABLE, default);
	}

	std::string getConfigString(const char* varname)
	{
		return getString(varname, CONFIG_TABLE);
	}

	std::string getConfigString(const char* varname, const char* default)
	{
		return getString(varname, CONFIG_TABLE, default);
	}

	bool getConfigBool(const char* varname)
	{
		return getBool(varname, CONFIG_TABLE);
	}

	bool getConfigBool(const char* varname, bool default)
	{
		return getBool(varname, CONFIG_TABLE, default);
	}
}
