// -----------------------------------------------------------------------------
// UnescapeHandler.cpp                                       UnescapeHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref UnescapeHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include "message.h"
#include "UnescapeHandler.h"


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// ---------------
// UnescapeHandler
// ---------------
/*
 *
 */
UnescapeHandler::UnescapeHandler(KVHandler* next)
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
void UnescapeHandler::OnBeginParsing(const string& filename)
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
void UnescapeHandler::OnData(const string& key, const string& value)
{
  // no handler set
  if (m_next == 0) return;

  // invalid state
  if ( !healthy() ) return;

  // the unescaped value
  string plain;

  // try to remove escape sequences
  if ( unescape(value, plain) )
  {
    // notify next handler
    m_next->OnData(key, plain);
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

// --------
// unescape
// --------
/*
 *
 */
bool UnescapeHandler::unescape(const string& s, string& plain) const
{
  // reset return value
  plain = "";

  // the parser's state
  enum { TXT, ESC } state = TXT;

  // remove escape sequences
  for(string::size_type i = 0; i < s.size(); i++)
  {
    // get current character
    unsigned char uc = static_cast<unsigned char>(s[i]);

    // TXT
    if (state == TXT)
    {
      // escape sequence started
      if (uc == '\\')
      {
        // switch state
        state = ESC;
      }

      // verbatim characters
      else
      {
        // append character
        plain += uc;
      }
    }

    // ESC
    else if (state == ESC)
    {
      // append character
      plain += uc;

      // switch state
      state = TXT;
    }
  }

  // check final state
  if (state == ESC)
  {
    // notify user
    msg::err( msg::catq("invalid syntax: ", s) );

    // signalize trouble
    return false;
  }

  // signalize success
  return true;
}

