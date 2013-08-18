#define BERKELIUM_CPP_IMPL

#include <Berkelium/API/LogDelegate.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/Impl/Manager.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <berkelium.h>

typedef void logger(BK_Runtime runtime, BK_LogSource source, BK_LogType type, char* clazz, char* name, char* msg);

class BK_LogDelegateImpl : public Berkelium::LogDelegate
{
private:
	logger* l;

public:
	BK_LogDelegateImpl(void* delegate) :
		Berkelium::LogDelegate(),
		l((logger*)delegate) {
	}

	virtual void log(Berkelium::RuntimeRef runtime, Berkelium::LogSource source, Berkelium::LogType type,
			const std::string& clazz, const std::string& name, const std::string& message) {

	}
};

bk_bk_obj mapIn(BK_Env_Enum type, bk_ext_obj id, void* data)
{
	if(type == LogDelegate) {
		return new Berkelium::LogDelegateRef(new BK_LogDelegateImpl(id));
	}
	/*
	fprintf(stderr, "mapIn %s %p\n", BK_Env_Enum_To_String_Or_Err(type), id);
	*/
	return (bk_bk_obj)id;
}

bk_ext_obj mapOut(BK_Env_Enum type, bk_bk_obj id, void* data)
{
	/*
	fprintf(stderr, "mapOut %s %p\n", BK_Env_Enum_To_String_Or_Err(type), id);
	*/
	return (bk_ext_obj)id;
}

bk_ext_obj mapNew(BK_Env_Enum type, bk_bk_obj id, void* data)
{
	/*
	fprintf(stderr, "mapNew %s %p\n", BK_Env_Enum_To_String_Or_Err(type), id);
	*/
	return (bk_ext_obj)id;
}

void mapInError(BK_Env_Enum expected, BK_Env_Enum actual, bk_ext_obj id, void* data)
{
}

extern "C" void setupBkEnv(BK_Env* env)
{
	env->mapIn = &mapIn;
	env->mapOut = &mapOut;
	env->mapNew = &mapNew;
	env->mapInError = &mapInError;
}
