// -----------------------------------------------------------------------------
// StackHandler.cpp                                             StackHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref StackHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <sstream>
#include "message.h"
#include "keyinfo.h"
#include "StackHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// ------------
// StackHandler
// ------------
/*
 *
 */
StackHandler::StackHandler(KVHandler* next)
: ChainHandler(next)
{
  m_cmpindex = 1;
  m_tracknum = 1;
}


// -----------------------------------------------------------------------------
// Callback handler                                             Callback handler
// -----------------------------------------------------------------------------

// --------------
// OnBeginParsing
// --------------
/*
 *
 */
void StackHandler::OnBeginParsing(const string& filename)
{
  // reset healthy flag
  setHealthy();

  // reset stacks
  m_kstack.clear();
  m_vstack.clear();

  // reset counters
  m_cmpindex = 1;
  m_tracknum = 1;

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
void StackHandler::OnEndParsing(bool healthy)
{
  // empty stacks
  m_kstack.clear();
  m_vstack.clear();

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
void StackHandler::OnData(const string& key, const string& value)
{
  // no handler set
  if (m_next == 0) return;

  // invalid state
  if ( !healthy() ) return;

  // cut stacks
  for(unsigned i = 0; i < m_kstack.size(); i++)
  {
    // key found
    if (m_kstack[i] == key)
    {
      // cut stacks here
      m_kstack.resize(i);
      m_vstack.resize(i);

      // exit loop
      break;
    }
  }

  // don't add empty values
  if ( value.empty() ) return;

  // new track number passed
  if (key == keyinfo::name(keyinfo::TRACKNUMBER))
  {
    // convert to unsigned
    if ( str2unsigned(value, m_tracknum) )
    {
      // check number
      if (m_tracknum > 999)
      {
        // notify user
        msg::wrn( msg::cat("large track number found: ", msg::str(m_tracknum)) );
      }
    }

    else
    {
      // notify user
      msg::err( msg::cat("invalid track number found: ", value) );

      // update healthy flag
      setHealthy(false);
    }
  }

  // new compilation ID passed
  else if (key == keyinfo::name(keyinfo::COMPILATIONID))
  {
    // append data
    m_kstack.push_back(key);
    m_vstack.push_back(value);

    // reset compilation index
    m_cmpindex = 1;
  }

  // new album or opus passed
  else if ( (key == keyinfo::name(keyinfo::ALBUM))
  ||        (key == keyinfo::name(keyinfo::OPUS)) )
  {
    // append data
    m_kstack.push_back(key);
    m_vstack.push_back(value);

    // reset track number
    m_tracknum = 1;
  }

  // new title passed
  else if (key == keyinfo::name(keyinfo::TITLE))
  {
    // append TITLE data
    m_kstack.push_back(key);
    m_vstack.push_back(value);

    // numbers as strings
    string tracknum;
    string cmpindex;

    // convert unsigned to string
    stringstream conv;
    conv << m_tracknum << " " << m_cmpindex;
    conv >> tracknum >> cmpindex;

    // always append TRACKNUMBER
    m_kstack.push_back( keyinfo::name(keyinfo::TRACKNUMBER) );
    m_vstack.push_back(tracknum);

    // always append COMPILATIONINDEX
    // this key triggers the operation of next handlers
    m_kstack.push_back( keyinfo::name(keyinfo::COMPILATIONINDEX) );
    m_vstack.push_back(cmpindex);

    // flush all buffered values
    for(unsigned n = 0; n < m_kstack.size(); n++)
    {
      // notify next handler
      m_next->OnData(m_kstack[n], m_vstack[n]);

      // check healthy state
      if ( !(m_next->healthy()) )
      {
        // update healthy flag
        setHealthy(false);

        // exit method
        return;
      }
    }

    // increase numbers
    m_cmpindex += 1;
    m_tracknum += 1;
  }

  // append other tags
  else
  {
    // append data
    m_kstack.push_back(key);
    m_vstack.push_back(value);
  }
}


// -----------------------------------------------------------------------------
// Internal methods                                             Internal methods
// -----------------------------------------------------------------------------

// ------------
// str2unsigned
// ------------
/*
 *
 */
bool StackHandler::str2unsigned(const string& s, unsigned& u) const
{
  // initialize string stream
  stringstream conv(s);

  // don't change u
  unsigned temp;

  // try to convert given string
  if (conv >> temp)
  {
    // set valid number
    u = temp;

    // signalize success
    return true;
  }

  // invalid number given
  return false;
}

