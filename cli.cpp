// -----------------------------------------------------------------------------
// cli.cpp                                                               cli.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref cli class.
 * @author     Col. Walter E. Kurtz
 * @version    2019-01-29
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <unistd.h>
#include <sstream>
#include <iostream>
#include "message.h"
#include "cli.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// ---
// cli
// ---
/*
 *
 */
cli::cli()
{
  // run default operation
  operation = DEFAULT;

  // get filename from command-line
  source = PARAM;
}


// -----------------------------------------------------------------------------
// Handling                                                             Handling
// -----------------------------------------------------------------------------

// ----
// help
// ----
/*
 *
 */
void cli::help(const std::string& xname) const
{
  // filename without leading directories
  string nodir;

  for(string::size_type i = 0; i < xname.size(); i++)
  {
    // check character
    if (xname[i] == '/')
    {
      // reset
      nodir = "";
    }

    else
    {
      // append character
      nodir += xname[i];
    }
  }

  // show syntax
  cout << endl;
  cout << nodir << " [options] [<filename>]" << endl;
  cout << endl;
  cout << "  -h  show help and exit" << endl;
  cout << "  -v  show version and exit" << endl;
  cout << "  -d  show database lines" << endl;
  cout << "  -k  show the list of keys" << endl;
  cout << "  -L  show the list of commands" << endl;
  cout << "  -o  show a brief overview" << endl;
  cout << "  -O  show a verbose overview" << endl;
  cout << "  -s  create auxiliary bash scripts and exit" << endl;
  cout << "  -z  read NUL terminated filenames from stdin" << endl;
  cout << endl;
}

// -------
// version
// -------
/*
 *
 */
void cli::version() const
{
  cout << endl;
  cout << "version 2019-01-29.1" << endl;
  cout << endl;
}

// -----
// parse
// -----
/*
 *
 */
bool cli::parse(int argc, char** argv)
{
  // don't print error messages
  opterr = 0;

  // ASCII code of the detected option
  int optchar;

  // parse all given options
  while ((optchar = getopt(argc, argv, ":hvdkLoOsz")) != -1)
  {
    // use this object to convert arguments
    stringstream argstream((optarg == 0) ? "" : optarg);

    // the ID of the script to generate
    string scriptid;

    // analyze (short) options
    switch (optchar)
    {
      case 'h': operation = SHOW_HELP;

                // stop parsing
                return true;

      case 'v': operation = SHOW_VERSION;

                // stop parsing
                return true;

      case 'd': operation = SHOW_DBASE_LINES;

                // next option
                break;

      case 'k': operation = SHOW_KEYS;

                // stop parsing
                return true;

      case 'L': operation = SHOW_COMMANDS;

                // stop parsing
                return true;

      case 'o': operation = SHOW_OVERVIEW_BRIEF;

                // next option
                break;

      case 'O': operation = SHOW_OVERVIEW_VERBOSE;

                // next option
                break;

      case 's': operation = CREATE_SCRIPTS;

                // stop parsing
                return true;

      case 'z': source = STDIN;

                // next option
                break;

      case ':': msg::err("missing argument");

                // signalize trouble
                return false;

      case '?': msg::err("unknown option");

                // signalize trouble
                return false;
    }
  }

  // check source
  if (source == PARAM)
  {
    // get number of positional arguments
    unsigned pcount = (argc - optind);

    // check number of positional arguments
    if (pcount != 1)
    {
      if (pcount < 1)
      {
        // notify user
        msg::err("filename missing");
      }

      else
      {
        // notify user
        msg::err("too many filenames given");
      }

      // signalize trouble
      return false;
    }

    // get filename of the file to parse
    filename = argv[optind];
  }

  // signalize success
  return true;
}

