export module stk.log;

#pragma warning(push)
#pragma warning(disable: 5050) // _M_FP_PRECISE is defined in current command line and not in module command line
import std.core;
#pragma warning(pop)
import stk.hash;
import <Windows.h>;

namespace stk
{
	export template<bool Debug = true, bool Line = false, std::ostream&... ConstOstreams>
	class c_logger
	{
	public:
		template<typename... Args>
		c_logger(Args... ostreams)
			: m_ostreams{ ostreams... }
			, m_file_stream(nullptr)
		{
		}

		template<typename... Args>
		c_logger(char const* file, Args... ostreams) : m_ostreams{ ostreams... }
		{
			if (file != nullptr)
			{
				m_file_stream = new std::ofstream(file);
			}
		}

		~c_logger()
		{
			if (m_file_stream != nullptr)
			{
				delete m_file_stream;
			}
		}

		template<c_hash... Tags>
		void enable()
		{
			(m_tags.push_back(Tags), ...);
		}

		void disable(c_hash tag)
		{
			m_tags.erase(std::remove(m_tags.begin(), m_tags.end(), tag), m_tags.end());
		}

		template<c_hash... Tags>
		void disable()
		{
			(disable(Tags), ...);
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

		template<typename... Args>
		void operator()(std::vector<c_hash> const&& tags, std::format_string<Args...> fmt, Args&&... args) const
		{
			bool tag_found = false;
			for (auto tag : m_tags)
			{
				if (std::find(tags.begin(), tags.end(), tag) == tags.end())
				{
					continue;
				}

				tag_found = true;
				break;
			}

			if (!tag_found)
			{
				return;
			}

			operator()(fmt, std::forward<Args>(args)...);
		}

	private:
		std::vector<std::ostream*> m_ostreams;
		std::ofstream* m_file_stream;
		std::vector<c_hash> m_tags;
	};

	export c_logger<true, false, std::cout> log("log.txt");
	export c_logger<true, true, std::cout> logln("log.txt");
	export c_logger<true, false> debug;
	export c_logger<true, true> debugln;
	export c_logger<true, false, std::cerr> error("log.txt");
	export c_logger<true, true, std::cerr> errorln("log.txt");
}
