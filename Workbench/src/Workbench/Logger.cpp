#include "wbpch.h"
#include "Logger.h"

namespace Workbench {

	std::mutex Logger::s_write_mutex;

	std::shared_ptr<Logger> Logger::s_CoreLogger;
	std::shared_ptr<Logger> Logger::s_RendererLogger;
	std::shared_ptr<Logger> Logger::s_ClientLogger;

	Logger::Logger(MSG_TYPE name, log_level main_log_level) :m_name(name), m_format("%^[%T] <%n> : %v%$"), m_main_log_level(main_log_level)
	{
		m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void Logger::Init() {
		s_CoreLogger = std::make_shared<Logger>("WORKBENCH", CORE_LOG_LEVEL);
		s_RendererLogger = std::make_shared<Logger>("RENDERER", RENDERER_LOG_LEVEL);
		s_ClientLogger = std::make_shared<Logger>("APP", LOG_LEVEL);
	}

	void Logger::set_formatting(MSG_TYPE format)
	{
		m_data_mutex.lock();
		m_format = format;
		m_data_mutex.unlock();
	}

	void Logger::log(MSG_TYPE msg)
	{
		log(std::forward<std::string>(msg), log_level::regular);
	}

	void Logger::log(MSG_TYPE msg, log_level level)
	{
		if (level >= m_main_log_level) {
			const char* _format = m_format.c_str();
			s_write_mutex.lock();
			set_output_color();
			for (int i = 0; i < strlen(_format); i++) {
				if (_format[i] == '%' && i < strlen(_format)) {
					switch (_format[i + 1]) {
					case '^':
					{										//begin colored segment
						set_output_color(level);
						break;
					}
					case 'T':
					{										// HH:MM:SS
						std::cout << parse_date_time('T');
						break;
					}
					case 'n':								//logger name
					{
						std::cout << m_name;
						break;
					}
					case 'v':								//message
					{
						std::cout << msg;
						break;
					}
					case '$':
					{										//end colored segment
						set_output_color();
						break;
					}
					default:
					{
						break;
					}
					}
					i++;
				}
				else {
					std::cout << _format[i];
				}
			}
			std::cout << "\n";
			s_write_mutex.unlock();
		}
	}

	const char* Logger::parse_date_time(char opt) {
		time_t rawtime;
		struct tm timeinfo;
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		char* buff = new char[70];
		
		switch (opt) {
		case 'T':
		{
			strftime(buff, sizeof(char)*70, "%T", &timeinfo);
			return buff;
			break;
		}
		default: 
		{
			return "";
		}
		}
	}

	void Logger::set_output_color(log_level level) {
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
				color = FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY;
				break;
			}
			case log_level::warn:
			{
				color = FOREGROUND_RED | FOREGROUND_GREEN;
				break;
			}
			case log_level::error:
			{
				color = FOREGROUND_RED;
				break;
			}
			case log_level::critical:
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

	void Logger::parse_msg(std::string& msg, ArgVec arg_vec)
	{
		std::regex arg_regex("\\{\\d+\\}", std::regex_constants::ECMAScript | std::regex_constants::icase);

		auto args_begin = std::sregex_iterator(msg.begin(), msg.end(), arg_regex);
		auto args_end = std::sregex_iterator();

		std::vector<std::smatch> vec;

		int counter = 0;
		for (std::sregex_iterator i = args_begin; i != args_end; ++i) {
			vec.push_back(*i);
		}
		for (int64_t i = vec.size() - 1; i >= 0; i--) {
			auto arg_raw = vec[i].str();
			int arg_index = stoi(arg_raw.substr(1, arg_raw.length() - 2));

			if (arg_index < arg_vec.size()) {
				int64_t pos = vec[i].position();
				int64_t length = vec[i].length();
				msg.erase(pos, length);
				msg.insert(pos, arg_vec[arg_index]);
			}
		}
	}
}
