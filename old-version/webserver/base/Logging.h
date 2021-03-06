// Author Fangping Liu
// 2019-08-29

#ifndef WEBSERVER_BASE_LOGGING_H
#define WEBSERVER_BASE_LOGGING_H

#include "AsyncLogging.h"
#include "LogStream.h"
#include "noncopyable.h"
#include "Singleton.h"
#include <string>

namespace lfp
{
	class Logger : noncopyable
	{
	public:
		Logger(const char* file, int line, bool isAsync);
		~Logger();

		LogStream& stream() { return impl_.stream_; }

		typedef void (*OutputFunc)(const char* msg, int len);
		typedef void (*FlushFunc)();
		static void setOutput(OutputFunc);
		static void setFlush(FlushFunc);

	private:
		class Impl
		{
		public:
			Impl(const char* file, int line);
			void formatTime();
			void finish();

			LogStream stream_;
			int line_;
			std::string basename_; //这里保存的只是文件名，不含路径
		};
	private:
		Impl impl_;
		bool isAsync_;  //是否为异步日志
	};


//日志输出接口。注意这里__FILE__是文件的绝对路径
#define SYNC_LOG  Logger(__FILE__, __LINE__, false).stream() //同步日志
#define ASYNC_LOG Logger(__FILE__, __LINE__, true).stream()  //异步日志


//设置异步日志属性
#define SET_ASYNCLOG_BASENAME(name)  Singleton<AsyncLogging>::instance().setBaseName(name)
#define SET_ASYNCLOG_ROLLSIZE(size)  Singleton<AsyncLogging>::instance().setRollSize(size)
//停止异步日志线程
#define ASYNCLOG_STOP Singleton<AsyncLogging>::instance().stop()

//根据savedErrno返回错误描述
const char* strerror_tl(int savedErrno);

} //end of namespace lfp

#endif //end of WEBSERVER_BASE_LOGGING_H



/*
待改进的不完美的地方：在前端线程格式化数据到缓冲区时进行了一次内存拷贝，在将前端数据送入后端线程时又会进行一次内存拷贝
					  第一次拷贝不可避免，第二次拷贝留做优化
*/