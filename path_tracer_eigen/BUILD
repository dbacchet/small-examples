cc_library(
    name = 'path_tracer_eigen_lib',
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
    deps = ['@eigen//:eigen'],
    visibility = ['//visibility:public'],
    copts = ['-Wno-uninitialized'],
)

cc_binary(
    name = 'path_tracer_eigen',
    srcs = ['main.cpp'],
    deps = [':path_tracer_eigen_lib',
            '//common:cmdline',
            '//common:terminal',
            ],
    visibility = ['//visibility:public'],
)
