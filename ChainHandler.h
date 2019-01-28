// -----------------------------------------------------------------------------
// ChainHandler.h                                                 ChainHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref ChainHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-07
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef CHAINHANDLER_H_INCLUDE_NO1
#define CHAINHANDLER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include "KVHandler.h"


// ------------
// ChainHandler
// ------------
/**
 * @brief  The best class in the world.
 */
class ChainHandler : public KVHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // ------------
  // ChainHandler
  // ------------
  /**
   * @brief  The standard-constructor.
   */
  ChainHandler(KVHandler* next = 0);


  // ---------------------------------------------------------------------------
  // Initialization                                               Initialization
  // ---------------------------------------------------------------------------

  // --------------
  // setNextHandler
  // --------------
  /**
   *
   */
  void setNextHandler(KVHandler* next);


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
  // Attributes                                                       Attributes
  // ---------------------------------------------------------------------------

  /// the next handler in the process chain
  KVHandler* m_next;

};

#endif  /* #ifndef CHAINHANDLER_H_INCLUDE_NO1 */

