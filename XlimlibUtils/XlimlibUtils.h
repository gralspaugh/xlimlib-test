#if defined(_WIN32)
#  if defined(EXPORTING_XLIMLIB_UTILS)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif

namespace testroutines {
    void DECLSPEC say_hello();
}