// -----------------------------------------------------------------------------
// FormatHandler.cpp                                           FormatHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref FormatHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-23
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <vector>
#include <sstream>
#include "utf8.h"
#include "message.h"
#include "FormatHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// -------------
// FormatHandler
// -------------
/*
 *
 */
FormatHandler::FormatHandler(KVHandler* next)
: ChainHandler(next)
{
  // nothing
}


// ---------------------------------------------------------------------------
// Callback handler                                           Callback handler
// ---------------------------------------------------------------------------

// --------------
// OnBeginParsing
// --------------
/*
 *
 */
void FormatHandler::OnBeginParsing(const string& filename)
{
  // reset healthy flag
  setHealthy();

  // notify next handler
  if (m_next)
  {
    m_next->OnBeginParsing(filename);
  }
}

// ------
// OnData
// ------
/*
 *
 */
void FormatHandler::OnData(const string& key, const string& value)
{
  // no handler set
  if (m_next == 0) return;

  // invalid state
  if ( !healthy() ) return;

  // the formatted value
  string formatted;

  // try to run format commands
  if ( format(value, formatted) )
  {
    // notify next handler
    m_next->OnData(key, formatted);
  }

  else
  {
    // update healthy flag
    setHealthy(false);
  }
}


// -----------------------------------------------------------------------------
// Internal methods                                             Internal methods
// -----------------------------------------------------------------------------

// ------
// format
// ------
/*
 *
 */
bool FormatHandler::format(const string& text, string& formatted) const
{
  // reset return value
  formatted = "";

  // the parser's state
  enum
  {
    PLAIN,
    VERBATIM,
    QUANTIFIER,
    DELIMITER,
    ARGUMENT
  }
  state = PLAIN;

  // buffers
  string argument;
  string quantifier;
  unsigned char function = 0;
  unsigned char delimiter = 0;

  // parse given text
  for(string::size_type i = 0; i < text.size(); i++)
  {
    // get current character
    unsigned char uc = static_cast<unsigned char>(text[i]);

    // PLAIN
    if (state == PLAIN)
    {
      // escape sequence started
      if (uc == '\\')
      {
        // switch state
        state = VERBATIM;
      }

      // format command started
      else if (uc == '%')
      {
        // reset buffers
        argument   = "";
        quantifier = "";
        function   = 0;
        delimiter  = 0;

        // switch state
        state = QUANTIFIER;
      }

      // normal character found
      else
      {
        // append character
        formatted += uc;
      }
    }

    // VERBATIM
    else if (state == VERBATIM)
    {
      // append character unparsed
      formatted += uc;

      // back to PLAIN state
      state = PLAIN;
    }

    // QUANTIFIER
    else if (state == QUANTIFIER)
    {
      // digits
      if ( (uc >= '0') && (uc <= '9') )
      {
        // append digit
        quantifier += uc;
      }

      // function name
      else
      {
        // set name
        function = uc;

        // switch state
        state = DELIMITER;
      }
    }

    // DELIMITER
    else if (state == DELIMITER)
    {
      // delimitier must be a 7-bit value
      if (uc > 127)
      {
        // notify user
        msg::err( msg::catq("delimiters must be 7-bit values: ", text) );

        // signalize trouble
        return false;
      }

      // set delimiter
      delimiter = uc;

      // switch state
      state = ARGUMENT;
    }

    // ARGUMENT
    else if (state == ARGUMENT)
    {
      // argument completed
      if (uc == delimiter)
      {
        // formatted argument
        string argF;

        // format argument first (recursively)
        if ( !format(argument, argF) )
        {
          // signalize trouble
          return false;
        }

        // evaluated argument
        string argE;

        // run format command
        if ( !evaluate(function, quantifier, argF, argE) )
        {
          // signalize trouble
          return false;
        }

        // append evaluated argument
        formatted.append(argE);

        // back to initial state
        state = PLAIN;
      }

      else
      {
        // append character
        argument += uc;
      }
    }
  }

  // check final state
  if (state != PLAIN)
  {
    // notify user
    msg::err( msg::catq("invalid syntax: ", text) );

    // signalize trouble
    return false;
  }

  // signalize success
  return true;
}

// --------
// evaluate
// --------
/*
 *
 */
bool FormatHandler::evaluate( unsigned char  function,
                              const string&  quantifier,
                              const string&  argument,
                              string&        result
                            ) const
{
  // reset result
  result = "";

  // set default value
  unsigned number = 0;

  // check if quantifier is given
  if ( !quantifier.empty() )
  {
    // convert string to unsigned
    if ( !(stringstream(quantifier) >> number) )
    {
      // notify user
      msg::err( msg::catq("invalid number: ", quantifier) );

      // signalize trouble
      return false;
    }
  }

  // known format commands
  if (function == 'z') return fmtLeadingZeros   (number, argument, result);
  if (function == 'Z') return fmtTrailingZeros  (number, argument, result);
  if (function == 'c') return fmtCommaSplit     (number, argument, result);
  if (function == 's') return fmtSemicolonSplit (number, argument, result);
  if (function == 't') return fmtTrim           (number, argument, result);
  if (function == 'q') return fmtSqueeze        (number, argument, result);
  if (function == 'f') return fmtFilename       (number, argument, result);

  // get name as string
  string cmd("%");
  cmd += function;

  // notify user
  msg::err( msg::catq("unknown command: ", cmd) );

  // signalize trouble
  return false;
}

// ---------------
// fmtLeadingZeros
// ---------------
/*
 *
 */
bool FormatHandler::fmtLeadingZeros(unsigned number, const string& in, string& out) const
{
  // zeros to insert
  string zeros = "";

  // check if zeros are needed
  if (in.size() < number)
  {
    zeros = string(number - in.size(), '0');
  }

  // insert leading zeros
  out = zeros + in;

  // signalize success
  return true;
}

// ----------------
// fmtTrailingZeros
// ----------------
/*
 *
 */
bool FormatHandler::fmtTrailingZeros(unsigned number, const string& in, string& out) const
{
  // zeros to insert
  string zeros = "";

  // check if zeros are needed
  if (in.size() < number)
  {
    zeros = string(number - in.size(), '0');
  }

  // insert trailing zeros
  out = in + zeros;

  // signalize success
  return true;
}

// -------------
// fmtCommaSplit
// -------------
/*
 *
 */
bool FormatHandler::fmtCommaSplit(unsigned number, const string& in, string& out) const
{
  // buffer
  string buffer;

  // number of extracted buffers
  unsigned bcount = 0;

  // split argument
  for(string::size_type i = 0; i < in.size(); i++)
  {
    // get current character
    unsigned char uc = static_cast<unsigned char>(in[i]);

    // separator found
    if (uc == ',')
    {
      // increase counter
      bcount += 1;

      // specified buffer found
      if (bcount == number)
      {
        // return this buffer
        out = buffer;

        // exit loop
        break;
      }

      // reset buffer
      buffer = "";
    }

    else
    {
      // append character
      buffer += uc;
    }
  }

  // check counter
  if ((bcount > 0) && ((bcount + 1) == number))
  {
    // return this buffer
    out = buffer;
  }

  // signalize success
  return true;
}

// -----------------
// fmtSemicolonSplit
// -----------------
/*
 *
 */
bool FormatHandler::fmtSemicolonSplit(unsigned number, const string& in, string& out) const
{
  // buffer
  string buffer;

  // number of extracted buffers
  unsigned bcount = 0;

  // split argument
  for(string::size_type i = 0; i < in.size(); i++)
  {
    // get current character
    unsigned char uc = static_cast<unsigned char>(in[i]);

    // separator found
    if (uc == ';')
    {
      // increase counter
      bcount += 1;

      // specified buffer found
      if (bcount == number)
      {
        // return this buffer
        out = buffer;

        // exit loop
        break;
      }

      // reset buffer
      buffer = "";
    }

    else
    {
      // append character
      buffer += uc;
    }
  }

  // check counter
  if ((bcount > 0) && ((bcount + 1) == number))
  {
    // return this buffer
    out = buffer;
  }

  // signalize success
  return true;
}

// -------
// fmtTrim
// -------
/*
 *
 */
bool FormatHandler::fmtTrim(unsigned number, const string& in, string& out) const
{
  // buffer
  string blanks;

  // skip leading and trailing blanks
  for(string::size_type i = 0; i < in.size(); i++)
  {
    // get current character
    unsigned char uc = static_cast<unsigned char>(in[i]);

    // white space
    if ((uc == 9) || (uc == 10) || (uc == 13) || (uc == 32))
    {
      // buffer character
      blanks += uc;
    }

    else
    {
      // add buffered blanks first
      if ( !blanks.empty() )
      {
        // don't start with blanks
        if ( !out.empty() )
        {
          out.append(blanks);
        }

        // reset buffer
        blanks = "";
      }

      // append character
      out += uc;
    }
  }

  // signalize success
  return true;
}

// ----------
// fmtSqueeze
// ----------
/*
 *
 */
