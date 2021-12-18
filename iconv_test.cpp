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

TEST(iconv, largeGBK) {
  // '烫' in GBK and UTF-8
  std::string gbk_char ("\xCC\xCC");
  std::string utf_char ("\xE7\x83\xAB");
  std::string in;
  std::string out;
  for (int i = 0; i < 2048; ++i) {
    in.append("x").append(gbk_char).append("x");
    out.append("x").append(utf_char).append("x");
  }

  converter conv("UTF-8", "GBK");
  std::string out_cv;
  conv.convert(in, out_cv);
  EXPECT_EQ(out, out_cv);

  converter reconv("GBK", "UTF-8");
  std::string in_cv;
  reconv.convert(out, in_cv);
  EXPECT_EQ(in, in_cv);
}

TEST(iconv, multry) {
  std::string gbk_char {"\xCC\xCC"};
  std::string gbk_line = gbk_char + "--" + gbk_char + "\n";
  std::string utf_char{"\xE7\x83\xAB"};
  std::string utf_line = utf_char + "--" + utf_char + "\n";

  iconvpp::converter gbk2utf("UTF-8", "GBK", true);

  std::string input;
  std::string output;
  bool ok = false;

  for (int i = 0; i < 8; ++i) {
    input.clear();
    output.clear();
    for (int n = 0; n <= i; ++n) {
      input.append(gbk_line);
      output.append(utf_line);
    }
    ok = gbk2utf.try_convert(input);
    EXPECT_TRUE(ok);
    EXPECT_EQ(input, output);
  }

  // should fail convert as input already in utf now.
  ok = gbk2utf.try_convert(input);
  EXPECT_TRUE(!ok);
  EXPECT_EQ(input, output);
}

}  // namespace iconvpp
