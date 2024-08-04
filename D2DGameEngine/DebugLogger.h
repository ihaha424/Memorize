#pragma once

#include "framework.h"

#include "DebugUtil.h"

#include "Singleton.h"

#include "ObjectRegistryID.h"
#include "TypeRegistryID.h"

#include <vector>
#include <queue>

class Logger : public Singleton<Logger> {
	using Super = Singleton<Logger>;

	struct TaskData {
		SeverityFlagBit severity;
		MessageTypeFlagBit messageType;
		CallbackData pData;
		void* pUser;
	};

	bool _concurrent;
	std::mutex _m;
	std::condition_variable _cv;
	std::queue<TaskData> _taskDataQueue;
	std::thread _thread;
	bool _exit{ false };

	LogType _type;
	cstream* _stream;
	SeverityFlags _severities;
	MessageTypeFlags _messageTypes;
	DebugCallback _callback;

public:
	/**
	 * @brief 로거를 생성하고 초기화 합니다.
	 * @param type: LogType - 로거의 타입을 지정합니다. 
	 * 로거의 타입으로는 `LOG_TYPE_CONSOLE` 와 `LOG_TYPE_FILE_IO` 이 있습니다.
	 * @param severityFlags: SeverityFlags - 로거가 출력할 심각도 정도를 지정합니다.
	 * 심각도는 `LOG_SEVERITY_VERBOSE` 와 `LOG_SEVERITY_MESSAGE`, 
	 * `LOG_SEVERITY_INFO`, `LOG_SEVERITY_WARNING`, `LOG_SEVERITY_ERROR` 가 있습니다.
	 * 콘솔 사용시 각각의 심각도는 서로 다른 색으로 출력됩니다.
	 * @param messageTypeFlags: MessageTypeFlags - 로거가 출력할 메세지 타입 채널을 지정합니다.
	 * 기본 채널로는 `LOG_MESSAGE_TYPE_GENERAL` 와 `LOG_MESSAGE_TYPE_PERFORMANCE`, 
	 * `LOG_MESSAGE_TYPE_DEBUG`, `LOG_MESSAGE_TYPE_OTHER` 가 있습니다.
	 * @param debugCallback: DebugCallback - 메세지 출력 시 필요한 디버그 콜백 함수 포인터 입니다.
	 * @param concurrent: bool - True 일 경우 멀티 스레드를 사용합니다.
	 */
	Logger(
		LogType type,
		SeverityFlags severityFlags,
		MessageTypeFlags messageTypeFlags,
		DebugCallback debugCallback,
		bool concurrent = false
	);

	virtual ~Logger();

	static bool Log(
		SeverityFlagBit severity,
		MessageTypeFlagBit messageType,
		CallbackData* pData,
		void* pUser = nullptr
	);

	static bool Log(
		SeverityFlagBit severity,
		MessageTypeFlagBit messageType,
		int64_t errorCode,
		uint64_t objectId,
		const char* pObjectName,
		uint64_t messageId,
		std::string&& message,
		void* pUser = nullptr
	);

	static bool Log(
		SeverityFlagBit severity,
		MessageTypeFlagBit messageType,
		int64_t errorCode,
		uint64_t messageId,
		std::string&& message,
		void* pUser = nullptr
	);

	static bool Log(
		SeverityFlagBit severity,
		MessageTypeFlagBit messageType,
		uint64_t messageId,
		std::string&& message,
		void* pUser = nullptr
	);
};

/**
 * @brief 디버그 로거를 초기화 합니다.
 * @param type: LogType - 로거의 타입을 지정합니다.
 * 로거의 타입으로는 `LOG_TYPE_CONSOLE` 와 `LOG_TYPE_FILE_IO` 이 있습니다.
 * @param severityFlags: SeverityFlags - 로거가 출력할 심각도 정도를 지정합니다.
 * 심각도는 `LOG_SEVERITY_VERBOSE` 와 `LOG_SEVERITY_MESSAGE`,
 * `LOG_SEVERITY_INFO`, `LOG_SEVERITY_WARNING`, `LOG_SEVERITY_ERROR` 가 있습니다.
 * 콘솔 사용시 각각의 심각도는 서로 다른 색으로 출력됩니다.
 * @param messageTypeFlags: MessageTypeFlags - 로거가 출력할 메세지 타입 채널을 지정합니다.
 * 기본 채널로는 `LOG_MESSAGE_TYPE_GENERAL` 와 `LOG_MESSAGE_TYPE_PERFORMANCE`,
 * `LOG_MESSAGE_TYPE_DEBUG`, `LOG_MESSAGE_TYPE_OTHER` 가 있습니다.
 * @param debugCallback: DebugCallback - 메세지 출력 시 필요한 디버그 콜백 함수 포인터 입니다.
 * @param concurrent: bool - True 일 경우 멀티 스레드를 사용합니다.
 */
inline
void InitializeLogger(LogType type,
	SeverityFlags severityFlags,
	MessageTypeFlags messageTypeFlags,
	DebugCallback debugCallback,
	bool concurrent) {
	Logger::CreateInstance(
		type,
		severityFlags,
		messageTypeFlags,
		debugCallback,
		concurrent
	);
}

/**
 * @brief 로거를 종료하고, 디버깅 스트림을 닫습니다. 멀티 스레드 모드 시, 스레드를 종료합니다.
 */
inline
void FinalizeLogger() {
	Logger::DestroyInstance();
}

