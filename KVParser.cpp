// -----------------------------------------------------------------------------
// KVParser.cpp                                                     KVParser.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref KVParser class.
 * @author     Col. Walter E. Kurtz
 * @version    2018-03-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <sstream>
#include <fstream>
#include <iostream>
#include "message.h"
#include "KVHandler.h"
#include "KVParser.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// --------
// KVParser
// --------
/*
 *
 */
KVParser::KVParser()
{
  // no file is currently parsed
  m_filename = "";

  // initialize pointer
  m_handler = 0;
}


// -----------------------------------------------------------------------------
// Initialization                                                 Initialization
// -----------------------------------------------------------------------------

// ----------
// setHandler
// ----------
/*
 *
 */
void KVParser::setHandler(KVHandler* handler)
{
  m_handler = handler;
}


// -----------------------------------------------------------------------------
// Handling                                                             Handling
// -----------------------------------------------------------------------------

// -----
// parse
// -----
/*
 *
 */
bool KVParser::parse()
{
  // no handler set
  if (m_handler == 0)
  {
    // signalize trouble
    return false;
  }

  // parsing stdin
  m_filename = "-";

  // send message
  m_handler->OnBeginParsing("");

  // use internal method
  bool flag = parseStream(cin);

  // send message
  m_handler->OnEndParsing(flag);

  // no file is currently parsed
  m_filename = "";

  // signalize trouble
  return flag;
}

// -----
// parse
// -----
/*
 *
 */
bool KVParser::parse(const string& filename)
{
  // no handler set
  if (m_handler == 0)
  {
    // signalize trouble
    return false;
  }

  // parsing given file
  m_filename = filename;

  // send message
  m_handler->OnBeginParsing(filename);

  // try to open file for reading
  ifstream ifile(filename.c_str());

  // check file operation
  if ( !ifile.is_open() )
  {
    // notify user
    error("unable to open file", filename);

    // send message
    m_handler->OnEndParsing(false);

    // signalize trouble
    return false;
  }

  // use internal method
  bool flag = parseStream(ifile);

  // close file
  ifile.close();

  // send message
  m_handler->OnEndParsing(flag);

  // no file is currently parsed
  m_filename = "";

  // signalize trouble
  return flag;
}


// -----------------------------------------------------------------------------
// Internal methods                                             Internal methods
// -----------------------------------------------------------------------------

// -------------------
// isKeyStartCharacter
// -------------------
/*
 *
 */
bool KVParser::isKeyStartCharacter(char c) const
{
  // keys are allowed to start with capital letters
  if ((c >= 'A') && (c <= 'Z')) return true;

  // keys are allowed to start with small letters
  if ((c >= 'a') && (c <= 'z')) return true;

  // keys are allowed to start with underscore
  if (c == '_') return true;

  // reject all other characters
  return false;
}

// --------------
// isKeyCharacter
// --------------
/*
 *
 */
bool KVParser::isKeyCharacter(char c) const
{
  // keys are allowed to contain capital letters
  if ((c >= 'A') && (c <= 'Z')) return true;

  // keys are allowed to contain small letters
  if ((c >= 'a') && (c <= 'z')) return true;

  // keys are allowed to contain digits
  if ((c >= '0') && (c <= '9')) return true;

  // keys are allowed to contain underscores
  if (c == '_') return true;

  // reject all other characters
  return false;
}

// -----------
// parseStream
// -----------
/*
 *
 */
bool KVParser::parseStream(istream& stream)
{
  // the parser's state
  enum { FIRST, KEY, VALUE, COMMENT, CORRUPTED } state = FIRST;

  // buffers
  string key = "";
  string val = "";

  // line number
  unsigned lno = 1;

  // one extracted byte
  char c = 0;

  // get all bytes from given stream
  while( stream.get(c) )
  {
    // check handler first regardless of current state
    if ( !(m_handler->healthy()) )
    {
      // set final state
      state = CORRUPTED;

      // exit loop
      break;
    }

    // FIRST
    if (state == FIRST)
    {
      // empty line found
      if (c == 10)
      {
        // step counter
        lno += 1;
      }

      // comment found
      else if (c == '#')
      {
        // set next state
        state = COMMENT;
      }

      // valid start character found
      else if ( isKeyStartCharacter(c) )
      {
        // append current character
        key += c;

        // set netxt state
        state = KEY;
      }

      else
      {
        // allow initial white space
        if ((c != 9) && (c != 32))
        {
          // send error message
          error(lno, "a key must not start with this character", c);

          // set final state
          state = CORRUPTED;

          // exit loop
          break;
        }
      }
    }

    // KEY
    else if (state == KEY)
    {
      // end of line character found
      if (c == 10)
      {
        // send error message
        error(lno, "end of line not allowed here");

        // set final state
        state = CORRUPTED;

        // exit loop
        break;
      }

      // key finished
      else if (c == '=')
      {
        // invalid key
        if (key.empty())
        {
          // send error message
          error(lno, "empty key found");

          // set final state
          state = CORRUPTED;

          // exit loop
          break;
        }

        // set next state
        state = VALUE;
      }

      // valid key character found
      else if ( isKeyCharacter(c) )
      {
        // append current character
        key += c;
      }

      // character is not allowed to appear inside a key name
      else
      {
        // send error message
        error(lno, "a key must not contain this character", c);

        // set final state
        state = CORRUPTED;

        // exit loop
        break;
      }
    }

    // VALUE
    else if (state == VALUE)
    {
      // end of line character found
      if (c == 10)
      {
        // send message
        m_handler->OnData(key, val);

        // reset buffers
        key = "";
        val = "";

        // step line counter
        lno += 1;

        // back to initial state
        state = FIRST;
      }

      // normal character found
      else
      {
        // append current character
        val += c;
      }
    }

    // COMMENT
    else if (state == COMMENT)
    {
      // end of line character found
      if (c == 10)
      {
        // step line counter
        lno += 1;

        // set next state
        state = FIRST;
      }
    }
  }

  // check final state
  if (state != FIRST)
  {
    if (state != CORRUPTED)
    {
      // send error message
      error(lno, "unexpected end of stream");
    }

    // signalize trouble
    return false;
  }

  // signalize success
  return true;
}

// -----
// error
// -----
/*
 *
 */
void KVParser::error(const string& msg)
{
  // use err function
  msg::err(msg);
}

// -----
// error
// -----
/*
 *
 */
void KVParser::error(const string& msg, const string& bad)
{
  // create message
  stringstream message;
  message << msg;
  message << ": \"";
  message << bad;
  message << "\"";

  // call this version
  error(message.str());
}

// -----
// error
// -----
/*
 *
 */
void KVParser::error(unsigned lno, const string& msg)
{
  // create message
  stringstream message;
  message << m_filename;
  message << " line ";
  message << lno;
  message << ": ";
  message << msg;

  // call this version
  error(message.str());
}

// -----
// error
// -----
/*
 *
 */
void KVParser::error(unsigned lno, const string& msg, char bad)
{
  // create message
  stringstream message;
  message << m_filename;
  message << " line ";
  message << lno;
  message << ": ";
  message << msg;
  message << ": \"";
  message << bad;
  message << "\"";

  // call this version
  error(message.str());
}

