// -----------------------------------------------------------------------------
// FilterHandler.cpp                                           FilterHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref FilterHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-12-01
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include "message.h"
#include "keyinfo.h"
#include "FilterHandler.h"


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// -------------
// FilterHandler
// -------------
/*
 *
 */
FilterHandler::FilterHandler(KVHandler* next)
: ChainHandler(next)
{
  // nothing
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
void FilterHandler::OnBeginParsing(const string& filename)
{
  // reset flag
  setHealthy();

  // notify next handler
  if (m_next)
  {
    return m_next->OnBeginParsing(filename);
  }
}

// ------
// OnData
// ------
/*
 *
 */
void FilterHandler::OnData(const string& key, const string& value)
{
  // no handler set
  if (m_next == 0) return;

  // invalid state
  if ( !healthy() ) return;

  // let writable and internal keys pass
  if ( keyinfo::isWritable(key) || (key[0] == '_') )
  {
    // notify next handler
    m_next->OnData(key, value);
  }

  else
  {
    // notify user
    msg::err( msg::catq("key is not writable: ", key) );

    // update flag
    setHealthy(false);
  }
}

