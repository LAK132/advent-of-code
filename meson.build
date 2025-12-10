project(
  'advent-of-code',
  ['c', 'cpp'],
  default_options: [
    'warning_level=3',
    'werror=true',
    'b_vscrt=static_from_buildtype',
  ],
)

lak_subprj = subproject('lak')

override_options = lak_subprj.get_variable('override_options')
c_cpp_args = lak_subprj.get_variable('c_cpp_args')
cpp_args = lak_subprj.get_variable('cpp_args')
c_args = lak_subprj.get_variable('c_args')

add_project_arguments(c_cpp_args + cpp_args, language: ['cpp'])
add_project_arguments(c_cpp_args + c_args, language: ['c'])

subdir('src')

executable(
  'aoc',
  aoc,
  override_options: override_options,
  dependencies: [
    lak_subprj.get_variable('lak_dep'),
  ],
)
