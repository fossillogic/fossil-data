dir = include_directories('.')
cc = meson.get_compiler('c')
add_project_arguments('-D_POSIX_C_SOURCE=200112L', language: 'c')
add_project_arguments('-D_POSIX_C_SOURCE=200112L', language: 'cpp')

fossil_data_lib = library('fossil_data',
    files(
        'ml.c',
        'series.c',
        'prob.c',
        'plot.c',
        'tensor.c',
        'transform.c'
    ),
    install: true,
    dependencies: [cc.find_library('m', required: false)],
    include_directories: dir)

fossil_data_dep = declare_dependency(
    link_with: [fossil_data_lib],
    include_directories: dir)

meson.override_dependency('fossil-data', fossil_data_dep)
