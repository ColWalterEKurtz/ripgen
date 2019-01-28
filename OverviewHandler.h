// -----------------------------------------------------------------------------
// OverviewHandler.h                                           OverviewHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref OverviewHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-09
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef OVERVIEWHANDLER_H_INCLUDE_NO1
#define OVERVIEWHANDLER_H_INCLUDE_NO1


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


// ---------------
// OverviewHandler
// ---------------
/**
 * @brief  The best class in the world.
 */
class OverviewHandler : public KVHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // ---------------
  // OverviewHandler
  // ---------------
  /**
   * @brief  The standard-constructor.
   */
  OverviewHandler(bool detailed = false);


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

  // ---------
  // showBrief
  // ---------
  /**
   *
   */
  void showBrief() const;

  // -----------
  // showVerbose
  // -----------
  /**
   *
   */
  void showVerbose() const;


private:

  // ---------------------------------------------------------------------------
  // Attributes                                                       Attributes
  // ---------------------------------------------------------------------------

  /// brief or verbose
  bool m_detailed;

  /// buffer of keys
  vector<string> m_keys;

  /// buffer of values
  vector<string> m_values;

};

#endif  /* #ifndef OVERVIEWHANDLER_H_INCLUDE_NO1 */

