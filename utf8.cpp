// -----------------------------------------------------------------------------
// utf8.cpp                                                             utf8.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file defines all members of the @ref utf8 namespace.
 * @author     Col. Walter E. Kurtz
 * @version    2016-10-30
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <sstream>
#include <iomanip>
#include "utf8.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Definitions                                                       Definitions
// -----------------------------------------------------------------------------
namespace utf8
{

  // ------------
  // utf82unicode
  // ------------
  /*
   *
   */
  bool utf82unicode(const string& utf8, vector<unsigned>& unicode)
  {
    // these masks cuts the lowest n bits
    static const char lowest3 =  7;  // 00000111
    static const char lowest4 = 15;  // 00001111
    static const char lowest5 = 31;  // 00011111
    static const char lowest6 = 63;  // 00111111

    // these masks cuts the highest n bits
    static const char highest1 = (-128);  // 10000000
    static const char highest2 = ( -64);  // 11000000
    static const char highest3 = ( -32);  // 11100000
    static const char highest4 = ( -16);  // 11110000
    static const char highest5 = (  -8);  // 11111000

    // reset return value
    unicode.clear();

    // the expected number of bytes to come
    unsigned expected = 0;

    // the unicode number
    unsigned code = 0;

    // analyse all characters
    for(string::size_type i = 0; i < utf8.size(); i++)
    {
      // get character
      const char& c = utf8[i];

      // no more bytes expected
      if (expected == 0)
      {
        // 0xxxxxxx
        if ((c & highest1) == 0)
        {
          unicode.push_back(static_cast<unsigned>(c));
        }

        // 110xxxxx
        else if ((c & highest3) == highest2)
        {
          // first part
          code = ((c & lowest5) << 6);

          // one more byte expected
          expected = 1;
        }

        // 1110xxxx
        else if ((c & highest4) == highest3)
        {
          // first part
          code = ((c & lowest4) << 12);

          // two more bytes expected
          expected = 2;
        }

        // 11110xxx
        else if ((c & highest5) == highest4)
        {
          // first part
          code = ((c & lowest3) << 18);

          // three more bytes expected
          expected = 3;
        }

        // invalid encoding
        else
        {
          // stop parsing
          return false;
        }
      }

      // more bytes needed to complete UTF-8 encoding
      else
      {
        // 10xxxxxx
        if ((c & highest2) == highest1)
        {
          // decrease counter
          expected -= 1;

          // add current part
          code += ((c & lowest6) << (expected * 6));

          // UTF-8 code completed
          if (expected == 0)
          {
            // add unicode number
            unicode.push_back(code);
          }
        }

        // invalid encoding
        else
        {
          // stop parsing
          return false;
        }
      }
    }

    // check final state
    return (expected == 0);
  }

  // ------------
  // unicode2utf8
  // ------------
  /*
   *
   */
  string unicode2utf8(unsigned unicode)
  {
    // unsigned masks
    static const unsigned lowest3 =  7;  // 00000000...00000111
    static const unsigned lowest4 = 15;  // 00000000...00001111
    static const unsigned lowest5 = 31;  // 00000000...00011111
    static const unsigned lowest6 = 63;  // 00000000...00111111

    // char masks
    static const unsigned char introX = 128;  // 10000000
    static const unsigned char intro2 = 192;  // 11000000
    static const unsigned char intro3 = 224;  // 11100000
    static const unsigned char intro4 = 240;  // 11110000

    string utf8;

    // plain ASCII
    if (unicode < 128)
    {
      // append 7-bit code
      utf8 += static_cast<unsigned char>(unicode);
    }

    // 11 bits are enough to encode number
    else if ((unicode >> 11) == 0)
    {
      // spread bits
      const unsigned char b0 = static_cast<unsigned char>(unicode & lowest6);
      const unsigned char b1 = static_cast<unsigned char>((unicode >> 6) & lowest5);

      // add bytes
      utf8 += (intro2 | b1);
      utf8 += (introX | b0);
    }

    // 16 bits are enough to encode number
    else if ((unicode >> 16) == 0)
    {
      // spread bits
      const unsigned char b0 = static_cast<unsigned char>(unicode & lowest6);
      const unsigned char b1 = static_cast<unsigned char>((unicode >>  6) & lowest6);
      const unsigned char b2 = static_cast<unsigned char>((unicode >> 12) & lowest4);

      // add bytes
      utf8 += (intro3 | b2);
      utf8 += (introX | b1);
      utf8 += (introX | b0);
    }

    // 21 bits are enough to encode number
    else if ((unicode >> 21) == 0)
    {
      // spread bits
      const unsigned char b0 = static_cast<unsigned char>(unicode & lowest6);
      const unsigned char b1 = static_cast<unsigned char>((unicode >>  6) & lowest6);
      const unsigned char b2 = static_cast<unsigned char>((unicode >> 12) & lowest6);
      const unsigned char b3 = static_cast<unsigned char>((unicode >> 18) & lowest3);

      // add bytes
      utf8 += (intro4 | b3);
      utf8 += (introX | b2);
      utf8 += (introX | b1);
      utf8 += (introX | b0);
    }

    // empty if number is too large
    return utf8;
  }

