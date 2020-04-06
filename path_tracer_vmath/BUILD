cc_library(
    name = 'path_tracer_lib',
    hdrs = ['path_tracer.h',
            'objects.h',
            'pt_math.h',
            'camera.h',
            'material.h'],
    srcs = ['path_tracer.cpp',
            'material.cpp',
            'camera.cpp',
            ],
    strip_include_prefix = '.',
    linkstatic = True,
    deps = ['@vmath//:vmath'],
    visibility = ['//visibility:public'],
)

cc_binary(
    name = 'path_tracer',
    srcs = ['main.cpp'],
    deps = [':path_tracer_lib',
            '//common:cmdline',
            '//common:terminal',
            '@vmath//:vmath'],
    visibility = ['//visibility:public'],
)
