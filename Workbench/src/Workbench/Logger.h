#pragma once
#pragma warning(disable : 4251)
#include "wbpch.h"
#include "Core.h"

#define MSG_TYPE std::string&&
#define MSG_RVALUE(x) std::forward<std::string>(x)

namespace Workbench {
	template<class T>
	std::ostream& operator <<(std::ostream& os, const std::vector<T>& vec) {
		os << "{";
		for (size_t i = 0; i < vec.size(); ++i) {
			os << vec[i];
			if (i < vec.size() - 1)
				os << ", ";
		}
		os << "}\n";
		return os;
	}

	class WORKBENCH_API Logger {
	public:

		enum class log_level {
			regular = 0,
			trace,
			info,
			warn,
			error,
			critical
		};

		Logger(MSG_TYPE name, log_level main_log_level = log_level::info);

		void set_formatting(MSG_TYPE format = "%^[%T] <%n> : %v%$");

		void log(MSG_TYPE msg);
		void log(MSG_TYPE msg, log_level level);

		template<typename ...Args>
		void log(MSG_TYPE msg, log_level level, Args... args)
		{
			ArgVec msg_args;
			parse_args(msg_args, args...);
			parse_msg(msg, msg_args);
			log(MSG_RVALUE(msg), level);
		};

		template<typename ...Args>
		void log(MSG_TYPE msg, Args... args)
		{
			log(MSG_RVALUE(msg), log_level::regular, args...);
		};
		
		template<typename ...Args>
		void trace(MSG_TYPE msg, Args... args)
		{
			log(MSG_RVALUE(msg), log_level::trace, args...);
		};

		template<typename ...Args>
		void info(MSG_TYPE msg, Args ...args)
		{
			log(MSG_RVALUE(msg), log_level::info, args...);
		}

		template<typename ...Args>
		void warn(MSG_TYPE msg, Args ...args)
		{
			log(MSG_RVALUE(msg), log_level::warn, args...);
		}

		template<typename ...Args>
		void error(MSG_TYPE msg, Args... args)
		{
			log(MSG_RVALUE(msg), log_level::error, args...);
		}

		template<typename ...Args>
		void critical(MSG_TYPE msg, Args ...args)
		{
			log(MSG_RVALUE(msg), log_level::critical, args...);
		}

	protected:
		using ArgVec = std::vector<std::string>;

		void parse_msg(std::string&, ArgVec arg_vec);

		template<typename ...Args>
		void parse_args(ArgVec& arg_vec, Args... args)
		{
			(get_arg(arg_vec, args), ...);
		};

		template<typename T>
		void get_arg(ArgVec& arg_vec, T arg)
		{
			std::stringstream ss;
			ss << arg;
			arg_vec.push_back(ss.str());
		};

		void set_output_color(log_level level = log_level::regular);
		const char* parse_date_time(char opt);
	
	protected:
		static std::mutex s_write_mutex;

		HANDLE m_console_handle;

		std::mutex m_data_mutex;
		std::string m_name;
		std::string m_format;

		log_level m_main_log_level;

	//static access
	public:
		static void Init();

		static std::shared_ptr<Logger>& get_core_logger() { return s_CoreLogger; };
		static std::shared_ptr<Logger>& get_renderer_logger() { return s_RendererLogger; };
		static std::shared_ptr<Logger>& get_client_logger() { return s_ClientLogger; };

	protected:
		static std::shared_ptr<Logger> s_CoreLogger;
		static std::shared_ptr<Logger> s_RendererLogger;
		static std::shared_ptr<Logger> s_ClientLogger;
	};
}

#define LOG

#ifdef LOG
//core log macros
#define WB_CORE_LOG(...)		::Workbench::Logger::get_core_logger()->log(__VA_ARGS__)
#define WB_CORE_TRACE(...)		::Workbench::Logger::get_core_logger()->trace(__VA_ARGS__)
#define WB_CORE_INFO(...)		::Workbench::Logger::get_core_logger()->info(__VA_ARGS__)
#define WB_CORE_WARN(...)		::Workbench::Logger::get_core_logger()->warn(__VA_ARGS__)
#define WB_CORE_ERROR(...)		::Workbench::Logger::get_core_logger()->error(__VA_ARGS__)
#define WB_CORE_CRITICAL(...)	::Workbench::Logger::get_core_logger()->critical(__VA_ARGS__)

//core log macros
#define WB_RENDERER_LOG(...)		::Workbench::Logger::get_renderer_logger()->log(__VA_ARGS__)
#define WB_RENDERER_TRACE(...)		::Workbench::Logger::get_renderer_logger()->trace(__VA_ARGS__)
#define WB_RENDERER_INFO(...)		::Workbench::Logger::get_renderer_logger()->info(__VA_ARGS__)
#define WB_RENDERER_WARN(...)		::Workbench::Logger::get_renderer_logger()->warn(__VA_ARGS__)
#define WB_RENDERER_ERROR(...)		::Workbench::Logger::get_renderer_logger()->error(__VA_ARGS__)
#define WB_RENDERER_CRITICAL(...)	::Workbench::Logger::get_renderer_logger()->critical(__VA_ARGS__)

//client log macros
#define WB_LOG(...)				::Workbench::Logger::get_client_logger()->log(__VA_ARGS__)
#define WB_TRACE(...)			::Workbench::Logger::get_client_logger()->trace(__VA_ARGS__)
#define WB_INFO(...)			::Workbench::Logger::get_client_logger()->info(__VA_ARGS__)
#define WB_WARN(...)			::Workbench::Logger::get_client_logger()->warn(__VA_ARGS__)
#define WB_ERROR(...)			::Workbench::Logger::get_client_logger()->error(__VA_ARGS__)
#define WB_CRITICAL(...)		::Workbench::Logger::get_client_logger()->critical(__VA_ARGS__)

#else

//core log macros
#define WB_CORE_LOG(...)		
#define WB_CORE_TRACE(...)		
#define WB_CORE_INFO(...)		
#define WB_CORE_WARN(...)		
#define WB_CORE_ERROR(...)		
#define WB_CORE_CRITICAL(...)	

//core log macros
#define WB_RENDERER_LOG(...)	
#define WB_RENDERER_TRACE(...)	
#define WB_RENDERER_INFO(...)	
#define WB_RENDERER_WARN(...)	
#define WB_RENDERER_ERROR(...)	
#define WB_RENDERER_CRITICAL(...)

//client log macros
#define WB_LOG(...)				
#define WB_TRACE(...)			
#define WB_INFO(...)			
#define WB_WARN(...)			
#define WB_ERROR(...)			
#define WB_CRITICAL(...)	

#endif