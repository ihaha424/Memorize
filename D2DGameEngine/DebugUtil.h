#pragma once

#include "framework.h"

typedef enum LogType {
	LOG_TYPE_CONSOLE,
	LOG_TYPE_FILE_IO,
} LogType;

typedef enum SeverityFlagBit {
	LOG_SEVERITY_VERBOSE = 0x0001,
	LOG_SEVERITY_MESSAGE = 0x0002,
	LOG_SEVERITY_INFO = 0x0004,
	LOG_SEVERITY_WARNING = 0x0008,
	LOG_SEVERITY_ERROR = 0x0010,
} SeverityFlagBit;
using SeverityFlags = Flags;

typedef enum MessageTypeFlagBit {
	LOG_MESSAGE_TYPE_GENERAL = 0x0001,
	LOG_MESSAGE_TYPE_PERFORMANCE = 0x0002,
	LOG_MESSAGE_TYPE_DEBUG = 0x0004,
	LOG_MESSAGE_TYPE_OTHER = 0x0008,
} MessageTypeFlagBit;
using MessageTypeFlags = Flags;


constexpr	int32_t ERROR_UNDEFINED = (std::numeric_limits<int32_t>::min)();
constexpr int32_t ERROR_NONE = 0;

constexpr uint32_t OBJ_ID_NONE = (std::numeric_limits<uint32_t>::min)();

constexpr uint32_t MSG_ID_NONE = (std::numeric_limits<uint32_t>::min)();

struct CallbackData {
	int64_t		errorCode;			// Error code
	uint64_t		objectId;			// Object Id
	const char* pObjectName;	// Object Name
	uint64_t		messageId;		// Message Id
	std::string message;			// Message
};

using cstream = std::FILE;
using DebugCallback = bool (*)(
	cstream*,
	SeverityFlagBit,
	MessageTypeFlagBit,
	const CallbackData*,
	void*
	);

namespace dbg {
	/**
	 * @brief 함수의 파라미터들을 조합하여 문자열로 변환해 줍니다.
	 * @tparam ...Args 함수 파라미터 타입 팩
	 * @param ...args 함수 파라미터 팩
	 * @return 파라미터가 합쳐진 문자열.
	 */
	template<typename... Args>
	inline constexpr
		std::string text(Args&&... args) {
		return (std::string() + ... + std::format("{}", args));
	}

	/**
	 * @brief `printf`와 같이 파라미터를 포메팅하여 문자열로 변환해 줍니다.
	 * @tparam ...Args 함수 파라미터 타입 팩
	 * @param vstr 포멧 문자열
	 * @param ...args 포멧에 들어갈 파라미터들의 변수 팩
	 * @return 포메팅이 적용된 문자열
	 */
	template<typename... Args>
	inline constexpr
		std::string format(std::string_view vstr, Args&&... args) {
		return std::vformat(vstr, std::make_format_args(args...));
	}

	/**
	 * @brief 특정 스트림을 통해 파라미터들을 출력합니다.
	 * @tparam ...Args 함수 파라미터 타입 팩
	 * @param stream C-스타일 스트림 e.g., FILE*, 혹은 stdout
	 * @param ...args 파라미터 팩
	 * @return 스트림을 통해 출력된 문자의 개수
	 */
	template<typename... Args>
	inline constexpr
		int fprint(cstream* stream, Args&&... args) {
		std::string str = text(std::forward<Args>(args)...);
		return std::fprintf(stream, "%s", str.c_str());
	}

	/**
	 * @brief dbg::fprint와 동일하지만, 문자열에 포메팅이 적용됩니다.
	 * @tparam ...Args 함수 파라미터 타입 팩
	 * @param stream C-스타일 스트림 e.g., FILE*, 혹은 stdout
	 * @param vstr 파라미터 팩
	 * @param ...args 스트림을 통해 출력된 문자의 개수
	 * @return 
	 */
	template<typename... Args>
	inline constexpr
		int fprintf(cstream* stream, std::string_view vstr, Args&&... args) {
		std::string str = format(vstr, std::forward<Args>(args)...);
		return std::fprintf(stream, "%s", str.c_str());
	}

	/**
	 * @brief dbg::fprint와 동일하지만, 문자열 끝에 '\n'을 삽입합니다.
	 * @tparam ...Args 함수 파라미터 타입 팩
	 * @param stream C-스타일 스트림 e.g., FILE*, 혹은 stdout
	 * @param ...args 파라미터 팩
	 * @return 스트림을 통해 출력된 문자의 개수
	 */
	template<typename... Args>
	inline constexpr
		int fprintln(cstream* stream, Args&&... args) {
		std::string str = text(std::forward<Args>(args)...);
		return std::fprintf(stream, "%s\n", str.c_str());
	}

	/**
	 * @brief stdout으로 문자열을 출력합니다.
	 * @tparam ...Args 함수 파라미터 타입 팩
	 * @param ...args 파라미터 팩
	 * @return 스트림을 통해 출력된 문자의 개수
	 */
	template<typename... Args>
	inline constexpr
		int console_out(Args&&... args) {
		std::string str = text(std::forward<Args>(args)...);
		return std::fprintf(stdout, "%s\n", str.c_str());
	}
}



