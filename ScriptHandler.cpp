// -----------------------------------------------------------------------------
// ScriptHandler.cpp                                           ScriptHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref ScriptHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-25
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include "keyinfo.h"
#include "ScriptHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// -------------
// ScriptHandler
// -------------
/*
 *
 */
ScriptHandler::ScriptHandler()
{
  // nothing
}


// -----------------------------------------------------------------------------
// Callback handler                                             Callback handler
// -----------------------------------------------------------------------------

// --------------
// OnBeginParsing
// --------------
/*
 *
 */
void ScriptHandler::OnBeginParsing(const string& filename)
{
  // update healthy flag
  setHealthy();

  // empty buffers
  m_keys.clear();
  m_values.clear();
  m_ichecks.clear();
  m_dchecks.clear();
  m_fchecks.clear();

  // reset stringstream
  m_fcbuffer.str("");
  m_fcbuffer.clear();
}

// ------------
// OnEndParsing
// ------------
/*
 *
 */
void ScriptHandler::OnEndParsing(bool healthy)
{
  if (healthy && this->healthy())
  {
    // print initial bash code
    beginScript();

    // check directories, files and images
    printCheckCommands();

    // print buffered file commands
    cout << m_fcbuffer.str();

    // print final bash code
    endScript();
  }

  // empty buffers
  m_keys.clear();
  m_values.clear();
  m_ichecks.clear();
  m_dchecks.clear();
  m_fchecks.clear();

  // reset stringstream
  m_fcbuffer.str("");
  m_fcbuffer.clear();
}

// ------
// OnData
// ------
/*
 *
 */
void ScriptHandler::OnData(const string& key, const string& value)
{
  // don't run in bad state
  if ( !healthy() ) return;

  // buffer tag
  m_keys.push_back(key);
  m_values.push_back(value);

  // trigger found
  if (key == "COMPILATIONINDEX")
  {
    // buffer bash code
    bufferFileCommands();

    // empty buffers
    m_keys.clear();
    m_values.clear();
  }
}


// -----------------------------------------------------------------------------
// Internal methods                                             Internal methods
// -----------------------------------------------------------------------------

// -------
// dirname
// -------
/*
 *
 */
string ScriptHandler::dirname(const string& path) const
{
  // start with empty directory
  string dir("");

  // this buffer holds one directory name
  string buffer;

  // extract directory part
  for(string::size_type i = 0; i < path.size(); i++)
  {
    // get current character
    unsigned char uc = static_cast<unsigned char>(path[i]);

    // append character
    buffer += uc;

    // separator found
    if (uc == '/')
    {
      // append directory
      dir.append(buffer);

      // reset buffer
      buffer = "";
    }
  }

  // return directory part
  return dir;
}

// -----
// quote
// -----
/*
 *
 */
string ScriptHandler::quote(const string& line, bool outer) const
{
  // start with empty string
  string quoted("");

  // add outer quotation mark
  if (outer) quoted.append("\"");

  // copy characters
  for(string::size_type i = 0; i < line.size(); i++)
  {
    // get current character
    const char& c = line[i];

    // these characters have to be escaped to get them literally
    if ( (c == '"')
    ||   (c == '$')
    ||   (c == '`')
    ||   (c == '\\') )
    {
      // insert escape character
      quoted += '\\';
      quoted += c;
    }

    // history expansion
    else if (c == '!')
    {
      // switch to strong quoting
      quoted += "\"'!'\"";
    }

    // characters without special meaning
    else
    {
      // append character
      quoted += c;
    }
  }

  // add outer quotation mark
  if (outer) quoted.append("\"");

  // return quoted version
  return quoted;
}

// --------
// getValue
// --------
/*
 *
 */
string ScriptHandler::getValue(const string& key) const
{
  // find related value
  for(unsigned i = 0; i < m_keys.size(); i++)
  {
    if (m_keys[i] == key)
    {
      return m_values[i];
    }
  }

  // key not found
  return "";
}

// ------------------
// bufferFileCommands
// ------------------
/*
 *
 */
