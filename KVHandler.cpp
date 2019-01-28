// -----------------------------------------------------------------------------
// KVHandler.cpp                                                   KVHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref KVHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-06-20
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include "KVHandler.h"


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// ---------
// KVHandler
// ---------
/*
 *
 */
KVHandler::KVHandler()
{
  // start in healthy state
  m_healthy = true;
}

// ----------
// ~KVHandler
// ----------
/*
 *
 */
KVHandler::~KVHandler()
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
void KVHandler::OnBeginParsing(const string& filename)
{
  // nothing
}

// ------------
// OnEndParsing
// ------------
/*
 *
 */
void KVHandler::OnEndParsing(bool healthy)
{
  // nothing
}

// ------
// OnData
// ------
/*
 *
 */
void KVHandler::OnData(const string& key, const string& value)
{
  // nothing
}


// -----------------------------------------------------------------------------
// Internal methods                                             Internal methods
// -----------------------------------------------------------------------------

// ----------
// setHealthy
// ----------
/*
 *
 */
void KVHandler::setHealthy(bool healthy)
{
  m_healthy = healthy;
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
bool KVHandler::healthy() const
{
  return m_healthy;
}

