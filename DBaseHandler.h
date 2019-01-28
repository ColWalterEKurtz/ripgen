// -----------------------------------------------------------------------------
// DBaseHandler.h                                                 DBaseHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref DBaseHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-28
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef DBASEHANDLER_H_INCLUDE_NO1
#define DBASEHANDLER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <vector>
#include <string>
#include "KVHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// ------------
// DBaseHandler
// ------------
/**
 * @brief  The best class in the world.
 */
class DBaseHandler : public KVHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // ------------
  // DBaseHandler
  // ------------
  /**
   * @brief  The standard-constructor.
   */
  DBaseHandler();


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

  // --------
  // getValue
  // --------
  /**
   *
   */
  string getValue(const string& key) const;

  // -----------
  // printBuffer
  // -----------
  /**
   *
   */
  void printBuffer() const;


private:

  // ---------------------------------------------------------------------------
  // Attributes                                                       Attributes
  // ---------------------------------------------------------------------------

  /// the file that is currently parsed
  string m_filename;

  /// buffer of keys
  vector<string> m_keys;

  /// buffer of values
  vector<string> m_values;

};

#endif  /* #ifndef DBASEHANDLER_H_INCLUDE_NO1 */