bool FormatHandler::fmtSqueeze(unsigned number, const string& in, string& out) const
{
  // buffer
  bool blank = false;

  // skip leading and trailing blanks
  for(string::size_type i = 0; i < in.size(); i++)
  {
    // get current character
    unsigned char uc = static_cast<unsigned char>(in[i]);

    // white space
    if ((uc == 9) || (uc == 10) || (uc == 13) || (uc == 32))
    {
      // buffer character
      blank = true;
    }

    else
    {
      // add buffered blank first
      if ( blank )
      {
        // don't start with blanks
        if ( !out.empty() )
        {
          out += ' ';
        }

        // reset buffer
        blank = false;
      }

      // append character
      out += uc;
    }
  }

  // signalize success
  return true;
}

// -----------
// fmtFilename
// -----------
/*
 *
 */
bool FormatHandler::fmtFilename(unsigned number, const string& in, string& out) const
{
  vector<unsigned> unicode;

  // get unicode numbers
  if ( !utf8::utf82unicode(in, unicode) )
  {
    // notify user
    msg::err( msg::catq("invalid UTF-8 encoding", in) );

    // signalize trouble
    return false;
  }

  // ascii version
  string plain;

  // get filesystem compliant characters
  for(unsigned i = 0; i < unicode.size(); i++)
  {
    // name current number
    const unsigned& u = unicode[i];

    switch (u)
    {
      // Basic Latin (U+0000 to U+007F)
      case '$': plain.append("usd"); continue;
      case '0': plain.append("0");   continue;
      case '1': plain.append("1");   continue;
      case '2': plain.append("2");   continue;
      case '3': plain.append("3");   continue;
      case '4': plain.append("4");   continue;
      case '5': plain.append("5");   continue;
      case '6': plain.append("6");   continue;
      case '7': plain.append("7");   continue;
      case '8': plain.append("8");   continue;
      case '9': plain.append("9");   continue;
      case 'A': plain.append("a");   continue;
      case 'B': plain.append("b");   continue;
      case 'C': plain.append("c");   continue;
      case 'D': plain.append("d");   continue;
      case 'E': plain.append("e");   continue;
      case 'F': plain.append("f");   continue;
      case 'G': plain.append("g");   continue;
      case 'H': plain.append("h");   continue;
      case 'I': plain.append("i");   continue;
      case 'J': plain.append("j");   continue;
      case 'K': plain.append("k");   continue;
      case 'L': plain.append("l");   continue;
      case 'M': plain.append("m");   continue;
      case 'N': plain.append("n");   continue;
      case 'O': plain.append("o");   continue;
      case 'P': plain.append("p");   continue;
      case 'Q': plain.append("q");   continue;
      case 'R': plain.append("r");   continue;
      case 'S': plain.append("s");   continue;
      case 'T': plain.append("t");   continue;
      case 'U': plain.append("u");   continue;
      case 'V': plain.append("v");   continue;
      case 'W': plain.append("w");   continue;
      case 'X': plain.append("x");   continue;
      case 'Y': plain.append("y");   continue;
      case 'Z': plain.append("z");   continue;
      case 'a': plain.append("a");   continue;
      case 'b': plain.append("b");   continue;
      case 'c': plain.append("c");   continue;
      case 'd': plain.append("d");   continue;
      case 'e': plain.append("e");   continue;
      case 'f': plain.append("f");   continue;
      case 'g': plain.append("g");   continue;
      case 'h': plain.append("h");   continue;
      case 'i': plain.append("i");   continue;
      case 'j': plain.append("j");   continue;
      case 'k': plain.append("k");   continue;
      case 'l': plain.append("l");   continue;
      case 'm': plain.append("m");   continue;
      case 'n': plain.append("n");   continue;
      case 'o': plain.append("o");   continue;
      case 'p': plain.append("p");   continue;
      case 'q': plain.append("q");   continue;
      case 'r': plain.append("r");   continue;
      case 's': plain.append("s");   continue;
      case 't': plain.append("t");   continue;
      case 'u': plain.append("u");   continue;
      case 'v': plain.append("v");   continue;
      case 'w': plain.append("w");   continue;
      case 'x': plain.append("x");   continue;
      case 'y': plain.append("y");   continue;
      case 'z': plain.append("z");   continue;

      // Latin-1 Supplement (U+0080 to U+00FF)
      case 0x00a2: plain.append("ct");  continue; //   162: ¢
      case 0x00a3: plain.append("gbp"); continue; //   163: £
      case 0x00a5: plain.append("jpy"); continue; //   165: ¥
      case 0x00a9: plain.append("c");   continue; //   169: ©
      case 0x00aa: plain.append("a");   continue; //   170: ª
      case 0x00ae: plain.append("r");   continue; //   174: ®
      case 0x00b2: plain.append("2");   continue; //   178: ²
      case 0x00b3: plain.append("3");   continue; //   179: ³
      case 0x00b5: plain.append("mu");  continue; //   181: µ
      case 0x00b9: plain.append("1");   continue; //   185: ¹
      case 0x00ba: plain.append("o");   continue; //   186: º
      case 0x00c0: plain.append("a");   continue; //   192: À
      case 0x00c1: plain.append("a");   continue; //   193: Á
      case 0x00c2: plain.append("a");   continue; //   194: Â
      case 0x00c3: plain.append("a");   continue; //   195: Ã
      case 0x00c4: plain.append("ae");  continue; //   196: Ä
      case 0x00c5: plain.append("a");   continue; //   197: Å
      case 0x00c6: plain.append("ae");  continue; //   198: Æ
      case 0x00c7: plain.append("c");   continue; //   199: Ç
      case 0x00c8: plain.append("e");   continue; //   200: È
      case 0x00c9: plain.append("e");   continue; //   201: É
      case 0x00ca: plain.append("e");   continue; //   202: Ê
      case 0x00cb: plain.append("e");   continue; //   203: Ë
      case 0x00cc: plain.append("i");   continue; //   204: Ì
      case 0x00cd: plain.append("i");   continue; //   205: Í
      case 0x00ce: plain.append("i");   continue; //   206: Î
      case 0x00cf: plain.append("i");   continue; //   207: Ï
      case 0x00d0: plain.append("dh");  continue; //   208: Ð
      case 0x00d1: plain.append("n");   continue; //   209: Ñ
      case 0x00d2: plain.append("o");   continue; //   210: Ò
      case 0x00d3: plain.append("o");   continue; //   211: Ó
      case 0x00d4: plain.append("o");   continue; //   212: Ô
      case 0x00d5: plain.append("o");   continue; //   213: Õ
      case 0x00d6: plain.append("oe");  continue; //   214: Ö
      case 0x00d8: plain.append("oe");  continue; //   216: Ø
      case 0x00d9: plain.append("u");   continue; //   217: Ù
      case 0x00da: plain.append("u");   continue; //   218: Ú
      case 0x00db: plain.append("u");   continue; //   219: Û
      case 0x00dc: plain.append("ue");  continue; //   220: Ü
      case 0x00dd: plain.append("y");   continue; //   221: Ý
      case 0x00de: plain.append("th");  continue; //   222: Þ
      case 0x00df: plain.append("ss");  continue; //   223: ß
      case 0x00e0: plain.append("a");   continue; //   224: à
      case 0x00e1: plain.append("a");   continue; //   225: á
      case 0x00e2: plain.append("a");   continue; //   226: â
      case 0x00e3: plain.append("a");   continue; //   227: ã
      case 0x00e4: plain.append("ae");  continue; //   228: ä
      case 0x00e5: plain.append("a");   continue; //   229: å
      case 0x00e6: plain.append("ae");  continue; //   230: æ
      case 0x00e7: plain.append("c");   continue; //   231: ç
      case 0x00e8: plain.append("e");   continue; //   232: è
      case 0x00e9: plain.append("e");   continue; //   233: é
      case 0x00ea: plain.append("e");   continue; //   234: ê
      case 0x00eb: plain.append("e");   continue; //   235: ë
      case 0x00ec: plain.append("i");   continue; //   236: ì
      case 0x00ed: plain.append("i");   continue; //   237: í
      case 0x00ee: plain.append("i");   continue; //   238: î
      case 0x00ef: plain.append("i");   continue; //   239: ï
      case 0x00f0: plain.append("dh");  continue; //   240: ð
      case 0x00f1: plain.append("n");   continue; //   241: ñ
      case 0x00f2: plain.append("o");   continue; //   242: ò
      case 0x00f3: plain.append("o");   continue; //   243: ó
      case 0x00f4: plain.append("o");   continue; //   244: ô
      case 0x00f5: plain.append("o");   continue; //   245: õ
      case 0x00f6: plain.append("oe");  continue; //   246: ö
      case 0x00f8: plain.append("oe");  continue; //   248: ø
      case 0x00f9: plain.append("u");   continue; //   249: ù
      case 0x00fa: plain.append("u");   continue; //   250: ú
      case 0x00fb: plain.append("u");   continue; //   251: û
      case 0x00fc: plain.append("ue");  continue; //   252: ü
      case 0x00fd: plain.append("y");   continue; //   253: ý
      case 0x00fe: plain.append("th");  continue; //   254: þ
      case 0x00ff: plain.append("y");   continue; //   255: ÿ

      // Latin Extended-A
      case 0x0100: plain.append("a");  continue; //   256: Ā
      case 0x0101: plain.append("a");  continue; //   257: ā
      case 0x0102: plain.append("a");  continue; //   258: Ă
      case 0x0103: plain.append("a");  continue; //   259: ă
      case 0x0104: plain.append("a");  continue; //   260: Ą
      case 0x0105: plain.append("a");  continue; //   261: ą
      case 0x0106: plain.append("c");  continue; //   262: Ć
      case 0x0107: plain.append("c");  continue; //   263: ć
      case 0x0108: plain.append("c");  continue; //   264: Ĉ
      case 0x0109: plain.append("c");  continue; //   265: ĉ
      case 0x010a: plain.append("c");  continue; //   266: Ċ
      case 0x010b: plain.append("c");  continue; //   267: ċ
      case 0x010c: plain.append("c");  continue; //   268: Č
      case 0x010d: plain.append("c");  continue; //   269: č
      case 0x010e: plain.append("d");  continue; //   270: Ď
      case 0x010f: plain.append("d");  continue; //   271: ď
      case 0x0110: plain.append("d");  continue; //   272: Đ
      case 0x0111: plain.append("d");  continue; //   273: đ
      case 0x0112: plain.append("e");  continue; //   274: Ē
      case 0x0113: plain.append("e");  continue; //   275: ē
      case 0x0114: plain.append("e");  continue; //   276: Ĕ
      case 0x0115: plain.append("e");  continue; //   277: ĕ
      case 0x0116: plain.append("e");  continue; //   278: Ė
      case 0x0117: plain.append("e");  continue; //   279: ė
      case 0x0118: plain.append("e");  continue; //   280: Ę
      case 0x0119: plain.append("e");  continue; //   281: ę
      case 0x011a: plain.append("e");  continue; //   282: Ě
      case 0x011b: plain.append("e");  continue; //   283: ě
      case 0x011c: plain.append("g");  continue; //   284: Ĝ
      case 0x011d: plain.append("g");  continue; //   285: ĝ
      case 0x011e: plain.append("g");  continue; //   286: Ğ
      case 0x011f: plain.append("g");  continue; //   287: ğ
      case 0x0120: plain.append("g");  continue; //   288: Ġ
      case 0x0121: plain.append("g");  continue; //   289: ġ
      case 0x0122: plain.append("g");  continue; //   290: Ģ
      case 0x0123: plain.append("g");  continue; //   291: ģ
      case 0x0124: plain.append("h");  continue; //   292: Ĥ
      case 0x0125: plain.append("h");  continue; //   293: ĥ
      case 0x0126: plain.append("h");  continue; //   294: Ħ
      case 0x0127: plain.append("h");  continue; //   295: ħ
      case 0x0128: plain.append("i");  continue; //   296: Ĩ
      case 0x0129: plain.append("i");  continue; //   297: ĩ
      case 0x012a: plain.append("i");  continue; //   298: Ī
      case 0x012b: plain.append("i");  continue; //   299: ī
      case 0x012c: plain.append("i");  continue; //   300: Ĭ
      case 0x012d: plain.append("i");  continue; //   301: ĭ
      case 0x012e: plain.append("i");  continue; //   302: Į
      case 0x012f: plain.append("i");  continue; //   303: į
      case 0x0130: plain.append("i");  continue; //   304: İ
      case 0x0131: plain.append("i");  continue; //   305: ı
      case 0x0132: plain.append("ij"); continue; //   306: Ĳ
      case 0x0133: plain.append("ij"); continue; //   307: ĳ
      case 0x0134: plain.append("j");  continue; //   308: Ĵ
      case 0x0135: plain.append("j");  continue; //   309: ĵ
      case 0x0136: plain.append("k");  continue; //   310: Ķ
      case 0x0137: plain.append("k");  continue; //   311: ķ
      case 0x0138: plain.append("k");  continue; //   312: ĸ
      case 0x0139: plain.append("l");  continue; //   313: Ĺ
      case 0x013a: plain.append("l");  continue; //   314: ĺ
      case 0x013b: plain.append("l");  continue; //   315: Ļ
      case 0x013c: plain.append("l");  continue; //   316: ļ
      case 0x013d: plain.append("l");  continue; //   317: Ľ
      case 0x013e: plain.append("l");  continue; //   318: ľ
      case 0x013f: plain.append("l");  continue; //   319: Ŀ
      case 0x0140: plain.append("l");  continue; //   320: ŀ
      case 0x0141: plain.append("l");  continue; //   321: Ł
      case 0x0142: plain.append("l");  continue; //   322: ł
      case 0x0143: plain.append("n");  continue; //   323: Ń
      case 0x0144: plain.append("n");  continue; //   324: ń
      case 0x0145: plain.append("n");  continue; //   325: Ņ
      case 0x0146: plain.append("n");  continue; //   326: ņ
      case 0x0147: plain.append("n");  continue; //   327: Ň
      case 0x0148: plain.append("n");  continue; //   328: ň
      case 0x0149: plain.append("n");  continue; //   329: ŉ
      case 0x014a: plain.append("ng"); continue; //   330: Ŋ
      case 0x014b: plain.append("ng"); continue; //   331: ŋ
      case 0x014c: plain.append("o");  continue; //   332: Ō
      case 0x014d: plain.append("o");  continue; //   333: ō
      case 0x014e: plain.append("o");  continue; //   334: Ŏ
      case 0x014f: plain.append("o");  continue; //   335: ŏ
      case 0x0150: plain.append("oe"); continue; //   336: Ő
      case 0x0151: plain.append("oe"); continue; //   337: ő
      case 0x0152: plain.append("oe"); continue; //   338: Œ
      case 0x0153: plain.append("oe"); continue; //   339: œ
      case 0x0154: plain.append("r");  continue; //   340: Ŕ
      case 0x0155: plain.append("r");  continue; //   341: ŕ
      case 0x0156: plain.append("r");  continue; //   342: Ŗ
      case 0x0157: plain.append("r");  continue; //   343: ŗ
      case 0x0158: plain.append("r");  continue; //   344: Ř
      case 0x0159: plain.append("r");  continue; //   345: ř
      case 0x015a: plain.append("s");  continue; //   346: Ś
      case 0x015b: plain.append("s");  continue; //   347: ś
      case 0x015c: plain.append("s");  continue; //   348: Ŝ
      case 0x015d: plain.append("s");  continue; //   349: ŝ
      case 0x015e: plain.append("s");  continue; //   350: Ş
      case 0x015f: plain.append("s");  continue; //   351: ş
      case 0x0160: plain.append("s");  continue; //   352: Š
      case 0x0161: plain.append("s");  continue; //   353: š
      case 0x0162: plain.append("t");  continue; //   354: Ţ
      case 0x0163: plain.append("t");  continue; //   355: ţ
      case 0x0164: plain.append("t");  continue; //   356: Ť
      case 0x0165: plain.append("t");  continue; //   357: ť
      case 0x0166: plain.append("t");  continue; //   358: Ŧ
      case 0x0167: plain.append("t");  continue; //   359: ŧ
      case 0x0168: plain.append("u");  continue; //   360: Ũ
      case 0x0169: plain.append("u");  continue; //   361: ũ
      case 0x016a: plain.append("u");  continue; //   362: Ū
      case 0x016b: plain.append("u");  continue; //   363: ū
      case 0x016c: plain.append("u");  continue; //   364: Ŭ
      case 0x016d: plain.append("u");  continue; //   365: ŭ
      case 0x016e: plain.append("u");  continue; //   366: Ů
      case 0x016f: plain.append("u");  continue; //   367: ů
      case 0x0170: plain.append("ue"); continue; //   368: Ű
      case 0x0171: plain.append("ue"); continue; //   369: ű
      case 0x0172: plain.append("u");  continue; //   370: Ų
      case 0x0173: plain.append("u");  continue; //   371: ų
      case 0x0174: plain.append("w");  continue; //   372: Ŵ
      case 0x0175: plain.append("w");  continue; //   373: ŵ
      case 0x0176: plain.append("y");  continue; //   374: Ŷ
      case 0x0177: plain.append("y");  continue; //   375: ŷ
      case 0x0178: plain.append("y");  continue; //   376: Ÿ
      case 0x0179: plain.append("z");  continue; //   377: Ź
      case 0x017a: plain.append("z");  continue; //   378: ź
      case 0x017b: plain.append("z");  continue; //   379: Ż
      case 0x017c: plain.append("z");  continue; //   380: ż
      case 0x017d: plain.append("z");  continue; //   381: Ž
      case 0x017e: plain.append("r");  continue; //   382: ž
      case 0x017f: plain.append("s");  continue; //   383: ſ

      // Latin Extended-B
      case 0x0180: plain.append("b");  continue; //   384: ƀ
      case 0x0181: plain.append("b");  continue; //   385: Ɓ
      case 0x0182: plain.append("b");  continue; //   386: Ƃ
      case 0x0183: plain.append("b");  continue; //   387: ƃ
      case 0x0184: plain.append("h");  continue; //   388: Ƅ
      case 0x0185: plain.append("h");  continue; //   389: ƅ
      case 0x0186: plain.append("o");  continue; //   390: Ɔ
      case 0x0187: plain.append("c");  continue; //   391: Ƈ
      case 0x0188: plain.append("c");  continue; //   392: ƈ
      case 0x0189: plain.append("d");  continue; //   393: Ɖ
      case 0x018a: plain.append("d");  continue; //   394: Ɗ
      case 0x018b: plain.append("d");  continue; //   395: Ƌ
      case 0x018c: plain.append("d");  continue; //   396: ƌ
      case 0x018d: plain.append("s");  continue; //   397: ƍ
      case 0x018e: plain.append("e");  continue; //   398: Ǝ
      case 0x018f: plain.append("e");  continue; //   399: Ə
      case 0x0190: plain.append("e");  continue; //   400: Ɛ
      case 0x0191: plain.append("f");  continue; //   401: Ƒ
      case 0x0192: plain.append("f");  continue; //   402: ƒ
      case 0x0193: plain.append("g");  continue; //   403: Ɠ
      case 0x0194: plain.append("g");  continue; //   404: Ɣ
      case 0x0195: plain.append("hv"); continue; //   405: ƕ
      case 0x0196: plain.append("i");  continue; //   406: Ɩ
      case 0x0197: plain.append("i");  continue; //   407: Ɨ
      case 0x0198: plain.append("k");  continue; //   408: Ƙ
      case 0x0199: plain.append("k");  continue; //   409: ƙ
      case 0x019a: plain.append("l");  continue; //   410: ƚ
      case 0x019b: plain.append("l");  continue; //   411: ƛ
      case 0x019c: plain.append("m");  continue; //   412: Ɯ
      case 0x019d: plain.append("n");  continue; //   413: Ɲ
      case 0x019e: plain.append("n");  continue; //   414: ƞ
      case 0x019f: plain.append("o");  continue; //   415: Ɵ
      case 0x01a0: plain.append("o");  continue; //   416: Ơ
      case 0x01a1: plain.append("o");  continue; //   417: ơ
      case 0x01a2: plain.append("g");  continue; //   418: Ƣ
      case 0x01a3: plain.append("g");  continue; //   419: ƣ
      case 0x01a4: plain.append("p");  continue; //   420: Ƥ
      case 0x01a5: plain.append("p");  continue; //   421: ƥ
      case 0x01a6: plain.append("r");  continue; //   422: Ʀ
      case 0x01a7: plain.append("s");  continue; //   423: Ƨ
      case 0x01a8: plain.append("s");  continue; //   424: ƨ
      case 0x01a9: plain.append("s");  continue; //   425: Ʃ
      case 0x01aa: plain.append("t");  continue; //   426: ƪ
      case 0x01ab: plain.append("t");  continue; //   427: ƫ
      case 0x01ac: plain.append("t");  continue; //   428: Ƭ
      case 0x01ad: plain.append("t");  continue; //   429: ƭ
      case 0x01ae: plain.append("t");  continue; //   430: Ʈ
      case 0x01af: plain.append("u");  continue; //   431: Ư
      case 0x01b0: plain.append("u");  continue; //   432: ư
      case 0x01b1: plain.append("u");  continue; //   433: Ʊ
      case 0x01b2: plain.append("v");  continue; //   434: Ʋ
      case 0x01b3: plain.append("y");  continue; //   435: Ƴ
      case 0x01b4: plain.append("y");  continue; //   436: ƴ
      case 0x01b5: plain.append("z");  continue; //   437: Ƶ
      case 0x01b6: plain.append("z");  continue; //   438: ƶ
      case 0x01b7: plain.append("z");  continue; //   439: Ʒ
      case 0x01b8: plain.append("z");  continue; //   440: Ƹ
      case 0x01b9: plain.append("z");  continue; //   441: ƹ
      case 0x01ba: plain.append("z");  continue; //   442: ƺ
      case 0x01bb: plain.append("z");  continue; //   443: ƻ
      case 0x01bc: plain.append("q");  continue; //   444: Ƽ
      case 0x01bd: plain.append("q");  continue; //   445: ƽ
      case 0x01bf: plain.append("w");  continue; //   447: ƿ
      case 0x01c4: plain.append("dz"); continue; //   452: Ǆ
      case 0x01c5: plain.append("dz"); continue; //   453: ǅ
      case 0x01c6: plain.append("dz"); continue; //   454: ǆ
      case 0x01c7: plain.append("lj"); continue; //   455: Ǉ
      case 0x01c8: plain.append("lj"); continue; //   456: ǈ
      case 0x01c9: plain.append("lj"); continue; //   457: ǉ
      case 0x01ca: plain.append("nj"); continue; //   458: Ǌ
      case 0x01cb: plain.append("nj"); continue; //   459: ǋ
      case 0x01cc: plain.append("nj"); continue; //   460: ǌ
      case 0x01cd: plain.append("a");  continue; //   461: Ǎ
      case 0x01ce: plain.append("a");  continue; //   462: ǎ
      case 0x01cf: plain.append("i");  continue; //   463: Ǐ
      case 0x01d0: plain.append("i");  continue; //   464: ǐ
      case 0x01d1: plain.append("o");  continue; //   465: Ǒ
      case 0x01d2: plain.append("o");  continue; //   466: ǒ
      case 0x01d3: plain.append("u");  continue; //   467: Ǔ
      case 0x01d4: plain.append("u");  continue; //   468: ǔ
      case 0x01d5: plain.append("ue"); continue; //   469: Ǖ
      case 0x01d6: plain.append("ue"); continue; //   470: ǖ
      case 0x01d7: plain.append("ue"); continue; //   471: Ǘ
      case 0x01d8: plain.append("ue"); continue; //   472: ǘ
      case 0x01d9: plain.append("ue"); continue; //   473: Ǚ
      case 0x01da: plain.append("ue"); continue; //   474: ǚ
      case 0x01db: plain.append("ue"); continue; //   475: Ǜ
      case 0x01dc: plain.append("ue"); continue; //   476: ǜ
      case 0x01dd: plain.append("e");  continue; //   477: ǝ
      case 0x01de: plain.append("ae"); continue; //   478: Ǟ
      case 0x01df: plain.append("ae"); continue; //   479: ǟ
      case 0x01e0: plain.append("ae"); continue; //   480: Ǡ
      case 0x01e1: plain.append("ae"); continue; //   481: ǡ
      case 0x01e2: plain.append("ae"); continue; //   482: Ǣ
      case 0x01e3: plain.append("ae"); continue; //   483: ǣ
      case 0x01e4: plain.append("g");  continue; //   484: Ǥ
      case 0x01e5: plain.append("g");  continue; //   485: ǥ
      case 0x01e6: plain.append("g");  continue; //   486: Ǧ
      case 0x01e7: plain.append("g");  continue; //   487: ǧ
      case 0x01e8: plain.append("k");  continue; //   488: Ǩ
      case 0x01e9: plain.append("k");  continue; //   489: ǩ
      case 0x01ea: plain.append("o");  continue; //   490: Ǫ
      case 0x01eb: plain.append("o");  continue; //   491: ǫ
      case 0x01ec: plain.append("o");  continue; //   492: Ǭ
      case 0x01ed: plain.append("o");  continue; //   493: ǭ
      case 0x01ee: plain.append("z");  continue; //   494: Ǯ
      case 0x01ef: plain.append("z");  continue; //   495: ǯ
      case 0x01f0: plain.append("j");  continue; //   496: ǰ
      case 0x01f1: plain.append("dz"); continue; //   497: Ǳ
      case 0x01f2: plain.append("dz"); continue; //   498: ǲ
      case 0x01f3: plain.append("dz"); continue; //   499: ǳ
      case 0x01f4: plain.append("g");  continue; //   500: Ǵ
      case 0x01f5: plain.append("g");  continue; //   501: ǵ
      case 0x01f6: plain.append("hv"); continue; //   502: Ƕ
      case 0x01f7: plain.append("w");  continue; //   503: Ƿ
      case 0x01f8: plain.append("n");  continue; //   504: Ǹ
      case 0x01f9: plain.append("n");  continue; //   505: ǹ
      case 0x01fa: plain.append("a");  continue; //   506: Ǻ
      case 0x01fb: plain.append("a");  continue; //   507: ǻ
      case 0x01fc: plain.append("ae"); continue; //   508: Ǽ
      case 0x01fd: plain.append("ae"); continue; //   509: ǽ
      case 0x01fe: plain.append("oe"); continue; //   510: Ǿ
      case 0x01ff: plain.append("oe"); continue; //   511: ǿ
      case 0x0200: plain.append("ae"); continue; //   512: Ȁ
      case 0x0201: plain.append("ae"); continue; //   513: ȁ
      case 0x0202: plain.append("a");  continue; //   514: Ȃ
      case 0x0203: plain.append("a");  continue; //   515: ȃ
      case 0x0204: plain.append("e");  continue; //   516: Ȅ
      case 0x0205: plain.append("e");  continue; //   517: ȅ
      case 0x0206: plain.append("e");  continue; //   518: Ȇ
      case 0x0207: plain.append("e");  continue; //   519: ȇ
      case 0x0208: plain.append("i");  continue; //   520: Ȉ
      case 0x0209: plain.append("i");  continue; //   521: ȉ
      case 0x020a: plain.append("i");  continue; //   522: Ȋ
      case 0x020b: plain.append("i");  continue; //   523: ȋ
      case 0x020c: plain.append("oe"); continue; //   524: Ȍ
      case 0x020d: plain.append("oe"); continue; //   525: ȍ
      case 0x020e: plain.append("o");  continue; //   526: Ȏ
      case 0x020f: plain.append("o");  continue; //   527: ȏ
      case 0x0210: plain.append("r");  continue; //   528: Ȑ
      case 0x0211: plain.append("r");  continue; //   529: ȑ
      case 0x0212: plain.append("r");  continue; //   530: Ȓ
      case 0x0213: plain.append("r");  continue; //   531: ȓ
      case 0x0214: plain.append("ue"); continue; //   532: Ȕ
      case 0x0215: plain.append("ue"); continue; //   533: ȕ
      case 0x0216: plain.append("u");  continue; //   534: Ȗ
      case 0x0217: plain.append("u");  continue; //   535: ȗ
      case 0x0218: plain.append("s");  continue; //   536: Ș
      case 0x0219: plain.append("s");  continue; //   537: ș
      case 0x021a: plain.append("t");  continue; //   538: Ț
      case 0x021b: plain.append("t");  continue; //   539: ț
      case 0x021c: plain.append("g");  continue; //   540: Ȝ
      case 0x021d: plain.append("g");  continue; //   541: ȝ
      case 0x021e: plain.append("h");  continue; //   542: Ȟ
      case 0x021f: plain.append("h");  continue; //   543: ȟ
      case 0x0220: plain.append("n");  continue; //   544: Ƞ
      case 0x0221: plain.append("d");  continue; //   545: ȡ
      case 0x0222: plain.append("ou"); continue; //   546: Ȣ
      case 0x0223: plain.append("ou"); continue; //   547: ȣ
      case 0x0224: plain.append("z");  continue; //   548: Ȥ
      case 0x0225: plain.append("z");  continue; //   549: ȥ
      case 0x0226: plain.append("a");  continue; //   550: Ȧ
      case 0x0227: plain.append("a");  continue; //   551: ȧ
      case 0x0228: plain.append("e");  continue; //   552: Ȩ
      case 0x0229: plain.append("e");  continue; //   553: ȩ
      case 0x022a: plain.append("oe"); continue; //   554: Ȫ
      case 0x022b: plain.append("oe"); continue; //   555: ȫ
      case 0x022c: plain.append("o");  continue; //   556: Ȭ
      case 0x022d: plain.append("o");  continue; //   557: ȭ
      case 0x022e: plain.append("o");  continue; //   558: Ȯ
      case 0x022f: plain.append("o");  continue; //   559: ȯ
      case 0x0230: plain.append("o");  continue; //   560: Ȱ
      case 0x0231: plain.append("o");  continue; //   561: ȱ
      case 0x0232: plain.append("y");  continue; //   562: Ȳ
      case 0x0233: plain.append("y");  continue; //   563: ȳ
      case 0x0234: plain.append("l");  continue; //   564: ȴ
      case 0x0235: plain.append("n");  continue; //   565: ȵ
      case 0x0236: plain.append("t");  continue; //   566: ȶ
      case 0x0237: plain.append("j");  continue; //   567: ȷ
      case 0x0238: plain.append("db"); continue; //   568: ȸ
      case 0x0239: plain.append("qp"); continue; //   569: ȹ
      case 0x023a: plain.append("a");  continue; //   570: Ⱥ
      case 0x023b: plain.append("c");  continue; //   571: Ȼ
      case 0x023c: plain.append("c");  continue; //   572: ȼ
      case 0x023d: plain.append("l");  continue; //   573: Ƚ
      case 0x023e: plain.append("t");  continue; //   574: Ⱦ
      case 0x023f: plain.append("s");  continue; //   575: ȿ
      case 0x0240: plain.append("z");  continue; //   576: ɀ
      case 0x0243: plain.append("b");  continue; //   579: Ƀ
      case 0x0244: plain.append("u");  continue; //   580: Ʉ
      case 0x0245: plain.append("v");  continue; //   581: Ʌ
      case 0x0246: plain.append("e");  continue; //   582: Ɇ
      case 0x0247: plain.append("e");  continue; //   583: ɇ
      case 0x0248: plain.append("j");  continue; //   584: Ɉ
      case 0x0249: plain.append("j");  continue; //   585: ɉ
      case 0x024a: plain.append("q");  continue; //   586: Ɋ
      case 0x024b: plain.append("q");  continue; //   587: ɋ
      case 0x024c: plain.append("r");  continue; //   588: Ɍ
      case 0x024d: plain.append("r");  continue; //   589: ɍ
      case 0x024e: plain.append("y");  continue; //   590: Ɏ
      case 0x024f: plain.append("y");  continue; //   591: ɏ

      // Latin Extended Additional
      case 0x1e00: plain.append("a");  continue; //  7680: Ḁ
      case 0x1e01: plain.append("a");  continue; //  7681: ḁ
      case 0x1e02: plain.append("b");  continue; //  7682: Ḃ
      case 0x1e03: plain.append("b");  continue; //  7683: ḃ
      case 0x1e04: plain.append("b");  continue; //  7684: Ḅ
      case 0x1e05: plain.append("b");  continue; //  7685: ḅ
      case 0x1e06: plain.append("b");  continue; //  7686: Ḇ
      case 0x1e07: plain.append("b");  continue; //  7687: ḇ
      case 0x1e08: plain.append("c");  continue; //  7688: Ḉ
      case 0x1e09: plain.append("c");  continue; //  7689: ḉ
      case 0x1e0a: plain.append("d");  continue; //  7690: Ḋ
      case 0x1e0b: plain.append("d");  continue; //  7691: ḋ
      case 0x1e0c: plain.append("d");  continue; //  7692: Ḍ
      case 0x1e0d: plain.append("d");  continue; //  7693: ḍ
      case 0x1e0e: plain.append("d");  continue; //  7694: Ḏ
      case 0x1e0f: plain.append("d");  continue; //  7695: ḏ
      case 0x1e10: plain.append("d");  continue; //  7696: Ḑ
      case 0x1e11: plain.append("d");  continue; //  7697: ḑ
      case 0x1e12: plain.append("d");  continue; //  7698: Ḓ
      case 0x1e13: plain.append("d");  continue; //  7699: ḓ
      case 0x1e14: plain.append("e");  continue; //  7700: Ḕ
      case 0x1e15: plain.append("e");  continue; //  7701: ḕ
      case 0x1e16: plain.append("e");  continue; //  7702: Ḗ
      case 0x1e17: plain.append("e");  continue; //  7703: ḗ
      case 0x1e18: plain.append("e");  continue; //  7704: Ḙ
      case 0x1e19: plain.append("e");  continue; //  7705: ḙ
      case 0x1e1a: plain.append("e");  continue; //  7706: Ḛ
      case 0x1e1b: plain.append("e");  continue; //  7707: ḛ
      case 0x1e1c: plain.append("e");  continue; //  7708: Ḝ
      case 0x1e1d: plain.append("e");  continue; //  7709: ḝ
      case 0x1e1e: plain.append("f");  continue; //  7710: Ḟ
      case 0x1e1f: plain.append("f");  continue; //  7711: ḟ
      case 0x1e20: plain.append("g");  continue; //  7712: Ḡ
      case 0x1e21: plain.append("g");  continue; //  7713: ḡ
      case 0x1e22: plain.append("h");  continue; //  7714: Ḣ
      case 0x1e23: plain.append("h");  continue; //  7715: ḣ
      case 0x1e24: plain.append("h");  continue; //  7716: Ḥ
      case 0x1e25: plain.append("h");  continue; //  7717: ḥ
      case 0x1e26: plain.append("h");  continue; //  7718: Ḧ
      case 0x1e27: plain.append("h");  continue; //  7719: ḧ
      case 0x1e28: plain.append("h");  continue; //  7720: Ḩ
      case 0x1e29: plain.append("h");  continue; //  7721: ḩ
      case 0x1e2a: plain.append("h");  continue; //  7722: Ḫ
      case 0x1e2b: plain.append("h");  continue; //  7723: ḫ
      case 0x1e2c: plain.append("i");  continue; //  7724: Ḭ
      case 0x1e2d: plain.append("i");  continue; //  7725: ḭ
      case 0x1e2e: plain.append("i");  continue; //  7726: Ḯ
      case 0x1e2f: plain.append("i");  continue; //  7727: ḯ
      case 0x1e30: plain.append("k");  continue; //  7728: Ḱ
      case 0x1e31: plain.append("k");  continue; //  7729: ḱ
      case 0x1e32: plain.append("k");  continue; //  7730: Ḳ
      case 0x1e33: plain.append("k");  continue; //  7731: ḳ
      case 0x1e34: plain.append("k");  continue; //  7732: Ḵ
      case 0x1e35: plain.append("k");  continue; //  7733: ḵ
      case 0x1e36: plain.append("l");  continue; //  7734: Ḷ
      case 0x1e37: plain.append("l");  continue; //  7735: ḷ
      case 0x1e38: plain.append("l");  continue; //  7736: Ḹ
      case 0x1e39: plain.append("l");  continue; //  7737: ḹ
      case 0x1e3a: plain.append("l");  continue; //  7738: Ḻ
      case 0x1e3b: plain.append("l");  continue; //  7739: ḻ
      case 0x1e3c: plain.append("l");  continue; //  7740: Ḽ
      case 0x1e3d: plain.append("l");  continue; //  7741: ḽ
      case 0x1e3e: plain.append("m");  continue; //  7742: Ḿ
      case 0x1e3f: plain.append("m");  continue; //  7743: ḿ
      case 0x1e40: plain.append("m");  continue; //  7744: Ṁ
      case 0x1e41: plain.append("m");  continue; //  7745: ṁ
      case 0x1e42: plain.append("m");  continue; //  7746: Ṃ
      case 0x1e43: plain.append("m");  continue; //  7747: ṃ
      case 0x1e44: plain.append("n");  continue; //  7748: Ṅ
      case 0x1e45: plain.append("n");  continue; //  7749: ṅ
      case 0x1e46: plain.append("n");  continue; //  7750: Ṇ
      case 0x1e47: plain.append("n");  continue; //  7751: ṇ
      case 0x1e48: plain.append("n");  continue; //  7752: Ṉ
      case 0x1e49: plain.append("n");  continue; //  7753: ṉ
      case 0x1e4a: plain.append("n");  continue; //  7754: Ṋ
      case 0x1e4b: plain.append("n");  continue; //  7755: ṋ
      case 0x1e4c: plain.append("o");  continue; //  7756: Ṍ
      case 0x1e4d: plain.append("o");  continue; //  7757: ṍ
      case 0x1e4e: plain.append("oe"); continue; //  7758: Ṏ
      case 0x1e4f: plain.append("oe"); continue; //  7759: ṏ
      case 0x1e50: plain.append("o");  continue; //  7760: Ṑ
      case 0x1e51: plain.append("o");  continue; //  7761: ṑ
      case 0x1e52: plain.append("o");  continue; //  7762: Ṓ
      case 0x1e53: plain.append("o");  continue; //  7763: ṓ
      case 0x1e54: plain.append("p");  continue; //  7764: Ṕ
      case 0x1e55: plain.append("p");  continue; //  7765: ṕ
      case 0x1e56: plain.append("p");  continue; //  7766: Ṗ
      case 0x1e57: plain.append("p");  continue; //  7767: ṗ
      case 0x1e58: plain.append("r");  continue; //  7768: Ṙ
      case 0x1e59: plain.append("r");  continue; //  7769: ṙ
      case 0x1e5a: plain.append("r");  continue; //  7770: Ṛ
      case 0x1e5b: plain.append("r");  continue; //  7771: ṛ
      case 0x1e5c: plain.append("r");  continue; //  7772: Ṝ
      case 0x1e5d: plain.append("r");  continue; //  7773: ṝ
      case 0x1e5e: plain.append("r");  continue; //  7774: Ṟ
      case 0x1e5f: plain.append("r");  continue; //  7775: ṟ
      case 0x1e60: plain.append("s");  continue; //  7776: Ṡ
      case 0x1e61: plain.append("s");  continue; //  7777: ṡ
      case 0x1e62: plain.append("s");  continue; //  7778: Ṣ
      case 0x1e63: plain.append("s");  continue; //  7779: ṣ
      case 0x1e64: plain.append("s");  continue; //  7780: Ṥ
      case 0x1e65: plain.append("s");  continue; //  7781: ṥ
      case 0x1e66: plain.append("s");  continue; //  7782: Ṧ
      case 0x1e67: plain.append("s");  continue; //  7783: ṧ
      case 0x1e68: plain.append("s");  continue; //  7784: Ṩ
      case 0x1e69: plain.append("s");  continue; //  7785: ṩ
      case 0x1e6a: plain.append("t");  continue; //  7786: Ṫ
      case 0x1e6b: plain.append("t");  continue; //  7787: ṫ
      case 0x1e6c: plain.append("t");  continue; //  7788: Ṭ
      case 0x1e6d: plain.append("t");  continue; //  7789: ṭ
      case 0x1e6e: plain.append("t");  continue; //  7790: Ṯ
      case 0x1e6f: plain.append("t");  continue; //  7791: ṯ
      case 0x1e70: plain.append("t");  continue; //  7792: Ṱ
      case 0x1e71: plain.append("t");  continue; //  7793: ṱ
      case 0x1e72: plain.append("u");  continue; //  7794: Ṳ
      case 0x1e73: plain.append("u");  continue; //  7795: ṳ
      case 0x1e74: plain.append("u");  continue; //  7796: Ṵ
      case 0x1e75: plain.append("u");  continue; //  7797: ṵ
      case 0x1e76: plain.append("u");  continue; //  7798: Ṷ
      case 0x1e77: plain.append("u");  continue; //  7799: ṷ
      case 0x1e78: plain.append("u");  continue; //  7800: Ṹ
      case 0x1e79: plain.append("u");  continue; //  7801: ṹ
      case 0x1e7a: plain.append("ue"); continue; //  7802: Ṻ
      case 0x1e7b: plain.append("ue"); continue; //  7803: ṻ
      case 0x1e7c: plain.append("v");  continue; //  7804: Ṽ
      case 0x1e7d: plain.append("v");  continue; //  7805: ṽ
      case 0x1e7e: plain.append("v");  continue; //  7806: Ṿ
      case 0x1e7f: plain.append("v");  continue; //  7807: ṿ
      case 0x1e80: plain.append("w");  continue; //  7808: Ẁ
      case 0x1e81: plain.append("w");  continue; //  7809: ẁ
      case 0x1e82: plain.append("w");  continue; //  7810: Ẃ
      case 0x1e83: plain.append("w");  continue; //  7811: ẃ
      case 0x1e84: plain.append("w");  continue; //  7812: Ẅ
      case 0x1e85: plain.append("w");  continue; //  7813: ẅ
      case 0x1e86: plain.append("w");  continue; //  7814: Ẇ
      case 0x1e87: plain.append("w");  continue; //  7815: ẇ
      case 0x1e88: plain.append("w");  continue; //  7816: Ẉ
      case 0x1e89: plain.append("w");  continue; //  7817: ẉ
      case 0x1e8a: plain.append("x");  continue; //  7818: Ẋ
      case 0x1e8b: plain.append("x");  continue; //  7819: ẋ
      case 0x1e8c: plain.append("x");  continue; //  7820: Ẍ
      case 0x1e8d: plain.append("x");  continue; //  7821: ẍ
      case 0x1e8e: plain.append("y");  continue; //  7822: Ẏ
      case 0x1e8f: plain.append("y");  continue; //  7823: ẏ
      case 0x1e90: plain.append("z");  continue; //  7824: Ẑ
      case 0x1e91: plain.append("z");  continue; //  7825: ẑ
      case 0x1e92: plain.append("z");  continue; //  7826: Ẓ
      case 0x1e93: plain.append("z");  continue; //  7827: ẓ
      case 0x1e94: plain.append("z");  continue; //  7828: Ẕ
      case 0x1e95: plain.append("z");  continue; //  7829: ẕ
      case 0x1e96: plain.append("h");  continue; //  7830: ẖ
      case 0x1e97: plain.append("t");  continue; //  7831: ẗ
      case 0x1e98: plain.append("w");  continue; //  7832: ẘ
      case 0x1e99: plain.append("y");  continue; //  7833: ẙ
      case 0x1e9a: plain.append("a");  continue; //  7834: ẚ
      case 0x1e9b: plain.append("s");  continue; //  7835: ẛ
      case 0x1e9c: plain.append("s");  continue; //  7836: ẜ
      case 0x1e9d: plain.append("s");  continue; //  7837: ẝ
      case 0x1e9e: plain.append("ss"); continue; //  7838: ẞ
      case 0x1e9f: plain.append("d");  continue; //  7839: ẟ
      case 0x1ea0: plain.append("a");  continue; //  7840: Ạ
      case 0x1ea1: plain.append("a");  continue; //  7841: ạ
      case 0x1ea2: plain.append("a");  continue; //  7842: Ả
      case 0x1ea3: plain.append("a");  continue; //  7843: ả
      case 0x1ea4: plain.append("a");  continue; //  7844: Ấ
      case 0x1ea5: plain.append("a");  continue; //  7845: ấ
      case 0x1ea6: plain.append("a");  continue; //  7846: Ầ
      case 0x1ea7: plain.append("a");  continue; //  7847: ầ
      case 0x1ea8: plain.append("a");  continue; //  7848: Ẩ
      case 0x1ea9: plain.append("a");  continue; //  7849: ẩ
      case 0x1eaa: plain.append("a");  continue; //  7850: Ẫ
      case 0x1eab: plain.append("a");  continue; //  7851: ẫ
      case 0x1eac: plain.append("a");  continue; //  7852: Ậ
      case 0x1ead: plain.append("a");  continue; //  7853: ậ
      case 0x1eae: plain.append("a");  continue; //  7854: Ắ
      case 0x1eaf: plain.append("a");  continue; //  7855: ắ
      case 0x1eb0: plain.append("a");  continue; //  7856: Ằ
      case 0x1eb1: plain.append("a");  continue; //  7857: ằ
      case 0x1eb2: plain.append("a");  continue; //  7858: Ẳ
      case 0x1eb3: plain.append("a");  continue; //  7859: ẳ
      case 0x1eb4: plain.append("a");  continue; //  7860: Ẵ
      case 0x1eb5: plain.append("a");  continue; //  7861: ẵ
      case 0x1eb6: plain.append("a");  continue; //  7862: Ặ
      case 0x1eb7: plain.append("a");  continue; //  7863: ặ
      case 0x1eb8: plain.append("e");  continue; //  7864: Ẹ
      case 0x1eb9: plain.append("e");  continue; //  7865: ẹ
      case 0x1eba: plain.append("e");  continue; //  7866: Ẻ
      case 0x1ebb: plain.append("e");  continue; //  7867: ẻ
      case 0x1ebc: plain.append("e");  continue; //  7868: Ẽ
      case 0x1ebd: plain.append("e");  continue; //  7869: ẽ
      case 0x1ebe: plain.append("e");  continue; //  7870: Ế
      case 0x1ebf: plain.append("e");  continue; //  7871: ế
      case 0x1ec0: plain.append("e");  continue; //  7872: Ề
      case 0x1ec1: plain.append("e");  continue; //  7873: ề
      case 0x1ec2: plain.append("e");  continue; //  7874: Ể
      case 0x1ec3: plain.append("e");  continue; //  7875: ể
      case 0x1ec4: plain.append("e");  continue; //  7876: Ễ
      case 0x1ec5: plain.append("e");  continue; //  7877: ễ
      case 0x1ec6: plain.append("e");  continue; //  7878: Ệ
      case 0x1ec7: plain.append("e");  continue; //  7879: ệ
      case 0x1ec8: plain.append("i");  continue; //  7880: Ỉ
      case 0x1ec9: plain.append("i");  continue; //  7881: ỉ
      case 0x1eca: plain.append("i");  continue; //  7882: Ị
      case 0x1ecb: plain.append("i");  continue; //  7883: ị
      case 0x1ecc: plain.append("o");  continue; //  7884: Ọ
      case 0x1ecd: plain.append("o");  continue; //  7885: ọ
      case 0x1ece: plain.append("o");  continue; //  7886: Ỏ
      case 0x1ecf: plain.append("o");  continue; //  7887: ỏ
      case 0x1ed0: plain.append("o");  continue; //  7888: Ố
      case 0x1ed1: plain.append("o");  continue; //  7889: ố
      case 0x1ed2: plain.append("o");  continue; //  7890: Ồ
      case 0x1ed3: plain.append("o");  continue; //  7891: ồ
      case 0x1ed4: plain.append("o");  continue; //  7892: Ổ
      case 0x1ed5: plain.append("o");  continue; //  7893: ổ
      case 0x1ed6: plain.append("o");  continue; //  7894: Ỗ
      case 0x1ed7: plain.append("o");  continue; //  7895: ỗ
      case 0x1ed8: plain.append("o");  continue; //  7896: Ộ
      case 0x1ed9: plain.append("o");  continue; //  7897: ộ
      case 0x1eda: plain.append("o");  continue; //  7898: Ớ
      case 0x1edb: plain.append("o");  continue; //  7899: ớ
      case 0x1edc: plain.append("o");  continue; //  7900: Ờ
      case 0x1edd: plain.append("o");  continue; //  7901: ờ
      case 0x1ede: plain.append("o");  continue; //  7902: Ở
      case 0x1edf: plain.append("o");  continue; //  7903: ở
      case 0x1ee0: plain.append("o");  continue; //  7904: Ỡ
      case 0x1ee1: plain.append("o");  continue; //  7905: ỡ
      case 0x1ee2: plain.append("o");  continue; //  7906: Ợ
      case 0x1ee3: plain.append("o");  continue; //  7907: ợ
      case 0x1ee4: plain.append("u");  continue; //  7908: Ụ
      case 0x1ee5: plain.append("u");  continue; //  7909: ụ
      case 0x1ee6: plain.append("u");  continue; //  7910: Ủ
      case 0x1ee7: plain.append("u");  continue; //  7911: ủ
      case 0x1ee8: plain.append("u");  continue; //  7912: Ứ
      case 0x1ee9: plain.append("u");  continue; //  7913: ứ
      case 0x1eea: plain.append("u");  continue; //  7914: Ừ
      case 0x1eeb: plain.append("u");  continue; //  7915: ừ
      case 0x1eec: plain.append("u");  continue; //  7916: Ử
      case 0x1eed: plain.append("u");  continue; //  7917: ử
      case 0x1eee: plain.append("u");  continue; //  7918: Ữ
      case 0x1eef: plain.append("u");  continue; //  7919: ữ
      case 0x1ef0: plain.append("u");  continue; //  7920: Ự
      case 0x1ef1: plain.append("u");  continue; //  7921: ự
      case 0x1ef2: plain.append("y");  continue; //  7922: Ỳ
      case 0x1ef3: plain.append("y");  continue; //  7923: ỳ
      case 0x1ef4: plain.append("y");  continue; //  7924: Ỵ
      case 0x1ef5: plain.append("y");  continue; //  7925: ỵ
      case 0x1ef6: plain.append("y");  continue; //  7926: Ỷ
      case 0x1ef7: plain.append("y");  continue; //  7927: ỷ
      case 0x1ef8: plain.append("y");  continue; //  7928: Ỹ
      case 0x1ef9: plain.append("y");  continue; //  7929: ỹ
      case 0x1efa: plain.append("li"); continue; //  7930: Ỻ
      case 0x1efb: plain.append("li"); continue; //  7931: ỻ
      case 0x1efc: plain.append("v");  continue; //  7932: Ỽ
      case 0x1efd: plain.append("v");  continue; //  7933: ỽ
      case 0x1efe: plain.append("y");  continue; //  7934: Ỿ
      case 0x1eff: plain.append("y");  continue; //  7935: ỿ

      // Currency Symbols
      case 0x20ac: plain.append("eur"); continue; //  8364: €

      // Latin Extended-C
      case 0x2c60: plain.append("l");  continue; // 11360: Ⱡ
      case 0x2c61: plain.append("i");  continue; // 11361: ⱡ
      case 0x2c62: plain.append("l");  continue; // 11362: Ɫ
      case 0x2c63: plain.append("p");  continue; // 11363: Ᵽ
      case 0x2c64: plain.append("r");  continue; // 11364: Ɽ
      case 0x2c65: plain.append("a");  continue; // 11365: ⱥ
      case 0x2c66: plain.append("t");  continue; // 11366: ⱦ
      case 0x2c67: plain.append("h");  continue; // 11367: Ⱨ
      case 0x2c68: plain.append("h");  continue; // 11368: ⱨ
      case 0x2c69: plain.append("k");  continue; // 11369: Ⱪ
      case 0x2c6a: plain.append("k");  continue; // 11370: ⱪ
      case 0x2c6b: plain.append("z");  continue; // 11371: Ⱬ
      case 0x2c6c: plain.append("z");  continue; // 11372: ⱬ
      case 0x2c6d: plain.append("a");  continue; // 11373: Ɑ
      case 0x2c6e: plain.append("m");  continue; // 11374: Ɱ
      case 0x2c6f: plain.append("a");  continue; // 11375: Ɐ
      case 0x2c70: plain.append("a");  continue; // 11376: Ɒ
      case 0x2c71: plain.append("v");  continue; // 11377: ⱱ
      case 0x2c72: plain.append("w");  continue; // 11378: Ⱳ
      case 0x2c73: plain.append("w");  continue; // 11379: ⱳ
      case 0x2c74: plain.append("v");  continue; // 11380: ⱴ
      case 0x2c75: plain.append("h");  continue; // 11381: Ⱶ
      case 0x2c76: plain.append("h");  continue; // 11382: ⱶ
      case 0x2c77: plain.append("ph"); continue; // 11383: ⱷ
      case 0x2c78: plain.append("e");  continue; // 11384: ⱸ
      case 0x2c79: plain.append("r");  continue; // 11385: ⱹ
      case 0x2c7a: plain.append("o");  continue; // 11386: ⱺ
      case 0x2c7b: plain.append("e");  continue; // 11387: ⱻ
      case 0x2c7c: plain.append("j");  continue; // 11388: ⱼ
      case 0x2c7d: plain.append("v");  continue; // 11389: ⱽ
      case 0x2c7e: plain.append("s");  continue; // 11390: Ȿ
      case 0x2c7f: plain.append("z");  continue; // 11391: Ɀ

      // Latin Extended-D
      case 0xa726: plain.append("h");  continue; // 42790: Ꜧ
      case 0xa727: plain.append("h");  continue; // 42791: ꜧ
      case 0xa728: plain.append("tz"); continue; // 42792: Ꜩ
      case 0xa729: plain.append("tz"); continue; // 42793: ꜩ
      case 0xa730: plain.append("f");  continue; // 42800: ꜰ
      case 0xa731: plain.append("s");  continue; // 42801: ꜱ
      case 0xa732: plain.append("AA"); continue; // 42802: Ꜳ
      case 0xa733: plain.append("aa"); continue; // 42803: ꜳ
      case 0xa734: plain.append("ao"); continue; // 42804: Ꜵ
      case 0xa735: plain.append("ao"); continue; // 42805: ꜵ
      case 0xa736: plain.append("au"); continue; // 42806: Ꜷ
      case 0xa737: plain.append("au"); continue; // 42807: ꜷ
      case 0xa738: plain.append("av"); continue; // 42808: Ꜹ
      case 0xa739: plain.append("av"); continue; // 42809: ꜹ
      case 0xa73a: plain.append("av"); continue; // 42810: Ꜻ
      case 0xa73b: plain.append("av"); continue; // 42811: ꜻ
      case 0xa73c: plain.append("ay"); continue; // 42812: Ꜽ
      case 0xa73d: plain.append("ay"); continue; // 42813: ꜽ
      case 0xa73e: plain.append("c");  continue; // 42814: Ꜿ
      case 0xa73f: plain.append("c");  continue; // 42815: ꜿ
      case 0xa740: plain.append("k");  continue; // 42816: Ꝁ
      case 0xa741: plain.append("k");  continue; // 42817: ꝁ
      case 0xa742: plain.append("k");  continue; // 42818: Ꝃ
      case 0xa743: plain.append("k");  continue; // 42819: ꝃ
      case 0xa744: plain.append("k");  continue; // 42820: Ꝅ
      case 0xa745: plain.append("k");  continue; // 42821: ꝅ
      case 0xa746: plain.append("l");  continue; // 42822: Ꝇ
      case 0xa747: plain.append("l");  continue; // 42823: ꝇ
      case 0xa748: plain.append("l");  continue; // 42824: Ꝉ
      case 0xa749: plain.append("l");  continue; // 42825: ꝉ
      case 0xa74a: plain.append("o");  continue; // 42826: Ꝋ
      case 0xa74b: plain.append("o");  continue; // 42827: ꝋ
      case 0xa74c: plain.append("o");  continue; // 42828: Ꝍ
      case 0xa74d: plain.append("o");  continue; // 42829: ꝍ
      case 0xa74e: plain.append("oo"); continue; // 42830: Ꝏ
      case 0xa74f: plain.append("oo"); continue; // 42831: ꝏ
      case 0xa750: plain.append("p");  continue; // 42832: Ꝑ
      case 0xa751: plain.append("p");  continue; // 42833: ꝑ
      case 0xa752: plain.append("p");  continue; // 42834: Ꝓ
      case 0xa753: plain.append("p");  continue; // 42835: ꝓ
      case 0xa754: plain.append("p");  continue; // 42836: Ꝕ
      case 0xa755: plain.append("p");  continue; // 42837: ꝕ
      case 0xa756: plain.append("q");  continue; // 42838: Ꝗ
      case 0xa757: plain.append("q");  continue; // 42839: ꝗ
      case 0xa758: plain.append("q");  continue; // 42840: Ꝙ
      case 0xa759: plain.append("q");  continue; // 42841: ꝙ
      case 0xa75a: plain.append("r");  continue; // 42842: Ꝛ
      case 0xa75b: plain.append("r");  continue; // 42843: ꝛ
      case 0xa75e: plain.append("v");  continue; // 42846: Ꝟ
      case 0xa75f: plain.append("v");  continue; // 42847: ꝟ
      case 0xa760: plain.append("vy"); continue; // 42848: Ꝡ
      case 0xa761: plain.append("vy"); continue; // 42849: ꝡ
      case 0xa762: plain.append("z");  continue; // 42850: Ꝣ
      case 0xa763: plain.append("z");  continue; // 42851: ꝣ
      case 0xa764: plain.append("th"); continue; // 42852: Ꝥ
      case 0xa765: plain.append("th"); continue; // 42853: ꝥ
      case 0xa766: plain.append("th"); continue; // 42854: Ꝧ
      case 0xa767: plain.append("th"); continue; // 42855: ꝧ
      case 0xa768: plain.append("v");  continue; // 42856: Ꝩ
      case 0xa769: plain.append("v");  continue; // 42857: ꝩ
      case 0xa76e: plain.append("9");  continue; // 42862: Ꝯ
      case 0xa76f: plain.append("9");  continue; // 42863: ꝯ
      case 0xa770: plain.append("9");  continue; // 42864: ꝰ
      case 0xa771: plain.append("d");  continue; // 42865: ꝱ
      case 0xa772: plain.append("l");  continue; // 42866: ꝲ
      case 0xa773: plain.append("m");  continue; // 42867: ꝳ
      case 0xa774: plain.append("n");  continue; // 42868: ꝴ
      case 0xa775: plain.append("r");  continue; // 42869: ꝵ
      case 0xa776: plain.append("r");  continue; // 42870: ꝶ
      case 0xa777: plain.append("t");  continue; // 42871: ꝷ
      case 0xa779: plain.append("d");  continue; // 42873: Ꝺ
      case 0xa77a: plain.append("d");  continue; // 42874: ꝺ
      case 0xa77b: plain.append("f");  continue; // 42875: Ꝼ
      case 0xa77c: plain.append("f");  continue; // 42876: ꝼ
      case 0xa77d: plain.append("g");  continue; // 42877: Ᵹ
      case 0xa77e: plain.append("g");  continue; // 42878: Ꝿ
      case 0xa77f: plain.append("g");  continue; // 42879: ꝿ
      case 0xa780: plain.append("l");  continue; // 42880: Ꞁ
      case 0xa781: plain.append("l");  continue; // 42881: ꞁ
      case 0xa782: plain.append("r");  continue; // 42882: Ꞃ
      case 0xa783: plain.append("r");  continue; // 42883: ꞃ
      case 0xa784: plain.append("s");  continue; // 42884: Ꞅ
      case 0xa785: plain.append("s");  continue; // 42885: ꞅ
      case 0xa786: plain.append("t");  continue; // 42886: Ꞇ
      case 0xa787: plain.append("t");  continue; // 42887: ꞇ
      case 0xa78d: plain.append("h");  continue; // 42893: Ɥ
      case 0xa78e: plain.append("l");  continue; // 42894: ꞎ
      case 0xa790: plain.append("n");  continue; // 42896: Ꞑ
      case 0xa791: plain.append("n");  continue; // 42897: ꞑ
      case 0xa792: plain.append("c");  continue; // 42898: Ꞓ
      case 0xa7a0: plain.append("g");  continue; // 42912: Ꞡ
      case 0xa7a1: plain.append("g");  continue; // 42913: ꞡ
      case 0xa7a2: plain.append("k");  continue; // 42914: Ꞣ
      case 0xa7a3: plain.append("k");  continue; // 42915: ꞣ
      case 0xa7a4: plain.append("n");  continue; // 42916: Ꞥ
      case 0xa7a5: plain.append("n");  continue; // 42917: ꞥ
      case 0xa7a6: plain.append("r");  continue; // 42918: Ꞧ
      case 0xa7a7: plain.append("r");  continue; // 42919: ꞧ
      case 0xa7a8: plain.append("s");  continue; // 42920: Ꞩ
      case 0xa7a9: plain.append("s");  continue; // 42921: ꞩ
      case 0xa7aa: plain.append("h");  continue; // 42922: Ɦ
      case 0xa7f8: plain.append("h");  continue; // 43000: ꟸ
      case 0xa7f9: plain.append("oe"); continue; // 43001: ꟹ
      case 0xa7fa: plain.append("m");  continue; // 43002: ꟺ
      case 0xa7fb: plain.append("f");  continue; // 43003: ꟻ
      case 0xa7fc: plain.append("p");  continue; // 43004: ꟼ
      case 0xa7fd: plain.append("m");  continue; // 43005: ꟽ
      case 0xa7fe: plain.append("i");  continue; // 43006: ꟾ
      case 0xa7ff: plain.append("m");  continue; // 43007: ꟿ
    }

    // default substitution
    plain.append("_");
  }

  // insert separator or not
  bool separate = false;

  // squeeze underscores
  for(string::size_type i = 0; i < plain.size(); i++)
  {
    // get current character
    unsigned char uc = static_cast<unsigned char>(plain[i]);

    // separator found
    if (uc == '_')
    {
      // set flag
      separate = true;
    }

    // normal character found
    else
    {
      // add separator first
      if (separate)
      {
        // don't start with separator
        if ( !out.empty() )
        {
          out += '_';
        }

        // reset flag
        separate = false;
      }

      // append character
      out += uc;
    }
  }

  // check maximum length (ext4)
  if (out.size() > 255)
  {
    // notify user
    msg::err( msg::catq("filename is too long: ", out) );

    // signalize trouble
    return false;
  }

  // signalize success
  return true;
}

