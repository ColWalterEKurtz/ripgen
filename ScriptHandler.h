// -----------------------------------------------------------------------------
// ScriptHandler.h                                               ScriptHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref ScriptHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef SCRIPTHANDLER_H_INCLUDE_NO1
#define SCRIPTHANDLER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include "KVHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -------------
// ScriptHandler
// -------------
/**
 * @brief  The best class in the world.
 */
class ScriptHandler : public KVHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // -------------
  // ScriptHandler
  // -------------
  /**
   * @brief  The standard-constructor.
   */
  ScriptHandler();


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

  // -------
  // dirname
  // -------
  /**
   *
   */
  string dirname(const string& path) const;

  // -----
  // quote
  // -----
  /**
   *
   */
  string quote(const string& line, bool outer = true) const;

  // --------
  // getValue
  // --------
  /**
   *
   */
  string getValue(const string& key) const;

  // ------------------
  // bufferFileCommands
  // ------------------
  /**
   *
   */
  void bufferFileCommands();

  // -----------
  // beginScript
  // -----------
  /**
   *
   */
  void beginScript() const;

  // ------------------
  // printCheckCommands
  // ------------------
  /**
   *
   */
  void printCheckCommands() const;

  // ---------
  // endScript
  // ---------
  /**
   *
   */
  void endScript() const;


private:

  // ---------------------------------------------------------------------------
  // Attributes                                                       Attributes
  // ---------------------------------------------------------------------------

  /// buffer of keys
  vector<string> m_keys;

  /// buffer of values
  vector<string> m_values;

  /// images to check
  set<string> m_ichecks;

  /// directories to check
  set<string> m_dchecks;

  /// files to check
  set<string> m_fchecks;

  /// buffer for all file commands
  stringstream m_fcbuffer;
};

#endif  /* #ifndef SCRIPTHANDLER_H_INCLUDE_NO1 */

