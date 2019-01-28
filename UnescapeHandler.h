// -----------------------------------------------------------------------------
// UnescapeHandler.h                                           UnescapeHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref UnescapeHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef UNESCAPEHANDLER_H_INCLUDE_NO1
#define UNESCAPEHANDLER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>
#include "ChainHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// ---------------
// UnescapeHandler
// ---------------
/**
 * @brief  The best class in the world.
 */
class UnescapeHandler : public ChainHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // ---------------
  // UnescapeHandler
  // ---------------
  /**
   * @brief  The standard-constructor.
   */
  UnescapeHandler(KVHandler* next = 0);


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

  // --------
  // unescape
  // --------
  /**
   *
   */
  bool unescape(const string& s, string& plain) const;

};

#endif  /* #ifndef UNESCAPEHANDLER_H_INCLUDE_NO1 */

