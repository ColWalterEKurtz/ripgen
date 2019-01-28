// -----------------------------------------------------------------------------
// main.cpp                                                             main.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the main() function and doxygen's main page.
 * @author     Col. Walter E. Kurtz
 * @version    2016-12-01
 * @copyright  GNU General Public License - Version 3.0
 *
 * @mainpage
 *
 * @section issue What is the issue here?
 *   Lorem ipsum dolor sit amet, consectetur adipisici elit, sed eiusmod
 *   tempor incidunt ut labore et dolore magna aliqua. Ut enim ad minim
 *   veniam, quis nostrud exercitation ullamco laboris nisi ut aliquid
 *   ex ea commodi consequat. Quis aute iure reprehenderit in voluptate
 *   velit esse cillum dolore eu fugiat nulla pariatur.
 *   Excepteur sint obcaecat cupiditat non proident, sunt in culpa qui
 *   officia deserunt mollit anim id est laborum.
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>
#include <iomanip>
#include <iostream>
#include "cli.h"
#include "keyinfo.h"
#include "scripts.h"
#include "KVParser.h"
#include "TestHandler.h"
#include "FilterHandler.h"
#include "StackHandler.h"
#include "ReplaceHandler.h"
#include "FormatHandler.h"
#include "UnescapeHandler.h"
#include "ScriptHandler.h"
#include "OverviewHandler.h"
#include "DBaseHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// ------------
// createOutput
// ------------
/**
 *
 */
bool createOutput(KVHandler& consumer, const string& filename)
{
  // create common process chain
  UnescapeHandler h5(&consumer);
  FormatHandler   h4(&h5);
  ReplaceHandler  h3(&h4);
  StackHandler    h2(&h3);
  FilterHandler   h1(&h2);

  // create parser
  KVParser parser;
  parser.setHandler(&h1);

  // no filename given (read from stdin)
  if ( filename.empty() )
  {
    // one byte from stdin
    char c;

    // buffer of characters
    string buffer;

    // get characters from stdin
    while( cin.get(c) )
    {
      // delimiter found
      if (c == 0)
      {
        // parse given file
        if ( !parser.parse(buffer) )
        {
          return false;
        }

        // reset buffer
        buffer = "";
      }

      else
      {
        // append character
        buffer += c;
      }
    }

    // check last filename
    if ( !buffer.empty() )
    {
      // parse given file
      if ( !parser.parse(buffer) )
      {
        return false;
      }
    }
  }

  // use given filename
  else
  {
    // parse given file
    if ( !parser.parse(filename) )
    {
      return false;
    }
  }

  // get healthy state
  return h1.healthy();
}

// -------------
// showTagScript
// -------------
/**
 *
 */
bool showTagScript(const string& filename)
{
  // define output format
  ScriptHandler consumer;

  // run operation
  return createOutput(consumer, filename);
}

// -----------------
// showBriefOverview
// -----------------
/**
 *
 */
bool showBriefOverview(const string& filename)
{
  // define output format
  OverviewHandler consumer;

  // run operation
  return createOutput(consumer, filename);
}

// -------------------
// showVerboseOverview
// -------------------
/**
 *
 */
bool showVerboseOverview(const string& filename)
{
  // define output format
  OverviewHandler consumer(true);

  // run operation
  return createOutput(consumer, filename);
}

// --------------
// showDBaseLines
// --------------
/**
 *
 */
bool showDBaseLines(const string& filename)
{
  // define output format
  DBaseHandler consumer;

  // run operation
  return createOutput(consumer, filename);
}

// --------------
// showListOfKeys
// --------------
/**
 *
 */
void showListOfKeys()
{
  // show list of known keys
  for(unsigned i = 0; i < keyinfo::keyCount; i++)
  {
    cout << left;
    cout << setw(keyinfo::maxNameSize) << keyinfo::name(i) << " [";
    cout << (keyinfo::isWritable(i)      ? 'W' : '-');
    cout << (keyinfo::isVorbisComment(i) ? 'C' : '-');
    cout << "]";

    if ( (i == keyinfo::ALBUM) || (i == keyinfo::OPUS) )
    {
      cout << " this key resets TRACKNUMBER to 1";
    }

    else if (i == keyinfo::COMPILATIONID)
    {
      cout << " this key resets COMPILATIONINDEX to 1";
    }

    else if ( (i == keyinfo::COMPILATIONINDEX) || (i == keyinfo::TRACKNUMBER) )
    {
      cout << " always passed along with TITLE; automatically increased by TITLE (after printing)";
    }

    else if (i == keyinfo::IMAGE)
    {
      cout << " the filename of the image that will be included as cover (front)";
    }

    else if (i == keyinfo::TITLE)
    {
      cout << " this key prints the current stack";
      cout << " and increases TRACKNUMBER and COMPILATIONINDEX afterwards";
    }

    cout << endl;
  }

  // keys starting with an underscore
  cout << left << setw(keyinfo::maxNameSize) << "_ANYOTHERKEY";
  cout << " [W-] any other key has to start with an underscore";
  cout << endl;
}

// ----
// main
// ----
/**
 * @brief  The program starts in this function.
 *
 * @param argc  holds the number of passed command-line arguments.
 * @param argv  holds the list of passed command-line arguments.
 *
 * @return
 * Value | Meaning
 * ----: | :------
 *     0 | The requested operation finished successfully.
 *     1 | The requested operation failed.
 */
int main(int argc, char** argv)
{
  // create object to parse the passed arguments
  cli cmdl;

  // try to parse the command line
  if ( !cmdl.parse(argc, argv) )
  {
    // signalize trouble
    return 1;
  }

  // run default operation
  if (cmdl.operation == cli::DEFAULT)
  {
    // show tag script
    if ( !showTagScript(cmdl.filename) )
    {
      // signalize trouble
      return 1;
    }
  }

  // run special operation
  else
  {
    // show brief overview
    if (cmdl.operation == cli::SHOW_OVERVIEW_BRIEF)
    {
      if ( !showBriefOverview(cmdl.filename) )
      {
        // signalize trouble
        return 1;
      }
    }

    // show verbose overview
    else if (cmdl.operation == cli::SHOW_OVERVIEW_VERBOSE)
    {
      if ( !showVerboseOverview(cmdl.filename) )
      {
        // signalize trouble
        return 1;
      }
    }

    // show database lines
    else if (cmdl.operation == cli::SHOW_DBASE_LINES)
    {
      if ( !showDBaseLines(cmdl.filename) )
      {
        // signalize trouble
        return 1;
      }
    }

    // show rip script
    else if (cmdl.operation == cli::CREATE_SCRIPTS)
    {
      scripts::createAllScripts();
    }

    // show help
    else if (cmdl.operation == cli::SHOW_HELP)
    {
      cmdl.help(argv[0]);
    }

    // show version
    else if (cmdl.operation == cli::SHOW_VERSION)
    {
      cmdl.version();
    }

    // show keys
    else if (cmdl.operation == cli::SHOW_KEYS)
    {
      showListOfKeys();
    }

    // unknown operation
    else
    {
      // signalize trouble
      return 1;
    }
  }

  // signalize success
  return 0;
}

