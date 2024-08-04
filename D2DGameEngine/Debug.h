#pragma once

#include "DebugLogger.h"

#define INIT_LOGGER(type, \
	severityFlags, \
	messageTypeFlags, \
	debugCallback, \
	concurrent) \
		(InitializeLogger( \
		type, \
		severityFlags, \
		messageTypeFlags, \
		debugCallback, \
		concurrent))

#define FINAL_LOGGER() (FinalizeLogger())

#ifndef NDEBUG
#define LOG(...) (Logger::Log(__VA_ARGS__))
#else
#define LOG(...) ()
#endif

#ifndef NDEBUG
#define LOG_VERBOSE(msg) (Logger::Log(LOG_SEVERITY_VERBOSE, LOG_MESSAGE_TYPE_GENERAL, MSG_ID_NONE, msg))
#else
#define LOG_VERBOSE(...) ()
#endif

#ifndef NDEBUG
#define LOG_MESSAGE(msg) (Logger::Log(LOG_SEVERITY_MESSAGE, LOG_MESSAGE_TYPE_GENERAL, MSG_ID_NONE, msg))
#else
#define LOG_MESSAGE(...) ()
#endif

#ifndef NDEBUG
#define LOG_INFO(msg) (Logger::Log(LOG_SEVERITY_INFO, LOG_MESSAGE_TYPE_GENERAL, MSG_ID_NONE, msg))
#else
#define LOG_INFO(...) ()
#endif

#ifndef NDEBUG
#define LOG_WARNING(msg) (Logger::Log(LOG_SEVERITY_WARNING, LOG_MESSAGE_TYPE_GENERAL, MSG_ID_NONE, msg))
#else
#define LOG_WARNING(...) ()
#endif

#define LOG_ERROR(errcode, msg) (Logger::Log(LOG_SEVERITY_ERROR, LOG_MESSAGE_TYPE_GENERAL, errcode, MSG_ID_NONE, msg))

#define LOG_REGISTER_OBJ(T) CallbackData __obj_callback_data { \
	.errorCode		= ERROR_NONE, \
	.objectId			= ObjectRegistryID::Get<T>(), \
	.pObjectName	= typeid(T).name(), \
	.messageId		= MSG_ID_NONE, \
	.message			= std::string(), \
};

// https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

#ifndef NDEBUG
#define LOG_OBJ_6(severity, msg_type, err_code, msg_id, msg, pUser) { \
		CallbackData data {	\
			.errorCode = err_code, \
			.objectId = __obj_callback_data.objectId, \
			.pObjectName = __obj_callback_data.pObjectName, \
			.messageId = msg_id, \
			.message = msg, \
		}; \
		Logger::Log(severity, msg_type, &data, pUser); \
	}
#else
#define LOG_OBJ_6(...) ()
#endif

#ifndef NDEBUG
#define LOG_OBJ_5(severity, msg_type, err_code, msg_id, msg) { \
		CallbackData data {	\
			.errorCode = err_code, \
			.objectId = __obj_callback_data.objectId, \
			.pObjectName = __obj_callback_data.pObjectName, \
			.messageId = msg_id, \
			.message = msg, \
		}; \
		Logger::Log(severity, msg_type, &data); \
	}
#else
#define LOG_OBJ_5(...) ()
#endif

#ifndef NDEBUG
#define LOG_OBJ_4(severity, msg_type, msg_id, msg) { \
		CallbackData data {	\
			.errorCode = ERROR_UNDEFINED, \
			.objectId = __obj_callback_data.objectId, \
			.pObjectName = __obj_callback_data.pObjectName, \
			.messageId = msg_id, \
			.message = msg, \
		}; \
		Logger::Log(severity, msg_type, &data); \
	}
#else
#define LOG_OBJ_4(...) ()
#endif

#ifndef NDEBUG
#define LOG_OBJ_1(msg) { \
		CallbackData data {	\
			.errorCode = ERROR_UNDEFINED, \
			.objectId = __obj_callback_data.objectId, \
			.pObjectName = __obj_callback_data.pObjectName, \
			.messageId = MSG_ID_NONE, \
			.message = msg, \
		}; \
		Logger::Log(LOG_SEVERITY_INFO, LOG_MESSAGE_TYPE_GENERAL, &data); \
	}
#else
#define LOG_OBJ_1(...) ()
#endif

#ifndef NDEBUG
#define LOG_OBJ(...) GET_MACRO(__VA_ARGS__, LOG_OBJ_6, LOG_OBJ_5, LOG_OBJ_4, _,	_, LOG_OBJ_1)(__VA_ARGS__)
#else
#define LOG_OBJ(...) ()
#endif

#ifndef NDEBUG
#define OBJ_VERBOSE(msg) { \
		CallbackData data {	\
			.errorCode = ERROR_NONE, \
			.objectId = __obj_callback_data.objectId, \
			.pObjectName = __obj_callback_data.pObjectName, \
			.messageId = MSG_ID_NONE, \
			.message = msg, \
		}; \
		Logger::Log(LOG_SEVERITY_VERBOSE, LOG_MESSAGE_TYPE_GENERAL, &data); \
	}
#else
#define OBJ_VERBOSE(...) ()
#endif

#ifndef NDEBUG
#define OBJ_MESSAGE(msg) { \
		CallbackData data {	\
			.errorCode = ERROR_NONE, \
			.objectId = __obj_callback_data.objectId, \
			.pObjectName = __obj_callback_data.pObjectName, \
			.messageId = MSG_ID_NONE, \
			.message = msg, \
		}; \
		Logger::Log(LOG_SEVERITY_MESSAGE, LOG_MESSAGE_TYPE_GENERAL, &data); \
	}
#else
#define OBJ_MESSAGE(...) ()
#endif

#ifndef NDEBUG
#define OBJ_INFO(msg) { \
		CallbackData data {	\
			.errorCode = ERROR_NONE, \
			.objectId = __obj_callback_data.objectId, \
			.pObjectName = __obj_callback_data.pObjectName, \
			.messageId = MSG_ID_NONE, \
			.message = msg, \
		}; \
		Logger::Log(LOG_SEVERITY_INFO, LOG_MESSAGE_TYPE_GENERAL, &data); \
	}
#else
#define OBJ_INFO(...) ()
#endif

#ifndef NDEBUG
#define OBJ_WARNING(msg) { \
		CallbackData data {	\
			.errorCode = ERROR_NONE, \
			.objectId = __obj_callback_data.objectId, \
			.pObjectName = __obj_callback_data.pObjectName, \
			.messageId = MSG_ID_NONE, \
			.message = msg, \
		}; \
		Logger::Log(LOG_SEVERITY_WARNING, LOG_MESSAGE_TYPE_GENERAL, &data); \
	}
#else
#define OBJ_WARNING(...) ()
#endif

#define OBJ_ERROR(errcode, msg) { \
	CallbackData data {	\
		.errorCode = errcode, \
		.objectId = __obj_callback_data.objectId, \
		.pObjectName = __obj_callback_data.pObjectName, \
		.messageId = MSG_ID_NONE, \
		.message = msg, \
	}; \
	Logger::Log(LOG_SEVERITY_ERROR, LOG_MESSAGE_TYPE_GENERAL, &data); \
}

