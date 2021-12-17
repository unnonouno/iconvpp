iconvpp: Simple wrapper of iconv for C++
========================================

About
-----

This is a simple wrapper of iconv for C++.


Require
-------

- libiconv (http://www.gnu.org/s/libiconv/)


Install
-------

Simply copy 'iconv.hpp' file to your project directory, or install it to your include directory.

::

 $ ./waf configure
 $ ./waf --check
 $ ./waf install


Usage
-----

::

 iconvpp::converter conv("UTF-8",   // output encoding
                         "EUC-JP",  // input encoding
                         true,      // ignore errors (optional, default: fasle)
                         1024);     // buffer size   (optional, default: 1024)
 std::string input = "...";
 std::string output;
 conv.convert(input, output);

 // or convert in-place and reuse the converter
 input = "...some str...";
 conv.try_convert(input);
 input = "...another str...";
 conv.try_convert(input);


See 'iconv_test.cpp' for more examples.


License
-------

This library is distributed under the new BSD license.
