def options(ctx):
  ctx.tool_options('compiler_cxx')
  ctx.tool_options('unittest_gtest')

def configure(ctx):
  ctx.check_tool('compiler_cxx')
  ctx.check_tool('unittest_gtest')

def build(bld):
  bld.program(
    features = 'gtest',
    source = 'iconv_test.cpp',
    target = 'iconv_test',
    )

  bld.install_files('${PREFIX}/include', 'iconv.hpp')
