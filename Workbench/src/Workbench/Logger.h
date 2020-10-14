#pragma once
#include "wbpch.h"
#include "Core.h"

namespace Workbench {
	class WORKBENCH_API Logger {
	public:
		using Msg = const char*;

		enum class log_level {
			regular = 0,
			trace,
			info,
			warn,
			error,
			critical
		};
		Logger(Msg name);

		void set_formatting(Msg format = "%^%T%n%v%$");

		void log(Msg msg);
		void log(Msg msg, log_level level);

		template<typename ...Args>
		void log(Msg msg, Args... args);

		template<typename ...Args>
		void log(Msg msg, log_level level, Args... args);

		template<typename ...Args>
		void trace(Msg msg, Args... args);

		template<typename ...Args>
		void info(Msg msg, Args... args);

		template<typename ...Args>
		void warn(Msg msg, Args... args);

		template<typename ...Args>
		void critical(Msg msg, Args... args);

		template<typename ...Args>
		void error(Msg msg, Args... args);

	protected:
		using ArgVec = std::vector<std::string>;

		void parse_msg(Msg msg, ArgVec arg_vec);

		template<typename ...Args>
		void parse_args(ArgVec& arg_vec, Args... args);

		template<typename T>
		void get_arg(ArgVec& arg_vec, T arg);

		void set_output_color(log_level level);
		const char* parse_date_time(char opt);
	protected:
		static std::mutex s_write_mutex;

		HANDLE m_console_handle;

		std::mutex m_data_mutex;
		std::string m_name;
		std::string m_format;
	};
}

