import Options

def options(ctx):
  ctx.tool_options('compiler_cxx')
  ctx.tool_options('unittest_gtest')

  ctx.add_option('--enable-gcov',
                 action='store_true',
                 default=False,
                 dest='gcov',
                 help='only for debug')


def configure(ctx):
  ctx.check_tool('compiler_cxx')
  ctx.check_tool('unittest_gtest')

  if Options.options.gcov:
    ctx.env.append_value('CXXFLAGS', '-fprofile-arcs')
    ctx.env.append_value('CXXFLAGS', '-ftest-coverage')
    ctx.env.append_value('LINKFLAGS', '-lgcov')
    ctx.env.append_value('LINKFLAGS', '-fprofile-arcs')
    ctx.env.append_value('LINKFLAGS', '-ftest-coverage')


def build(bld):
  bld.program(
    features = 'gtest',
    source = 'iconv_test.cpp',
    target = 'iconv_test',
    )

  bld.install_files('${PREFIX}/include', 'iconv.hpp')


def cpplint(ctx):
  cpplint_args = '--filter=-runtime/references,-build/include_order --extensions=cpp,hpp'

  src_dir = ctx.path.find_node('.')
  files = []
  for f in src_dir.ant_glob('*.cpp *.hpp'):
    files.append(f.path_from(ctx.path))

  args = 'cpplint.py %s %s 2>&1 | grep -v "^\(Done\|Total\)"' % (cpplint_args,' '.join(files))
  result = ctx.exec_command(args)
  if result == 0:
    ctx.fatal('cpplint failed')


def gcovr(ctx):
  excludes = [
    '.*\\.unittest-gtest.*',
    '.*_test\\.cpp'
    ]

  args = 'gcovr --branches -r . '
  for e in excludes:
    args += ' -e "%s"' % e

  ctx.exec_command(args)
