// -----------------------------------------------------------------------------
// KVParser.h                                                         KVParser.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref KVParser class.
 * @author     Col. Walter E. Kurtz
 * @version    2018-03-08
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef KVPARSER_H_INCLUDE_NO1
#define KVPARSER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>
#include <istream>


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// --------
// KVParser
// --------
/*
 * forward declaration
 */
class KVHandler;


// --------
// KVParser
// --------
/**
 * @brief  This class knows how to parse 'KEY=VALUE' files.
 */
class KVParser
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // --------
  // KVParser
  // --------
  /**
   * @brief  The standard-constructor.
   */
  KVParser();


  // ---------------------------------------------------------------------------
  // Initialization                                               Initialization
  // ---------------------------------------------------------------------------

  // ----------
  // setHandler
  // ----------
  /**
   *
   */
  void setHandler(KVHandler* handler);


  // ---------------------------------------------------------------------------
  // Handling                                                           Handling
  // ---------------------------------------------------------------------------

  // -----
  // parse
  // -----
  /**
   * This method parses data from stdin.
   */
  bool parse();

  // -----
  // parse
  // -----
  /**
   * This method parses data from the given text file.
   */
  bool parse(const string& filename);


protected:

  // ---------------------------------------------------------------------------
  // Internal methods                                           Internal methods
  // ---------------------------------------------------------------------------

  // -------------------
  // isKeyStartCharacter
  // -------------------
  /**
   *
   */
  bool isKeyStartCharacter(char c) const;

  // --------------
  // isKeyCharacter
  // --------------
  /**
   *
   */
  bool isKeyCharacter(char c) const;

  // -----------
  // parseStream
  // -----------
  /**
   *
   */
  bool parseStream(istream& stream);

  // -----
  // error
  // -----
  /**
   *
   */
  void error(const string& msg);

  // -----
  // error
  // -----
  /**
   *
   */
  void error(const string& msg, const string& bad);

  // -----
  // error
  // -----
  /**
   *
   */
  void error(unsigned lno, const string& msg);

  // -----
  // error
  // -----
  /**
   *
   */
  void error(unsigned lno, const string& msg, char bad);


private:

  // ---------------------------------------------------------------------------
  // Attributes                                                       Attributes
  // ---------------------------------------------------------------------------

  /// the name of the file that is currently parsed
  string m_filename;

  /// this handler wiil receive all messages
  KVHandler* m_handler;

};

#endif  /* #ifndef KVPARSER_H_INCLUDE_NO1 */

