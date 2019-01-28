// -----------------------------------------------------------------------------
// ReplaceHandler.h                                             ReplaceHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref ReplaceHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef REPLACEHANDLER_H_INCLUDE_NO1
#define REPLACEHANDLER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <vector>
#include <string>
#include "ChainHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// --------------
// ReplaceHandler
// --------------
/**
 * @brief  The best class in the world.
 */
class ReplaceHandler : public ChainHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // --------------
  // ReplaceHandler
  // --------------
  /**
   * @brief  The standard-constructor.
   */
  ReplaceHandler(KVHandler* next = 0);


  // ---------------------------------------------------------------------------
  // Callback handler                                           Callback handler
  // ---------------------------------------------------------------------------

  // --------------
  // OnBeginParsing
  // --------------
  /**
   *
   */
  virtual void OnBeginParsing(const string& filename);

  // ------------
  // OnEndParsing
  // ------------
  /**
   *
   */
  virtual void OnEndParsing(bool healthy);

  // ------
  // OnData
  // ------
  /**
   *
   */
  virtual void OnData(const string& key, const string& value);


protected:

  // ---------------------------------------------------------------------------
  // Internal methods                                           Internal methods
  // ---------------------------------------------------------------------------

  // ----------
  // replaceAll
  // ----------
  /**
   *
   */
  bool replaceAll();

  // -----
  // split
  // -----
  /**
   *
   */
  bool split(const string& s, string& a, string& id, string& b) const;

  // --------
  // getValue
  // --------
  /**
   *
   */
  string getValue(const string& key) const;

  // -------
  // isFinal
  // -------
  /**
   *
   */
  bool isFinal(const string& value) const;


private:

  // ---------------------------------------------------------------------------
  // Attributes                                                       Attributes
  // ---------------------------------------------------------------------------

  /// list of keys
  vector<string> m_keys;

  /// list of values
  vector<string> m_values;

};

#endif  /* #ifndef REPLACEHANDLER_H_INCLUDE_NO1 */

