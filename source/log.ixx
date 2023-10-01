export module stk.log;

#pragma warning(push)
#pragma warning(disable: 5050) // _M_FP_PRECISE is defined in current command line and not in module command line
import std.core;
#pragma warning(pop)
import <Windows.h>;

namespace stk
{
	export template<bool Debug = true, bool Line = false, std::ostream&... ConstOstreams>
	class logger
	{
	public:
		template<typename... Args>
		logger(Args... ostreams)
			: m_ostreams{ ostreams... }
			, m_file_stream(nullptr)
		{
		}

		template<typename... Args>
		logger(char const* file, Args... ostreams) : m_ostreams{ ostreams... }
		{
			if (file != nullptr)
			{
				m_file_stream = new std::ofstream(file);
			}
		}

		~logger()
		{
			if (m_file_stream != nullptr)
			{
				delete m_file_stream;
			}
		}

		template<typename... Args>
		void operator()(std::format_string<Args...> fmt, Args&&... args) const
		{
			std::string message = std::format(fmt, std::forward<Args>(args)...);
			if constexpr (Line)
			{
				message += '\n';
			}

			if constexpr (Debug)
			{
				OutputDebugStringA(message.c_str());
			}

			((ConstOstreams << message), ...);
			for (auto& os : m_ostreams)
			{
				*os << message;
			}

			if (m_file_stream != nullptr)
			{
				*m_file_stream << message;
			}
		}

	private:
		std::vector<std::ostream*> m_ostreams;
		std::ofstream* m_file_stream;
	};

	export logger<true, false, std::cout> log("log.txt");
	export logger<true, true, std::cout> logln("log.txt");
	export logger<true, false> debug;
	export logger<true, true> debugln;
	export logger<true, false, std::cerr> error("log.txt");
	export logger<true, true, std::cerr> errorln("log.txt");
}