  // ------------
  // getCodePoint
  // ------------
  /*
   *
   */
  string getCodePoint(unsigned unicode)
  {
    stringstream converter;
    converter << "U+"
              << setw(4)
              << setfill('0')
              << uppercase
              << hex
              << unicode;

    return converter.str();
  }

  // -----------
  // getUTF8Size
  // -----------
  /*
   *
   */
  unsigned getUTF8Size(const string& utf8)
  {
    // UTF-8 flags
    static const char mask = (-64);  // 11000000
    static const char next = (-128); // 10000000

    // number of encoded characters
    unsigned n = 0;

    // analyse UTF-8 encoded string
    for(string::size_type i = 0; i < utf8.size(); i++)
    {
      // get character
      const char& c = utf8[i];

      // check two highest bits
      if ((c & mask) != next) n += 1;
    }

    // return number of encoded characters
    return n;
  }

  // ------------
  // isWhitespace
  // ------------
  /*
   *
   */
  bool isWhitespace(unsigned unicode)
  {
    switch(unicode)
    {
      case 0x0009: return true;  // HT
      case 0x000A: return true;  // LF
      case 0x000B: return true;  // VT
      case 0x000C: return true;  // FF
      case 0x000D: return true;  // CR
      case 0x0020: return true;  // SP
      case 0x0085: return true;  // next line
      case 0x00A0: return true;  // no-break space
      case 0x1680: return true;  // ogham space mark
      case 0x180E: return true;  // mongolian vowel separator
      case 0x2000: return true;  // en quad
      case 0x2001: return true;  // em quad
      case 0x2002: return true;  // en space
      case 0x2003: return true;  // em space
      case 0x2004: return true;  // three-per-em space
      case 0x2005: return true;  // four-per-em space
      case 0x2006: return true;  // six-per-em space
      case 0x2007: return true;  // figure space
      case 0x2008: return true;  // punctuation space
      case 0x2009: return true;  // thin space
      case 0x200A: return true;  // hair space
      case 0x200B: return true;  // zero width space
      case 0x200C: return true;  // zero width non-joiner
      case 0x200D: return true;  // zero width joiner
      case 0x2028: return true;  // line separator
      case 0x2029: return true;  // paragraph separator
      case 0x202F: return true;  // narrow no-break space
      case 0x205F: return true;  // medium mathematical space
      case 0x2060: return true;  // word joiner
      case 0x3000: return true;  // ideographic space
      case 0xFEFF: return true;  // zero width non-breaking space
    }

    return false;
  }

