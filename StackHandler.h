// -----------------------------------------------------------------------------
// StackHandler.h                                                 StackHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref StackHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef STACKHANDLER_H_INCLUDE_NO1
#define STACKHANDLER_H_INCLUDE_NO1


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


// ------------
// StackHandler
// ------------
/**
 * @brief  The best class in the world.
 */
class StackHandler : public ChainHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // ------------
  // StackHandler
  // ------------
  /**
   * @brief  The standard-constructor.
   */
  StackHandler(KVHandler* next = 0);


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

  // ------------
  // str2unsigned
  // ------------
  /**
   *
   */
  bool str2unsigned(const string& s, unsigned& u) const;


private:

  // ---------------------------------------------------------------------------
  // Attributes                                                       Attributes
  // ---------------------------------------------------------------------------

  /// stack of keys
  vector<string> m_kstack;

  /// stack of values
  vector<string> m_vstack;

  /// compilation index
  unsigned m_cmpindex;

  /// track number
  unsigned m_tracknum;

};

#endif  /* #ifndef STACKHANDLER_H_INCLUDE_NO1 */

