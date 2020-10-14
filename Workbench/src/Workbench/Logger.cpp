#include "wbpch.h"
#include "Logger.h"

namespace Workbench {

	std::mutex Logger::s_write_mutex;

	Logger::Logger(Msg name) :m_name(name), m_format("%^%T%n%v%$")
	{
		m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void Logger::set_formatting(Msg format)
	{
		m_data_mutex.lock();
		m_format = format;
		m_data_mutex.unlock();
	}

	inline void Logger::log(Msg msg)
	{
		log(msg, log_level::regular);
	}

	inline void Logger::log(Msg msg, log_level level)
	{
		s_write_mutex.lock();
		for (int i = 1; i < strlen(msg); i+=2) {
			switch (msg[i]) {
			case '^': 
			{							//begin colored segment
				set_output_color(level);
				break;
			}
			case 'T':
			{

				break;
			}
			}
		}
		s_write_mutex.unlock();
	}

	inline const char* Logger::parse_date_time(char opt) {

		switch (opt) {

		}
	}

	inline void Logger::set_output_color(log_level level) {
		int color;
		switch (level) {
			case log_level::regular:
			{
				color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
				break;
			}
			case log_level::trace:
			{
				color = FOREGROUND_BLUE | FOREGROUND_GREEN;
				break;
			}
			case log_level::info:
			{
				color = FOREGROUND_BLUE;
				break;
			}
			case log_level::warn:
			{
				color = FOREGROUND_RED | FOREGROUND_GREEN;
				break;
			}
			case log_level::error:
			{
				color = FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;
			}
			default:
			{
				color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
				break;
			}
		}
		SetConsoleTextAttribute(m_console_handle, color);
	}

	inline void Logger::parse_msg(Msg msg, ArgVec arg_vec)
	{

	}

	template<typename ...Args>
	inline void Logger::log(Msg msg, Args ...args)
	{
		log(msg, log_level::regular, args...);
	}
	template<typename ...Args>
	inline void Logger::log(Msg msg, log_level level, Args ...args)
	{
		ArgVec msg_args;
		parse_args(msg_args, args...);
		parse_msg(msg, msg_args);
		log(msg, level);
	}

	template<typename ...Args>
	inline void Logger::trace(Msg msg, Args ...args)
	{
		log(msg, log_level::trace, args...);
	}

	template<typename ...Args>
	inline void Logger::info(Msg msg, Args ...args)
	{
		log(msg, log_level::info, args...);
	}

	template<typename ...Args>
	inline void Logger::warn(Msg msg, Args ...args)
	{
		log(msg, log_level::warn, args...);
	}

	template<typename ...Args>
	inline void Logger::critical(Msg msg, Args ...args)
	{
		log(msg, log_level::critical, args...);
	}

	template<typename ...Args>
	inline void Logger::error(Msg msg, Args ...args)
	{
		log(msg, log_level::error, args...);
	}

	template<typename ...Args>
	void Logger::parse_args(ArgVec& arg_vec, Args ...args)
	{
		(get_arg(arg_vec, args), ...);
	}

	template<typename T>
	void Logger::get_arg(ArgVec& arg_vec, T arg)
	{
		std::stringstream ss;
		ss << arg;
		arg_vec.push_back(ss.str());
	}
}