void ScriptHandler::bufferFileCommands()
{
  // set comments in this order
  vector<string> order;
  order.push_back( keyinfo::name(keyinfo::COMPILATIONID)    );
  order.push_back( keyinfo::name(keyinfo::COMPILATIONINDEX) );
  order.push_back( keyinfo::name(keyinfo::AUTHOR)           );
  order.push_back( keyinfo::name(keyinfo::COMPOSER)         );
  order.push_back( keyinfo::name(keyinfo::LYRICIST)         );
  order.push_back( keyinfo::name(keyinfo::OPUS)             );
  order.push_back( keyinfo::name(keyinfo::VERSION)          );
  order.push_back( keyinfo::name(keyinfo::ARRANGER)         );
  order.push_back( keyinfo::name(keyinfo::PERFORMER)        );
  order.push_back( keyinfo::name(keyinfo::CONDUCTOR)        );
  order.push_back( keyinfo::name(keyinfo::ENSEMBLE)         );
  order.push_back( keyinfo::name(keyinfo::ALBUMARTIST)      );
  order.push_back( keyinfo::name(keyinfo::ALBUM)            );
  order.push_back( keyinfo::name(keyinfo::GENRE)            );
  order.push_back( keyinfo::name(keyinfo::DATE)             );
  order.push_back( keyinfo::name(keyinfo::TRACKTOTAL)       );
  order.push_back( keyinfo::name(keyinfo::TRACKNUMBER)      );
  order.push_back( keyinfo::name(keyinfo::ARTIST)           );
  order.push_back( keyinfo::name(keyinfo::TITLE)            );
  order.push_back( keyinfo::name(keyinfo::COMMENT)          );
  
  // get special values
  string cmpindex = getValue( keyinfo::name(keyinfo::COMPILATIONINDEX) );
  string filename = getValue( keyinfo::name(keyinfo::FILENAME)         );
  string image    = getValue( keyinfo::name(keyinfo::IMAGE)            );

  // filenames (wav and flac)
  string infile;
  string outfile;

  // compile filenames
  stringstream strstr;
  strstr << "track";
  strstr << setw(3) << setfill('0') << cmpindex;
  strstr << ".cdda.wav";
  strstr << " ";
  strstr << "track";
  strstr << setw(3) << setfill('0') << cmpindex;
  strstr << ".flac";

  // get filenames
  strstr >> infile >> outfile;

  // add empty line
  m_fcbuffer << "# add empty line" << endl;
  m_fcbuffer << "echo" << endl;
  m_fcbuffer << endl;

  // start if block
  m_fcbuffer << "# create flac file" << endl;
  m_fcbuffer << "if [ -s " << quote(infile) << " ] ; then" << endl;
  m_fcbuffer << endl;

  m_fcbuffer << "  # show progress" << endl;
  m_fcbuffer << "  infomsg \"converting " << quote(infile, false) << "\"" << endl;
  m_fcbuffer << endl;

  // create flac command
  m_fcbuffer << "  # convert wav to flac" << endl;
  m_fcbuffer << "  flac --force \\" << endl;
  m_fcbuffer << "       --verify \\" << endl;
  m_fcbuffer << "       --compression-level-8 \\" << endl;
  if ( !image.empty() )
  {
    // add check
    m_ichecks.insert(image);

    m_fcbuffer << "       --picture=\"3||||" << quote(image, false) << "\" \\" << endl;
  }
  m_fcbuffer << "       --output-name=" << quote(outfile) << " \\" << endl;
  m_fcbuffer << "       " << quote(infile) << endl;
  m_fcbuffer << endl;

  // call metaflac once
  bool first = true;

  // create metaflac command
  for(unsigned n = 0; n < order.size(); n++)
  {
    // get related value
    string val = getValue( order[n] );

    // don't set empty comments
    if ( !val.empty() )
    {
      if (first)
      {
        // print comment
        m_fcbuffer << "  # set comments" << endl;

        // call metaflac
        m_fcbuffer << "  metaflac ";

        // update flac
        first = false;
      }

      else
      {
        // print indent
        m_fcbuffer << "           ";
      }

      // print next option
      m_fcbuffer << "--set-tag=\"" << order[n] << "=" << quote(val, false) << "\" \\" << endl;
    }
  }

  // add filename
  if (!first)
  {
    m_fcbuffer <<  "           " << quote(outfile) << endl;
  }

  m_fcbuffer << endl;

  // create mv command
  if ( !filename.empty() )
  {
    // add checks
    m_dchecks.insert( dirname(filename) );
    m_fchecks.insert( filename );

    m_fcbuffer << "  # rename file" << endl;
    m_fcbuffer << "  mv -f " << quote(outfile) << " " << quote(filename) << endl;
    m_fcbuffer << endl;
  }

  // start else block
  m_fcbuffer << "# missing or empty wav file" << endl;
  m_fcbuffer << "else" << endl;
  m_fcbuffer << endl;
  m_fcbuffer << "  # notify user" << endl;
  m_fcbuffer << "  warnmsg \"skipping missing (or empty) wav file: " << quote(infile, false) << "\"" << endl;
  m_fcbuffer << endl;

  if ( !filename.empty() )
  {
    m_fcbuffer << "  # remove (touched resp. truncated) flac file" << endl;
    m_fcbuffer << "  rm -f " << quote(filename) << endl;
    m_fcbuffer << endl;
  }

  m_fcbuffer << "fi" << endl;
  m_fcbuffer << endl;
}

