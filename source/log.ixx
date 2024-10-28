export module stk.log;

import se;
import stk.hash;
import <Windows.h>;

namespace stk
{
	export template<bool Debug = true, bool Line = false, bool Disabled = false, se::ostream&... ConstOstreams>
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
				m_file_stream = new se::ofstream(file);
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
			m_tags.erase(se::remove(m_tags.begin(), m_tags.end(), tag), m_tags.end());
		}

		template<c_hash... Tags>
		void disable()
		{
			(disable(Tags), ...);
		}

		template<typename... Args>
		void operator()(se::format_string<Args...> fmt, Args&&... args) const
		{
			if constexpr (Disabled)
			{
				return;
			}

			se::string message = se::format(fmt, se::forward<Args>(args)...);
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
		void operator()(se::vector<c_hash> const&& tags, se::format_string<Args...> fmt, Args&&... args) const
		{
			if constexpr (Disabled)
			{
				return;
			}

			bool tag_found = false;
			for (auto tag : m_tags)
			{
				if (se::find(tags.begin(), tags.end(), tag) == tags.end())
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

			operator()(fmt, se::forward<Args>(args)...);
		}

	private:
		se::vector<se::ostream*> m_ostreams;
		se::ofstream* m_file_stream;
		se::vector<c_hash> m_tags;
	};

	// Disable debug logging in release builds
#if defined NDEBUG
	export c_logger<true, false, true> debug;
	export c_logger<true, true, true> debugln;
#else
	export c_logger<true, false> debug;
	export c_logger<true, true> debugln;
#endif

	export c_logger<true, false, false, se::cout> log("log.txt");
	export c_logger<true, true, false, se::cout> logln("log.txt");
	export c_logger<true, false, false, se::cerr> error("log.txt");
	export c_logger<true, true, false, se::cerr> errorln("log.txt");
}
