#include "Logger.hpp"

#include <stdarg.h>
#include <stdio.h>

#include <readline/readline.h>

namespace casioemu {
    namespace logger {
        void Info(const char *format, ...) {
            // * TODO may introduce race condition
            if (RL_ISSTATE(RL_STATE_TERMPREPPED))
                rl_clear_visible_line();
            va_list args;
            va_start(args, format);
            vprintf(format, args);
            va_end(args);
            if (RL_ISSTATE(RL_STATE_TERMPREPPED)) {
                rl_on_new_line();
                rl_redisplay();
            }
        }
    }
} // namespace casioemu
