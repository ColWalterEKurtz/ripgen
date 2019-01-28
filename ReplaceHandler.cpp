// -----------------------------------------------------------------------------
// ReplaceHandler.cpp                                         ReplaceHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref ReplaceHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include "message.h"
#include "ReplaceHandler.h"


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// --------------
// ReplaceHandler
// --------------
/*
 *
 */
ReplaceHandler::ReplaceHandler(KVHandler* next)
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
void ReplaceHandler::OnBeginParsing(const string& filename)
{
  // reset healthy flag
  setHealthy();

  // reset stacks
  m_keys.clear();
  m_values.clear();

  // notify next handler
  if (m_next)
  {
    m_next->OnBeginParsing(filename);
  }
}

// ------------
// OnEndParsing
// ------------
/*
 *
 */
void ReplaceHandler::OnEndParsing(bool healthy)
{
  // empty stacks
  m_keys.clear();
  m_values.clear();

  // notify next handler
  if (m_next)
  {
    m_next->OnEndParsing(healthy);
  }
}

// ------
// OnData
// ------
/*
 *
 */
void ReplaceHandler::OnData(const string& key, const string& value)
{
  // no handler set
  if (m_next == 0) return;

  // invalid state
  if ( !healthy() ) return;

  // buffer tags
  m_keys.push_back(key);
  m_values.push_back(value);

  // COMPILATIONINDEX tag found
  if (key == "COMPILATIONINDEX")
  {
    // try to replace all IDs
    if ( replaceAll() )
    {
      // flush all buffered values
      for(unsigned i = 0; i < m_keys.size(); i++)
      {
        // notify next handler
        m_next->OnData(m_keys[i], m_values[i]);

        // check healthy state
        if ( !(m_next->healthy()) )
        {
          // update healthy flag
          setHealthy(false);

          // exit loop
          break;
        }
      }
    }

    else
    {
      // update healthy flag
      setHealthy(false);
    }

    // empty stacks
    m_keys.clear();
    m_values.clear();
  }
}


// -----------------------------------------------------------------------------
// Internal methods                                             Internal methods
// -----------------------------------------------------------------------------

// ----------
// replaceAll
// ----------
/*
 *
 */
bool ReplaceHandler::replaceAll()
{
  bool pending = true;
  bool updated = true;

  // while replacements can (and need to) be done
  while (pending && updated)
  {
    // reset flags
    pending = false;
    updated = false;

    // replace all IDs
    for(unsigned i = 0; i < m_values.size(); i++)
    {
      // get current value
      const string& value = m_values[i];

      // parts of a value
      string a, id, b;

      // split value
      if ( !split(value, a, id, b) )
      {
        // invalid syntax
        return false;
      }

      // value is already final
      if ( id.empty() ) continue;

      // still work to be done
      pending = true;

      // get value to insert
      string paste = getValue(id);

      // empty value specified
      if ( paste.empty() )
      {
        // notify user
        msg::wrn( msg::cat("empty substitution: $", id) );
      }

      // requested value can be inserted
      if ( isFinal(paste) )
      {
        // update value
        m_values[i] = a + paste + b;

        // set flag
        updated = true;
      }
    }
  }

  // unable to finish all values
  if (pending)
  {
    // notify user
    msg::err("unable to finish some values");

    // signalize trouble
    return false;
  }

  // signalize success
  return true;
}

// -----
// split
// -----
/*
 *
 */
bool ReplaceHandler::split(const string& s, string& a, string& id, string& b) const
{
  // reset return values
  a  = "";
  id = "";
  b  = "";

  // parser states
  enum
  {
    READ_PRE,
    READ_ESC,
    CHECK_ID,
    READ_PLAIN_ID,
    READ_CURLY_ID,
    READ_POST
  }
  state = READ_PRE;

  // parse s
  for(string::size_type i = 0; i < s.size(); i++)
  {
    // get current character
    unsigned char uc = static_cast<unsigned char>(s[i]);

    // READ_PRE
    if (state == READ_PRE)
    {
      // escape sequence found
      if (uc == '\\')
      {
        // don't replace escape sequences yet
        a += uc;

        // set next state
        state = READ_ESC;
      }

      // ID started
      else if (uc == '$')
      {
        // set next state
        state = CHECK_ID;
      }

      // verbatim character found
      else
      {
        // append character
        a += uc;
      }
    }

    // READ_ESC
    else if (state == READ_ESC)
    {
      // append this character unparsed
      a += uc;

      // back to initial state
      state = READ_PRE;
    }

    // CHECK_ID
    else if (state == CHECK_ID)
    {
      // curly ID found
      if (uc == '{')
      {
        // set next state
        state = READ_CURLY_ID;
      }

      // plain ID found
      else if (  (uc == '_')
      ||        ((uc >= 'A') && (uc <= 'Z'))
      ||        ((uc >= 'a') && (uc <= 'z'))
      ||        ((uc >= '0') && (uc <= '9')) )
      {
        // append character
        id += uc;

        // set next state
        state = READ_PLAIN_ID;
      }

      // invalid syntax
      else
      {
        // notify user
        msg::err( msg::catq("invalid syntax: ", s) );

        // signalize trouble
        return false;
      }
    }

    // READ_PLAIN_ID
    else if (state == READ_PLAIN_ID)
    {
      // ID continued
      if (  (uc == '_')
      ||   ((uc >= 'A') && (uc <= 'Z'))
      ||   ((uc >= 'a') && (uc <= 'z'))
      ||   ((uc >= '0') && (uc <= '9')) )
      {
        // append character
        id += uc;
      }

      // ID finished
      else
      {
        // append character
        b += uc;

        // set next state
        state = READ_POST;
      }
    }

    // READ_CURLY_ID
    else if (state == READ_CURLY_ID)
    {
      // ID continued
      if (  (uc == '_')
      ||   ((uc >= 'A') && (uc <= 'Z'))
      ||   ((uc >= 'a') && (uc <= 'z'))
      ||   ((uc >= '0') && (uc <= '9')) )
      {
        // append character
        id += uc;
      }

      // ID finished
      else if (uc == '}')
      {
        // set next state
        state = READ_POST;
      }

      // invalid syntax
      else
      {
        // notify user
        msg::err( msg::catq("invalid syntax: ", s) );

        // signalize trouble
        return false;
      }
    }

    // READ_POST
    else if (state == READ_POST)
    {
      // don't parse any characters in this state
      b += uc;
    }
  }

  // check final state
  if ( (state == READ_ESC)
  ||   (state == CHECK_ID)
  ||   (state == READ_CURLY_ID) )
  {
    // notify user
    msg::err( msg::catq("invalid syntax: ", s) );

    // signalize trouble
    return false;
  }

  // signalize success
  return true;
}

// --------
// getValue
// --------
/*
 *
 */
string ReplaceHandler::getValue(const string& key) const
{
  // search key
  for(unsigned i = 0; i < m_keys.size(); i++)
  {
    // key found
    if (m_keys[i] == key)
    {
      return m_values[i];
    }
  }

  // key not found
  return "";
}

// -------
// isFinal
// -------
/*
 *
 */
bool ReplaceHandler::isFinal(const string& value) const
{
  // parts of the given value
  string a, id, b;

  // split given value
  if ( !split(value, a, id, b) )
  {
    // invalid syntax
    return false;
  }

  // value is final
  return id.empty();
}