  // ----------------
  // isLatinGraphical
  // ----------------
  /*
   *
   */
  bool isLatinGraphical(unsigned unicode)
  {
    // Basic Latin
    // Latin-1 Supplement
    // Latin Extended-A
    // Latin Extended-B
    // IPA Extensions
    if (unicode <= 0x02AF)
    {
      switch (unicode)
      {
        case 0x0000: return false;
        case 0x0001: return false;
        case 0x0002: return false;
        case 0x0003: return false;
        case 0x0004: return false;
        case 0x0005: return false;
        case 0x0006: return false;
        case 0x0007: return false;
        case 0x0008: return false;
        case 0x0009: return false;  // HT
        case 0x000A: return false;  // LF
        case 0x000B: return false;
        case 0x000C: return false;
        case 0x000D: return false;  // CR
        case 0x000E: return false;
        case 0x000F: return false;
        case 0x0010: return false;
        case 0x0011: return false;
        case 0x0012: return false;
        case 0x0013: return false;
        case 0x0014: return false;
        case 0x0015: return false;
        case 0x0016: return false;
        case 0x0017: return false;
        case 0x0018: return false;
        case 0x0019: return false;
        case 0x001A: return false;
        case 0x001B: return false;
        case 0x001C: return false;
        case 0x001D: return false;
        case 0x001E: return false;
        case 0x001F: return false;
        case 0x0020: return false;  // SP
        case 0x007F: return false;  // DEL
        case 0x0080: return false;
        case 0x0081: return false;
        case 0x0082: return false;
        case 0x0083: return false;
        case 0x0084: return false;
        case 0x0085: return false;
        case 0x0086: return false;
        case 0x0087: return false;
        case 0x0088: return false;
        case 0x0089: return false;
        case 0x008A: return false;
        case 0x008B: return false;
        case 0x008C: return false;
        case 0x008D: return false;
        case 0x008E: return false;
        case 0x008F: return false;
        case 0x0090: return false;
        case 0x0091: return false;
        case 0x0092: return false;
        case 0x0093: return false;
        case 0x0094: return false;
        case 0x0095: return false;
        case 0x0096: return false;
        case 0x0097: return false;
        case 0x0098: return false;
        case 0x0099: return false;
        case 0x009A: return false;
        case 0x009B: return false;
        case 0x009C: return false;
        case 0x009D: return false;
        case 0x009E: return false;
        case 0x009F: return false;
        case 0x00A0: return false;  // non-breaking space
        case 0x00AD: return false;  // soft hyphen
      }

      // graphical
      return true;
    }

    // Latin Extended Additional
    if ((unicode >= 0x1E00) && (unicode <= 0x1EFF))
    {
      // all used and graphical
      return true;
    }

    // Superscripts and Subscripts
    if ((unicode >= 0x2070) && (unicode <= 0x209F))
    {
      // reserved
      switch (unicode)
      {
        case 0x2072: return false;
        case 0x2073: return false;
        case 0x208F: return false;
        case 0x209D: return false;
        case 0x209E: return false;
        case 0x209F: return false;
      }

      // graphical
      return true;
    }

    // Number Forms
    if ((unicode >= 0x2150) && (unicode <= 0x218F))
    {
      // reserved
      switch (unicode)
      {
        case 0x218A: return false;
        case 0x218B: return false;
        case 0x218C: return false;
        case 0x218D: return false;
        case 0x218E: return false;
        case 0x218F: return false;
      }

      // graphical
      return true;
    }

    // Latin Extended-C
    if ((unicode >= 0x2C60) && (unicode <= 0x2C7F))
    {
      // all used and graphical
      return true;
    }

    // Latin Extended-D
    if ((unicode >= 0xA720) && (unicode <= 0xA7FF))
    {
      // reserved
      switch (unicode)
      {
        case 0xA78F: return false;
        case 0xA7AE: return false;
        case 0xA7AF: return false;
        case 0xA7B2: return false;
        case 0xA7B3: return false;
        case 0xA7B4: return false;
        case 0xA7B5: return false;
        case 0xA7B6: return false;
        case 0xA7B7: return false;
        case 0xA7B8: return false;
        case 0xA7B9: return false;
        case 0xA7BA: return false;
        case 0xA7BB: return false;
        case 0xA7BC: return false;
        case 0xA7BD: return false;
        case 0xA7BE: return false;
        case 0xA7BF: return false;
        case 0xA7C0: return false;
        case 0xA7C1: return false;
        case 0xA7C2: return false;
        case 0xA7C3: return false;
        case 0xA7C4: return false;
        case 0xA7C5: return false;
        case 0xA7C6: return false;
        case 0xA7C7: return false;
        case 0xA7C8: return false;
        case 0xA7C9: return false;
        case 0xA7CA: return false;
        case 0xA7CB: return false;
        case 0xA7CC: return false;
        case 0xA7CD: return false;
        case 0xA7CE: return false;
        case 0xA7CF: return false;
        case 0xA7D0: return false;
        case 0xA7D1: return false;
        case 0xA7D2: return false;
        case 0xA7D3: return false;
        case 0xA7D4: return false;
        case 0xA7D5: return false;
        case 0xA7D6: return false;
        case 0xA7D7: return false;
        case 0xA7D8: return false;
        case 0xA7D9: return false;
        case 0xA7DA: return false;
        case 0xA7DB: return false;
        case 0xA7DC: return false;
        case 0xA7DD: return false;
        case 0xA7DE: return false;
        case 0xA7DF: return false;
        case 0xA7E0: return false;
        case 0xA7E1: return false;
        case 0xA7E2: return false;
        case 0xA7E3: return false;
        case 0xA7E4: return false;
        case 0xA7E5: return false;
        case 0xA7E6: return false;
        case 0xA7E7: return false;
        case 0xA7E8: return false;
        case 0xA7E9: return false;
        case 0xA7EA: return false;
        case 0xA7EB: return false;
        case 0xA7EC: return false;
        case 0xA7ED: return false;
        case 0xA7EE: return false;
        case 0xA7EF: return false;
        case 0xA7F0: return false;
        case 0xA7F1: return false;
        case 0xA7F2: return false;
        case 0xA7F3: return false;
        case 0xA7F4: return false;
        case 0xA7F5: return false;
        case 0xA7F6: return false;
      }

      // graphical
      return true;
    }

    // Latin Extended-E
    if ((unicode >= 0xAB30) && (unicode <= 0xAB6F))
    {
      // reserved
      switch (unicode)
      {
        case 0xAB60: return false;
        case 0xAB61: return false;
        case 0xAB62: return false;
        case 0xAB63: return false;
        case 0xAB66: return false;
        case 0xAB67: return false;
        case 0xAB68: return false;
        case 0xAB69: return false;
        case 0xAB6A: return false;
        case 0xAB6B: return false;
        case 0xAB6C: return false;
        case 0xAB6D: return false;
        case 0xAB6E: return false;
        case 0xAB6F: return false;
      }

      // graphical
      return true;
    }

    // other unicode blocks
    return false;
  }

}

