#include "wbpch.h"
#include "Logger.h"

namespace Workbench {

	Logger::Logger(Msg name) :m_name(name), m_format("%^%T%n%v%$") {
		
	}

	void Logger::set_formatting(Msg format)
	{
		m_format = format;
	}

	inline void Logger::log(Msg msg)
	{
		log(msg, log_level::regular);
	}

	inline void Logger::log(Msg msg, log_level level)
	{
		
	}

	void Logger::parse_msg(Msg msg, ArgVec arg_vec)
	{
	}

	template<typename ...Args>
	inline void Logger::log(Msg msg, Args ...args)
	{
		log(msg, log_level::regular, ...args);
	}
	template<typename ...Args>
	inline void Logger::log(Msg msg, log_level level, Args ...args)
	{
		ArgVec msg_args;
		parse_args(msg_args, ...args);
		parse_msg(msg, msg_args);
		log(msg, level);
	}

	template<typename ...Args>
	inline void Logger::trace(Msg msg, Args ...args)
	{
		log(msg, log_level::trace, ...args);
	}

	template<typename ...Args>
	inline void Logger::info(Msg msg, Args ...args)
	{
		log(msg, log_level::info, ...args);
	}

	template<typename ...Args>
	inline void Logger::warn(Msg msg, Args ...args)
	{
		log(msg, log_level::warn, ...args);
	}

	template<typename ...Args>
	inline void Logger::critical(Msg msg, Args ...args)
	{
		log(msg, log_level::critical, ...args);
	}

	template<typename ...Args>
	inline void Logger::error(Msg msg, Args ...args)
	{
		log(msg, log_level::error, ...args);
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
