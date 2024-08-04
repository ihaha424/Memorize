#ifndef UNICODE
#define UNICODE
#endif

#include "common.h"

#include "GameApp.h"

inline
bool _debug_callback(
	cstream* stream,
	SeverityFlagBit severity,
	MessageTypeFlagBit messageType,
	const CallbackData* pCallbackData,
	void* pUser);

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR pCmdLine,
	_In_ int nCmdShow
) {
	// Start Logger
	INIT_LOGGER(
		LOG_TYPE_CONSOLE,
		LOG_SEVERITY_MESSAGE | LOG_SEVERITY_INFO | LOG_SEVERITY_WARNING | LOG_SEVERITY_ERROR,
		LOG_MESSAGE_TYPE_GENERAL | LOG_MESSAGE_TYPE_PERFORMANCE | LOG_MESSAGE_TYPE_DEBUG | LOG_MESSAGE_TYPE_OTHER,
		_debug_callback,
		true
	);

	GameApp app;
	app.Initialize();
	app.Execute();
	app.Shutdown();

	FINAL_LOGGER();
	return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////
//											 디버그 콜백 함수 셋업																							//
//////////////////////////////////////////////////////////////////////////////////////////
#define ESC "\x1b"
#define CSI "\x1b["

static struct TEXT_COLOR {
	static constexpr const char* DEFAULT{ CSI "0m" };
	static constexpr const char* BLACK{ CSI "30m" };
	static constexpr const char* RED{ CSI "31m" };
	static constexpr const char* GREEN{ CSI "32m" };
	static constexpr const char* YELLOW{ CSI "33m" };
	static constexpr const char* BLUE{ CSI "34m" };
	static constexpr const char* MAGENTA{ CSI "35m" };
	static constexpr const char* CYAN{ CSI "36m" };
	static constexpr const char* WHITE{ CSI "37m" };
};

inline
bool _debug_callback(
	cstream* stream,
	SeverityFlagBit severity,
	MessageTypeFlagBit messageType,
	const CallbackData* pCallbackData,
	void* pUser) {

	// Tag
	const char* tag;
	const char* textColor;
	switch (severity)
	{
	case LOG_SEVERITY_VERBOSE: {
		tag = "[VERBOSE]";
		textColor = TEXT_COLOR::WHITE;
	} break;
	case LOG_SEVERITY_MESSAGE: {
		tag = "[MESSAGE]";
		textColor = TEXT_COLOR::WHITE;
	} break;
	case LOG_SEVERITY_INFO: {
		tag = "[  INFO ]";
		textColor = TEXT_COLOR::GREEN;
	} break;
	case LOG_SEVERITY_WARNING: {
		tag = "[WARNING]";
		textColor = TEXT_COLOR::YELLOW;
	} break;
	case LOG_SEVERITY_ERROR: {
		tag = "[ ERROR ]";
		textColor = TEXT_COLOR::RED;
	} break;
	default:
		textColor = TEXT_COLOR::WHITE;
		break;
	}
	dbg::fprint(stream, textColor, tag, ' ');

	// Error info
	if (severity == LOG_SEVERITY_ERROR) {
		std::string errName;
		switch (pCallbackData->errorCode) {
		case ERROR_UNDEFINED: {
			errName = "UNDEFINED"s;
		} break;
		case ERROR_NONE: {
			errName = ""s;
		} break;
		default: {
			errName = dbg::text(pCallbackData->errorCode);
		} break;
		}
		if (!errName.empty()) {
			dbg::fprint(stream, "errno=", errName, " | ");
		}
	}

	// Object Info
	if (pCallbackData->pObjectName) {
		dbg::fprint(stream,
			pCallbackData->pObjectName, "(",
			pCallbackData->objectId, ") | ");
	}

	// Message Info
	std::string messageID;
	switch (pCallbackData->messageId)
	{
	case MSG_ID_NONE: {
		messageID = "";
	} break;
	default: {
		messageID = dbg::text(pCallbackData->messageId);
	} break;
	}
	if (!messageID.empty()) {
		dbg::fprint(stream, messageID, "|", pCallbackData->message);
	}
	else {
		dbg::fprint(stream, pCallbackData->message);
	}
	dbg::fprint(stream, '\n');

	// Reset the print color
	dbg::fprint(stream, TEXT_COLOR::DEFAULT);

	// Abort if the severity level is ERROR
	if (severity == LOG_SEVERITY_ERROR) {
		abort();
	}

	return true;
}
