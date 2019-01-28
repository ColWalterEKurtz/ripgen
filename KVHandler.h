// -----------------------------------------------------------------------------
// KVHandler.h                                                       KVHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref KVHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-12-01
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef KVHANDLER_H_INCLUDE_NO1
#define KVHANDLER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// ---------
// KVHandler
// ---------
/**
 * @brief  This class defines the messages a KVParser can send.
 */
class KVHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // ---------
  // KVHandler
  // ---------
  /**
   * @brief  The standard-constructor.
   */
  KVHandler();

  // ----------
  // ~KVHandler
  // ----------
  /**
   * @brief  The destructor is virtual.
   */
  virtual ~KVHandler();


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


  // ---------------------------------------------------------------------------
  // Status information                                       Status information
  // ---------------------------------------------------------------------------

  // -------
  // healthy
  // -------
  /**
   *
   */
  virtual bool healthy() const;


protected:

  // ---------------------------------------------------------------------------
  // Internal methods                                           Internal methods
  // ---------------------------------------------------------------------------

  // ----------
  // setHealthy
  // ----------
  /**
   *
   */
  void setHealthy(bool healthy = true);


private:

  // ---------------------------------------------------------------------------
  // Attributes                                                       Attributes
  // ---------------------------------------------------------------------------

  /// the handler's healthy state
  bool m_healthy;

};

#endif  /* #ifndef KVHANDLER_H_INCLUDE_NO1 */

