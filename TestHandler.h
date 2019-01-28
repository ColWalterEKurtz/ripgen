// -----------------------------------------------------------------------------
// TestHandler.h                                                   TestHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref TestHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-06-20
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef TESTHANDLER_H_INCLUDE_NO1
#define TESTHANDLER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>
#include "KVHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------
// TestHandler
// -----------
/**
 * @brief  A class to test the messages sent by KVParser.
 */
class TestHandler : public KVHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // -----------
  // TestHandler
  // -----------
  /**
   * @brief  The standard-constructor.
   */
  TestHandler();


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

};

#endif  /* #ifndef TESTHANDLER_H_INCLUDE_NO1 */

