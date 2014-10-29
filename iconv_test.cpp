// Copyright (c) 2011, Yuya Unno

#include <string>

#include <gtest/gtest.h>
#include "iconv.hpp"

namespace iconvpp {

using std::string;

TEST(iconv, trivial) {
  converter conv("EUC-JP", "UTF-8");
  string in = "hello iconv";
  string out;
  conv.convert(in, out);
  EXPECT_EQ(in, out);

  in = "今日は天気だ";
  converter reconv("UTF-8", "EUC-JP");
  conv.convert(in, out);
  string out2;
  reconv.convert(out, out2);
  EXPECT_EQ(in, out2);
}

TEST(iconv, large) {
  converter conv("EUC-JP", "UTF-8");
  string in = string(2048, 'a');
  string out;
  conv.convert(in, out);
  EXPECT_EQ(in, out);
}

TEST(iconv, unsupported) {
  EXPECT_THROW(converter("unknown", "UTF-8"), std::runtime_error);
  EXPECT_THROW(converter("UTF-8", "unknown"), std::runtime_error);
}

TEST(iconv, invalid) {
  converter conv("UTF-8", "EUC-JP");
  string out;
  EXPECT_THROW(conv.convert("あ", out), std::runtime_error);
}

TEST(iconv, ignore_error) {
  converter conv("UTF-8", "EUC-JP", true);
  string out;
  EXPECT_NO_THROW(conv.convert("あ", out));
}

TEST(iconv, zero_buffer) {
  EXPECT_THROW(converter("UTF-8", "EUC-JP", false, 0), std::runtime_error);
}

}  // namespace iconvpp
