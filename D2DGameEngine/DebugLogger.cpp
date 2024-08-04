#include "DebugLogger.h"

#include "DebugConsole.h"

#include <chrono>

Logger::Logger(
	LogType type,
	SeverityFlags severityFlags,
	MessageTypeFlags messageTypeFlags,
	DebugCallback debugCallback,
	bool concurrent) :
	_type{ type },
	_severities{ severityFlags },
	_messageTypes{ messageTypeFlags },
	_callback{ debugCallback },
	_concurrent{ concurrent } {

	// The way to log.
	if (type == LOG_TYPE_CONSOLE) {
		if (!AttachDebugConsole())
			throw std::runtime_error("Logger: Debug console allocation failed!");
		_stream = stdout;
	}
	else if (type == LOG_TYPE_FILE_IO) {
		std::filesystem::create_directory("Logs");

		// ChatGPT generated...
		// Get current time_point
		auto now = std::chrono::system_clock::now();
		// Truncate to seconds: remove fractional seconds
		auto now_sec = std::chrono::time_point_cast<std::chrono::seconds>(now);
		// Format the time_point without fractional seconds
		std::string s = std::format("{0:%F}T{0:%H}{0:%M}{0:%S}.txt", now_sec);

		_stream = std::fopen(("Logs/"s + s).c_str(), "w");
	}
	else {
		throw std::invalid_argument("Logger: LogType is unknown!");
	}

#ifndef NDEBUG
	dbg::fprintln(_stream, "Logger setup...");
#endif

	// Concurrency Setting
	if (concurrent) {
#ifndef NDEBUG
		dbg::fprintln(_stream, "\tConcurrency mode is on.");
#endif
		// Set up a thread
		_thread = std::thread([this] {
#ifndef NDEBUG
			dbg::fprintln(_stream, "\tLogger thread is created.");
#endif
			while (true) {
				// Mutex lock
				std::unique_lock<std::mutex> lk(_m);
				// cv(lock, condition);
				_cv.wait(lk,
					[this] { return !_taskDataQueue.empty() || _exit; }
				);

				// Exit when _cv is notify 
				// but the task queue is empty.
				if (_taskDataQueue.empty()) {
#ifndef NDEBUG
					dbg::fprintln(_stream, "\tLogger thread is terminated.");
#endif
					return;
				}
				// Run thread with task
				TaskData data = std::move(_taskDataQueue.front());
				_taskDataQueue.pop();

				if ((_severities & data.severity) &&
					(_messageTypes & data.messageType)) {
					_callback(
						_stream,
						data.severity,
						data.messageType,
						&data.pData,
						data.pUser
					);
				}
			}
			});
	}

#ifndef NDEBUG
	dbg::fprintln(_stream, "done.");
#endif
}

Logger::~Logger() {
	_exit = true;
	_cv.notify_all();
	_thread.join();

	if (_type == LOG_TYPE_CONSOLE) {
		DetachDebugConsole();
	}
}

bool Logger::Log(
	SeverityFlagBit severity,
	MessageTypeFlagBit messageType,
	CallbackData* pData,
	void* pUser) {
	if (!Super::_instance) return false;

	// Cuncurrent Mode
	if (Super::_instance->_concurrent) {
		// Lock
		std::unique_lock<std::mutex> lk(Super::_instance->_m);

		// Push task data
		Super::_instance->_taskDataQueue.push(
			TaskData{
				.severity = severity,
				.messageType = messageType,
				.pData = {
					.errorCode = pData->errorCode,
					.objectId = pData->objectId,
					.pObjectName = pData->pObjectName,
					.messageId = pData->messageId,
					.message = std::move(pData->message),
				},
				.pUser = pUser,
			}
			);

		// Unlock
		lk.unlock();

		// Notify One
		Super::_instance->_cv.notify_one();

		// Return
		return true;
	}

	// Default Mode
	if ((Super::_instance->_severities & severity) &&
		(Super::_instance->_messageTypes & messageType))
		return false;

	return Super::_instance->_callback(
		_instance->_stream,
		severity,
		messageType,
		pData,
		pUser
	);
}

bool Logger::Log(
	SeverityFlagBit severity,
	MessageTypeFlagBit messageType,
	int64_t errorCode,
	uint64_t objectId,
	const char* pObjectName,
	uint64_t messageId,
	std::string&& message,
	void* pUser) {
	// Construct a callback data
	CallbackData data{
		.errorCode = errorCode,
		.objectId = objectId,
		.pObjectName = pObjectName,
		.messageId = messageId,
		.message = std::move(message)
	};
	// Pass it to the overloaded Log function.
	return Log(severity, messageType, &data, pUser);
}

bool Logger::Log(
	SeverityFlagBit severity,
	MessageTypeFlagBit messageType,
	int64_t errorCode,
	uint64_t messageId,
	std::string&& message,
	void* pUser) {
	// Construct a callback data
	CallbackData data{
		.errorCode = errorCode,
		.objectId = OBJ_ID_NONE,
		.pObjectName = nullptr,
		.messageId = messageId,
		.message = std::move(message)
	};
	// Pass it to the overloaded Log function.
	return Log(severity, messageType, &data, pUser);
}

bool Logger::Log(
	SeverityFlagBit severity,
	MessageTypeFlagBit messageType,
	uint64_t messageId,
	std::string&& message,
	void* pUser) {
	// Construct a callback data
	CallbackData data{
		.errorCode = ERROR_UNDEFINED,
		.objectId = OBJ_ID_NONE,
		.pObjectName = nullptr,
		.messageId = messageId,
		.message = std::move(message)
	};
	// Pass it to the overloaded Log function.
	return Log(severity, messageType, &data, pUser);
}
