#ifdef _DEBUG
#define ASSERT(condition) do { \
    if (!(condition)) { \
        std::cerr << "Assertion failed: " #condition ", file " << __FILE__ << ", line " << __LINE__ << std::endl; \
        std::abort(); \
    } \
} while (0)
#else
#define ASSERT(condition) do { } while (0)
#endif // DEBUG