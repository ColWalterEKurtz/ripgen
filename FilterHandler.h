// -----------------------------------------------------------------------------
// FilterHandler.h                                               FilterHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref FilterHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef FILTERHANDLER_H_INCLUDE_NO1
#define FILTERHANDLER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include "ChainHandler.h"


// -------------
// FilterHandler
// -------------
/**
 * @brief  The best class in the world.
 */
class FilterHandler : public ChainHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // -------------
  // FilterHandler
  // -------------
  /**
   * @brief  The standard-constructor.
   */
  FilterHandler(KVHandler* next = 0);


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

};

#endif  /* #ifndef FILTERHANDLER_H_INCLUDE_NO1 */

