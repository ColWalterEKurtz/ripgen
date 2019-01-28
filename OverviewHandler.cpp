// -----------------------------------------------------------------------------
// OverviewHandler.cpp                                       OverviewHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref OverviewHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-09
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include "keyinfo.h"
#include "OverviewHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// ---------------
// OverviewHandler
// ---------------
/*
 *
 */
OverviewHandler::OverviewHandler(bool detailed)
{
  // set verbosity level
  m_detailed = detailed;
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
void OverviewHandler::OnBeginParsing(const string& filename)
{
  // update healthy flag
  setHealthy();

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
void OverviewHandler::OnEndParsing(bool healthy)
{
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
void OverviewHandler::OnData(const string& key, const string& value)
{
  // don't run in bad state
  if ( !healthy() ) return;

  // buffer tag
  m_keys.push_back(key);
  m_values.push_back(value);

  // trigger found
  if ( key == keyinfo::name(keyinfo::COMPILATIONINDEX) )
  {
    if (m_detailed)
    {
      showVerbose();
    }

    else
    {
      showBrief();
    }

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
string OverviewHandler::getValue(const string& key) const
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

// ---------
// showBrief
// ---------
/*
 *
 */
void OverviewHandler::showBrief() const
{
  // get album and track artist
  string aa = getValue( keyinfo::name(keyinfo::ALBUMARTIST) );
  string ta = getValue( keyinfo::name(keyinfo::ARTIST) );

  // same artist
  if (aa == ta)
  {
    // print one line
    cout << setw(3) << right
         << getValue( keyinfo::name(keyinfo::COMPILATIONINDEX) ) << ". "
         << getValue( keyinfo::name(keyinfo::ALBUMARTIST)      ) << " - "
         << getValue( keyinfo::name(keyinfo::ALBUM)            ) << " - ["
         << getValue( keyinfo::name(keyinfo::TRACKNUMBER)      ) << "] "
         << getValue( keyinfo::name(keyinfo::TITLE)            ) << endl;
  }

  // different artists
  else
  {
    // print one line
    cout << setw(3) << right
         << getValue( keyinfo::name(keyinfo::COMPILATIONINDEX) ) << ". "
         << getValue( keyinfo::name(keyinfo::ALBUMARTIST)      ) << " - "
         << getValue( keyinfo::name(keyinfo::ALBUM)            ) << " - ["
         << getValue( keyinfo::name(keyinfo::TRACKNUMBER)      ) << "] "
         << getValue( keyinfo::name(keyinfo::ARTIST)           ) << " - "
         << getValue( keyinfo::name(keyinfo::TITLE)            ) << endl;
  }
}

// -----------
// showVerbose
// -----------
/*
 *
 */
void OverviewHandler::showVerbose() const
{
  // print comments in this order
  vector<string> order;
  order.push_back( keyinfo::name(keyinfo::IMAGE)            );
  order.push_back( keyinfo::name(keyinfo::COMPILATIONID)    );
  order.push_back( keyinfo::name(keyinfo::COMPILATIONINDEX) );
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

  showBrief();

  // show sequence
  for(unsigned n = 0; n < order.size(); n++)
  {
    cout << setw(21) << right
         << order[n]
         << "="
         << getValue(order[n])
         << endl;
  }

  // add empty line
  cout << endl;
}

