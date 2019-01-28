// -----------------------------------------------------------------------------
// cli.h                                                                   cli.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref cli class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-25
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef CLI_H_INCLUDE_NO1
#define CLI_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>


// ---
// cli
// ---
/**
 * @brief  This class defines the command-line interface.
 */
class cli
{

public:

  // ---------------------------------------------------------------------------
  // Settings                                                           Settings
  // ---------------------------------------------------------------------------

  // operations that are mutually exclusive
  enum
  {
    DEFAULT,
    SHOW_HELP,
    SHOW_VERSION,
    SHOW_KEYS,
    SHOW_COMMANDS,
    SHOW_OVERVIEW_BRIEF,
    SHOW_OVERVIEW_VERBOSE,
    SHOW_DBASE_LINES,
    CREATE_SCRIPTS
  }
  operation;

  // where to get the filenames from
  enum
  {
    PARAM,
    STDIN
  }
  source;

  /// the name of the file to parse
  std::string filename;


  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // ---
  // cli
  // ---
  /**
   * @brief  The standard-constructor.
   */
  cli();


  // ---------------------------------------------------------------------------
  // Handling                                                           Handling
  // ---------------------------------------------------------------------------

  // ----
  // help
  // ----
  /**
   * @brief  This method shows the syntax of the command-line interface.
   */
  void help(const std::string& xname) const;

  // -------
  // version
  // -------
  /**
   * @brief  This method shows the program's version.
   */
  void version() const;

  // -----
  // parse
  // -----
  /**
   * @brief  This Method uses getopt() to parse the given arguments.
   */
  bool parse(int argc, char** argv);

};

#endif  /* #ifndef CLI_H_INCLUDE_NO1 */

