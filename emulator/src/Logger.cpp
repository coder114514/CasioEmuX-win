#include "Logger.hpp"

#include <stdarg.h>
#include <stdio.h>

namespace casioemu {
    namespace logger {
        void Info(const char *format, ...) {
            va_list args;
            va_start(args, format);
            vprintf(format, args);
            va_end(args);
        }
    }
} // namespace casioemu
