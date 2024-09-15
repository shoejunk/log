export module stk.log;

import std;
import stk.hash;
import <Windows.h>;

namespace stk
{
	export template<bool Debug = true, bool Line = false, bool Disabled = false, std::ostream&... ConstOstreams>
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
			if constexpr (Disabled)
			{
				return;
			}

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
			if constexpr (Disabled)
			{
				return;
			}

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

	// Disable debug logging in release builds
#if defined NDEBUG
	export c_logger<true, false, true> debug;
	export c_logger<true, true, true> debugln;
#else
	export c_logger<true, false> debug;
	export c_logger<true, true> debugln;
#endif

	export c_logger<true, false, false, std::cout> log("log.txt");
	export c_logger<true, true, false, std::cout> logln("log.txt");
	export c_logger<true, false, false, std::cerr> error("log.txt");
	export c_logger<true, true, false, std::cerr> errorln("log.txt");
}