// -----------
// beginScript
// -----------
/*
 *
 */
void ScriptHandler::beginScript() const
{
  cout << "#!/bin/bash" << endl;
  cout << endl;
  cout << "# ------------------------------------------------------------------------------" << endl;
  cout << "# settings                                                              settings" << endl;
  cout << "# ------------------------------------------------------------------------------" << endl;
  cout << endl;
  cout << "# terminal colors" << endl;
  cout << "   NONE=$(tput sgr0)" << endl;
  cout << "    RED=$(tput setaf 1)" << endl;
  cout << "  GREEN=$(tput setaf 2)" << endl;
  cout << " YELLOW=$(tput setaf 3)" << endl;
  cout << "   BLUE=$(tput setaf 4)" << endl;
  cout << "MAGENTA=$(tput setaf 5)" << endl;
  cout << "   CYAN=$(tput setaf 6)" << endl;
  cout << "  WHITE=$(tput setaf 7)" << endl;
  cout << endl;
  cout << "# ------------------------------------------------------------------------------" << endl;
  cout << "# functions                                                            functions" << endl;
  cout << "# ------------------------------------------------------------------------------" << endl;
  cout << endl;

  cout << "# -------" << endl;
  cout << "# failmsg" << endl;
  cout << "# -------" << endl;
  cout << "#" << endl;
  cout << "# This function prints a fail message via stderr." << endl;
  cout << "#" << endl;
  cout << "function failmsg()" << endl;
  cout << "{" << endl;
  cout << "  # push to stderr" << endl;
  cout << "  echo -e \"${RED}[FAIL]${NONE} $1\" 1>&2" << endl;
  cout << "}" << endl;
  cout << endl;
  cout << "# -------" << endl;
  cout << "# warnmsg" << endl;
  cout << "# -------" << endl;
  cout << "#" << endl;
  cout << "# This function prints a warn message via stderr." << endl;
  cout << "#" << endl;
  cout << "function warnmsg()" << endl;
  cout << "{" << endl;
  cout << "  # push to stderr" << endl;
  cout << "  echo -e \"${YELLOW}[WARN]${NONE} $1\" 1>&2" << endl;
  cout << "}" << endl;
  cout << endl;
  cout << "# -------" << endl;
  cout << "# infomsg" << endl;
  cout << "# -------" << endl;
  cout << "#" << endl;
  cout << "# This function prints an info message via stderr." << endl;
  cout << "#" << endl;
  cout << "function infomsg()" << endl;
  cout << "{" << endl;
  cout << "  # push to stderr" << endl;
  cout << "  echo -e \"${BLUE}[INFO]${NONE} $1\" 1>&2" << endl;
  cout << "}" << endl;
  cout << endl;

  cout << "# -------" << endl;
  cout << "# chktool" << endl;
  cout << "# -------" << endl;
  cout << "#" << endl;
  cout << "# This function checks if an external tool is installed." << endl;
  cout << "#" << endl;
  cout << "# $1  name of the tool to check" << endl;
  cout << "#" << endl;
  cout << "function chktool()" << endl;
  cout << "{" << endl;
  cout << "  # query type" << endl;
  cout << "  TOOLTYPE=$(type -t \"$1\")" << endl;
  cout << endl;
  cout << "  # check type" << endl;
  cout << "  if [ \"$TOOLTYPE\" != 'file' ] ; then" << endl;
  cout << endl;
  cout << "    # notify user" << endl;
  cout << "    failmsg \"external tool is missing: \\\"$1\\\"\"" << endl;
  cout << endl;
  cout << "    # signalize trouble" << endl;
  cout << "    exit 1" << endl;
  cout << endl;
  cout << "  fi" << endl;
  cout << "}" << endl;
  cout << endl;
  cout << "# --------" << endl;
  cout << "# chkimage" << endl;
  cout << "# --------" << endl;
  cout << "#" << endl;
  cout << "# This function checks type, size and dimension of the cover image." << endl;
  cout << "#" << endl;
  cout << "# $1  filename" << endl;
  cout << "#" << endl;
  cout << "function chkimage()" << endl;
  cout << "{" << endl;
  cout << "  # check if the image exists" << endl;
  cout << "  if [ ! -f \"$1\" ] ; then" << endl;
  cout << endl;
  cout << "    # notify user" << endl;
  cout << "    failmsg \"unable to locate file: \\\"$1\\\"\"" << endl;
  cout << endl;
  cout << "    # signalize trouble" << endl;
  cout << "    exit 1" << endl;
  cout << endl;
  cout << "  fi" << endl;
  cout << endl;
  cout << "  # get file size in byte" << endl;
  cout << "  FILESIZE=$(stat --printf '%s' \"$1\")" << endl;
  cout << endl;
  cout << "  # get image format" << endl;
  cout << "  FORMAT=$(identify -format '%m' \"$1\")" << endl;
  cout << endl;
  cout << "  # jpeg found" << endl;
  cout << "  if [ \"$FORMAT\" == 'JPEG' ] ; then" << endl;
  cout << endl;
  cout << "    # check file size" << endl;
  cout << "    if (( FILESIZE > 85000 )) ; then" << endl;
  cout << endl;
  cout << "      # notify user" << endl;
  cout << "      failmsg \"the image's file size should not exeed 80K (found: $FILESIZE)\"" << endl;
  cout << endl;
  cout << "      # signalize trouble" << endl;
  cout << "      exit 1" << endl;
  cout << endl;
  cout << "    fi" << endl;
  cout << endl;
  cout << "  # png found" << endl;
  cout << "  elif [ \"$FORMAT\" == 'PNG' ] ; then" << endl;
  cout << endl;
  cout << "    # check file size" << endl;
  cout << "    if (( FILESIZE > 505000 )) ; then" << endl;
  cout << endl;
  cout << "      # notify user" << endl;
  cout << "      failmsg \"the image's file size should not exeed 500K (found: $FILESIZE)\"" << endl;
  cout << endl;
  cout << "      # signalize trouble" << endl;
  cout << "      exit 1" << endl;
  cout << endl;
  cout << "    fi" << endl;
  cout << endl;
  cout << "  # other image types" << endl;
  cout << "  else" << endl;
  cout << endl;
  cout << "    # notify user" << endl;
  cout << "    failmsg \"the image must have either JPEG or PNG format (found: $FORMAT)\"" << endl;
  cout << endl;
  cout << "    # signalize trouble" << endl;
  cout << "    exit 1" << endl;
  cout << endl;
  cout << "  fi" << endl;
  cout << endl;
  cout << "  # get image dimensions" << endl;
  cout << "  DIMENSIONS=$(identify -format '%wx%h' \"$1\")" << endl;
  cout << endl;
  cout << "  # check dimensions" << endl;
  cout << "  if [ \"$DIMENSIONS\" != '300x300' ] ; then" << endl;
  cout << endl;
  cout << "    # notify user" << endl;
  cout << "    failmsg \"the image must must be 300px wide and 300px high (found: $DIMENSIONS)\"" << endl;
  cout << endl;
  cout << "    # signalize trouble" << endl;
  cout << "    exit 1" << endl;
  cout << endl;
  cout << "  fi" << endl;
  cout << "}" << endl;
  cout << endl;
  cout << "# -------" << endl;
  cout << "# chktdir" << endl;
  cout << "# -------" << endl;
  cout << "#" << endl;
  cout << "# This function checks if the target directory can be created." << endl;
  cout << "#" << endl;
  cout << "# $1  name of the directory" << endl;
  cout << "#" << endl;
  cout << "function chktdir()" << endl;
  cout << "{" << endl;
  cout << "  # try to create directory" << endl;
  cout << "  mkdir --parents \"$1\" &>'/dev/null'" << endl;
  cout << endl;
  cout << "  # check if directory exists" << endl;
  cout << "  if [ ! -d \"$1\" ] ; then" << endl;
  cout << endl;
  cout << "    # notify user" << endl;
  cout << "    failmsg \"unable to create target directory: \\\"$1\\\"\"" << endl;
  cout << endl;
  cout << "    # signalize trouble" << endl;
  cout << "    exit 1" << endl;
  cout << endl;
  cout << "  fi" << endl;
  cout << "}" << endl;
  cout << endl;
  cout << "# --------" << endl;
  cout << "# chktfile" << endl;
  cout << "# --------" << endl;
  cout << "#" << endl;
  cout << "# This function checks if the target file can be created." << endl;
  cout << "#" << endl;
  cout << "# $1  filename" << endl;
  cout << "#" << endl;
  cout << "function chktfile()" << endl;
  cout << "{" << endl;
  cout << "  # try to truncate (resp. create) file" << endl;
  cout << "  if ! truncate --size='0' \"$1\" &>'/dev/null' ; then" << endl;
  cout << endl;
  cout << "    # notify user" << endl;
  cout << "    failmsg \"unable to create target file: \\\"$1\\\"\"" << endl;
  cout << endl;
  cout << "    # signalize trouble" << endl;
  cout << "    exit 1" << endl;
  cout << endl;
  cout << "  fi" << endl;
  cout << "}" << endl;
  cout << endl;
  cout << "# ------------------------------------------------------------------------------" << endl;
  cout << "# commands                                                              commands" << endl;
  cout << "# ------------------------------------------------------------------------------" << endl;
  cout << endl;
  cout << "# check required tools" << endl;
  cout << "chktool 'flac'" << endl;
  cout << "chktool 'metaflac'" << endl;
  cout << "chktool 'identify'" << endl;
  cout << endl;
}

