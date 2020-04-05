cc_library(
    name = 'terminal',
    hdrs = ['include/terminal.h'],
    strip_include_prefix = 'include',
    linkstatic = True,
    visibility = ['//visibility:public'],
)

cc_binary(
    name = 'sample_terminal',
    srcs = ['src/sample_terminal_main.cpp'],
    deps = [':terminal',
            '@vmath//:vmath'],
    visibility = ['//visibility:public'],
)
