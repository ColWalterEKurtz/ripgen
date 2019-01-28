// -----------------------------------------------------------------------------
// ChainHandler.cpp                                             ChainHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref ChainHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-07
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include "ChainHandler.h"


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// ------------
// ChainHandler
// ------------
/*
 *
 */
ChainHandler::ChainHandler(KVHandler* next)
: m_next(next)
{
  // nothing
}


// -----------------------------------------------------------------------------
// Initialization                                                 Initialization
// -----------------------------------------------------------------------------

// --------------
// setNextHandler
// --------------
/*
 *
 */
void ChainHandler::setNextHandler(KVHandler* next)
{
  m_next = next;
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
void ChainHandler::OnBeginParsing(const string& filename)
{
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
void ChainHandler::OnEndParsing(bool healthy)
{
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
void ChainHandler::OnData(const string& key, const string& value)
{
  if (m_next)
  {
    m_next->OnData(key, value);
  }
}


// -----------------------------------------------------------------------------
// Status information                                         Status information
// -----------------------------------------------------------------------------

// -------
// healthy
// -------
/*
 *
 */
bool ChainHandler::healthy() const
{
  // check if this handler got stuck itself
  if ( !KVHandler::healthy() )
  {
    return false;
  }

  // check if next handler got stuck
  if (m_next)
  {
    return m_next->healthy();
  }

  // everything ok
  return true;
}

