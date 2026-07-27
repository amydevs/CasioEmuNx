#include "Logger.hpp"

#include <stdio.h>
#include <stdarg.h>

#ifndef __SWITCH__
#include <readline/readline.h>
#endif

namespace casioemu
{
	namespace logger
	{
		void Info(const char *format, ...)
		{
#ifndef __SWITCH__
			// * TODO may introduce race condition
			if (RL_ISSTATE(RL_STATE_TERMPREPPED))
				rl_clear_visible_line();
#endif
			va_list args;
			va_start(args, format);
			vprintf(format, args);
			va_end(args);
#ifndef __SWITCH__
			if (RL_ISSTATE(RL_STATE_TERMPREPPED))
			{
				rl_on_new_line();
				rl_redisplay();
			}
#endif
		}
	}
}

