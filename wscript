def options(ctx):
  ctx.load('compiler_cxx')
  ctx.load('unittest_gtest')

def configure(ctx):
  ctx.env.CXXFLAGS += ['-O2', '-Wall', '-g', '-pipe']
  ctx.load('compiler_cxx')
  ctx.load('unittest_gtest')

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

  args = 'cpplint %s %s 2>&1 | grep -v "^\(Done\|Total\)"' % (cpplint_args,' '.join(files))
  result = ctx.exec_command(args)
  if result == 0:
    ctx.fatal('cpplint failed')
