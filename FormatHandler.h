// -----------------------------------------------------------------------------
// FormatHandler.h                                               FormatHandler.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref FormatHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-23
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef FORMATHANDLER_H_INCLUDE_NO1
#define FORMATHANDLER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>
#include "ChainHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -------------
// FormatHandler
// -------------
/**
 * @brief  The best class in the world.
 */
class FormatHandler : public ChainHandler
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // -------------
  // FormatHandler
  // -------------
  /**
   * @brief  The standard-constructor.
   */
  FormatHandler(KVHandler* next = 0);


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

  // ------
  // format
  // ------
  /**
   *
   */
  bool format(const string& text, string& formatted) const;

  // --------
  // evaluate
  // --------
  /**
   *
   */
  bool evaluate( unsigned char  function,
                 const string&  quantifier,
                 const string&  argument,
                 string&        result
               ) const;

  // ---------------
  // fmtLeadingZeros
  // ---------------
  /**
   *
   */
  bool fmtLeadingZeros(unsigned number, const string& in, string& out) const;
 
  // ----------------
  // fmtTrailingZeros
  // ----------------
  /**
   *
   */
  bool fmtTrailingZeros(unsigned number, const string& in, string& out) const;
 
  // -------------
  // fmtCommaSplit
  // -------------
  /**
   *
   */
  bool fmtCommaSplit(unsigned number, const string& in, string& out) const;
 
  // -----------------
  // fmtSemicolonSplit
  // -----------------
  /**
   *
   */
  bool fmtSemicolonSplit(unsigned number, const string& in, string& out) const;
 
  // -------
  // fmtTrim
  // -------
  /**
   *
   */
  bool fmtTrim(unsigned number, const string& in, string& out) const;

  // ----------
  // fmtSqueeze
  // ----------
  /**
   *
   */
  bool fmtSqueeze(unsigned number, const string& in, string& out) const;

  // -----------
  // fmtFilename
  // -----------
  /**
   *
   */
  bool fmtFilename(unsigned number, const string& in, string& out) const;

};

#endif  /* #ifndef FORMATHANDLER_H_INCLUDE_NO1 */

