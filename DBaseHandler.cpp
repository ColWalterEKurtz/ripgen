// -----------------------------------------------------------------------------
// DBaseHandler.cpp                                             DBaseHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref DBaseHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-28
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <iostream>
#include "keyinfo.h"
#include "DBaseHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// ------------
// DBaseHandler
// ------------
/*
 *
 */
DBaseHandler::DBaseHandler()
{
  // initialization
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
void DBaseHandler::OnBeginParsing(const string& filename)
{
  // update healthy flag
  setHealthy();

  // set current filename
  m_filename = filename;

  // empty buffers
  m_keys.clear();
  m_values.clear();
}

// ------------
// OnEndParsing
// ------------
/*
 *
 */
void DBaseHandler::OnEndParsing(bool healthy)
{
  // reset filename
  m_filename = "";

  // empty buffers
  m_keys.clear();
  m_values.clear();
}

// ------
// OnData
// ------
/*
 *
 */
void DBaseHandler::OnData(const string& key, const string& value)
{
  // don't run in bad state
  if ( !healthy() ) return;

  // buffer tag
  m_keys.push_back(key);
  m_values.push_back(value);

  // trigger found
  if (key == "COMPILATIONINDEX")
  {
    // create output
    printBuffer();

    // empty buffers
    m_keys.clear();
    m_values.clear();
  }
}


// -----------------------------------------------------------------------------
// Internal methods                                             Internal methods
// -----------------------------------------------------------------------------

// --------
// getValue
// --------
/*
 *
 */
string DBaseHandler::getValue(const string& key) const
{
  // find related value
  for(unsigned i = 0; i < m_keys.size(); i++)
  {
    if (m_keys[i] == key)
    {
      return m_values[i];
    }
  }

  // key not found
  return "";
}

// -----------
// printBuffer
// -----------
/*
 *
 */
void DBaseHandler::printBuffer() const
{
  vector<string> order;
  order.push_back( keyinfo::name(keyinfo::COMPILATIONID)    );
  order.push_back( keyinfo::name(keyinfo::COMPILATIONINDEX) );
  order.push_back( keyinfo::name(keyinfo::IMAGE)            );
  order.push_back( keyinfo::name(keyinfo::AUTHOR)           );
  order.push_back( keyinfo::name(keyinfo::COMPOSER)         );
  order.push_back( keyinfo::name(keyinfo::LYRICIST)         );
  order.push_back( keyinfo::name(keyinfo::OPUS)             );
  order.push_back( keyinfo::name(keyinfo::VERSION)          );
  order.push_back( keyinfo::name(keyinfo::ARRANGER)         );
  order.push_back( keyinfo::name(keyinfo::PERFORMER)        );
  order.push_back( keyinfo::name(keyinfo::CONDUCTOR)        );
  order.push_back( keyinfo::name(keyinfo::ENSEMBLE)         );
  order.push_back( keyinfo::name(keyinfo::ALBUMARTIST)      );
  order.push_back( keyinfo::name(keyinfo::ALBUM)            );
  order.push_back( keyinfo::name(keyinfo::GENRE)            );
  order.push_back( keyinfo::name(keyinfo::DATE)             );
  order.push_back( keyinfo::name(keyinfo::TRACKTOTAL)       );
  order.push_back( keyinfo::name(keyinfo::TRACKNUMBER)      );
  order.push_back( keyinfo::name(keyinfo::ARTIST)           );
  order.push_back( keyinfo::name(keyinfo::TITLE)            );
  order.push_back( keyinfo::name(keyinfo::COMMENT)          );
  order.push_back( keyinfo::name(keyinfo::FILENAME)         );

  // start with the name of the source file
  cout << "|CDFILE=" << m_filename;

  // print title related information
  for(unsigned i = 0; i < order.size(); i++)
  {
    cout  << "|" << order[i] << "=" << getValue(order[i]);
  }

  cout << "|" << endl;
}