// ------------------
// printCheckCommands
// ------------------
/*
 *
 */
void ScriptHandler::printCheckCommands() const
{
  // abbreviation
  typedef set<string>::const_iterator itt;

  // directory images
  if ( !m_ichecks.empty() )
  {
    // print comment
    cout << "# check images" << endl;

    // print all images to check
    for(itt it = m_ichecks.begin(); it != m_ichecks.end(); ++it)
    {
      cout << "chkimage " << quote(*it) << endl;
    }

    // print empty line
    cout << endl;
  }

  // directory checks
  if ( !m_dchecks.empty() )
  {
    // print comment
    cout << "# check directories" << endl;

    // print all directories to check
    for(itt it = m_dchecks.begin(); it != m_dchecks.end(); ++it)
    {
      cout << "chktdir " << quote(*it) << endl;
    }

    // print empty line
    cout << endl;
  }

  // file checks
  if ( !m_fchecks.empty() )
  {
    // print comment
    cout << "# check files" << endl;

    // print all files to check
    for(itt it = m_fchecks.begin(); it != m_fchecks.end(); ++it)
    {
      cout << "chktfile " << quote(*it) << endl;
    }

    // print empty line
    cout << endl;
  }
}

// ---------
// endScript
// ---------
/*
 *
 */
void ScriptHandler::endScript() const
{
  cout << "# signalize success" << endl;
  cout << "exit 0" << endl;
}

