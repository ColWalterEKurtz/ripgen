// -----------------------------------------------------------------------------
// scripts.cpp                                                       scripts.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file defines all members of the @ref scripts namespace.
 * @author     Col. Walter E. Kurtz
 * @version    2019-01-28
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "message.h"
#include "scripts.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Definitions                                                       Definitions
// -----------------------------------------------------------------------------
namespace scripts
{

  // ---------------
  // printTripScript
  // ---------------
  /**
   * Print the bash code that uses cdparanoia to rip audio CDs.
   */
  bool printTripScript(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# settings                                                              settings" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# use dot as decimal separator" << endl;
    cout << "LC_NUMERIC='en_US.UTF-8'" << endl;
    cout << endl;
    cout << "# set device" << endl;
    cout << "DEVICE='/dev/sr0'" << endl;
    cout << endl;
    cout << "# set default speed" << endl;
    cout << "SPEED=20" << endl;
    cout << endl;
    cout << "# number of seconds to wait before an idle process gets killed" << endl;
    cout << "IDLEMAX=90" << endl;
    cout << endl;
    cout << "# the CD's table of contents will be written to this file" << endl;
    cout << "TOCFILE='cd.toc'" << endl;
    cout << endl;
    cout << "# the offset to the CD's track numbers will be written to this file" << endl;
    cout << "DBFILE='offset.db'" << endl;
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
    cout << "# ---------------------" << endl;
    cout << "# show_version_and_exit" << endl;
    cout << "# ---------------------" << endl;
    cout << "#" << endl;
    cout << "# This function shows the script's version and terminates the script." << endl;
    cout << "#" << endl;
    cout << "function show_version_and_exit()" << endl;
    cout << "{" << endl;
    cout << "  echo" << endl;
    cout << "  echo 'version 2016-11-20.1'" << endl;
    cout << "  echo" << endl;
    cout << "  exit 1" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------" << endl;
    cout << "# show_help_and_exit" << endl;
    cout << "# ------------------" << endl;
    cout << "#" << endl;
    cout << "# This function shows the script's help and terminates it." << endl;
    cout << "#" << endl;
    cout << "function show_help_and_exit()" << endl;
    cout << "{" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"NAME\"" << endl;
    cout << "  echo \"    trip - test rip\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"SYNOPSIS\"" << endl;
    cout << "  echo \"    trip [options]\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"DESCRIPTION\"" << endl;
    cout << "  echo \"    trip uses cdparanoia to rip each track of an audio CD\"" << endl;
    cout << "  echo \"    into its own file.\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"OPTIONS\"" << endl;
    cout << "  echo \"    -A        get the track number offset from the database file\"" << endl;
    cout << "  echo \"    -a        get the track number offset from the highest filename\"" << endl;
    cout << "  echo \"    -d <dev>  rip from device <dev> (default: $DEVICE)\"" << endl;
    cout << "  echo \"    -h        show help and exit\"" << endl;
    cout << "  echo \"    -o <num>  add an offset of <num> to each track number\"" << endl;
    cout << "  echo \"    -s <num>  set device speed to <num> (default: $SPEED)\"" << endl;
    cout << "  echo \"    -t <num>  stop ripping the current track if the file size didn't\"" << endl;
    cout << "  echo \"              change within the last <num> seconds (default: $IDLEMAX)\"" << endl;
    cout << "  echo \"    -v        show version and exit\"" << endl;
    cout << "  echo \"    -z        retry to rip empty files\"" << endl;
    cout << "  echo" << endl;
    cout << "  exit 1" << endl;
    cout << "}" << endl;
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
    cout << "# donemsg" << endl;
    cout << "# -------" << endl;
    cout << "#" << endl;
    cout << "# This function prints a done message via stderr." << endl;
    cout << "#" << endl;
    cout << "function donemsg()" << endl;
    cout << "{" << endl;
    cout << "  # push to stderr" << endl;
    cout << "  echo -e \"${GREEN}[DONE]${NONE} $1\" 1>&2" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# --------------------" << endl;
    cout << "# get_number_of_tracks" << endl;
    cout << "# --------------------" << endl;
    cout << "#" << endl;
    cout << "# $1  toc file" << endl;
    cout << "#" << endl;
    cout << "function get_number_of_tracks()" << endl;
    cout << "{" << endl;
    cout << "  sed --quiet           \\" << endl;
    cout << "      --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << "        /^=====/,/^TOTAL/{" << endl;
    cout << endl;
    cout << "          s/^[[:space:]]*[[:digit:]]+\\.[[:space:]]+.*/./p" << endl;
    cout << endl;
    cout << "        }" << endl;
    cout << "      \" \"$1\" \\" << endl;
    cout << "  | wc --lines" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------------" << endl;
    cout << "# get_offset_from_filename" << endl;
    cout << "# ------------------------" << endl;
    cout << "#" << endl;
    cout << "#" << endl;
    cout << "#" << endl;
    cout << "function get_offset_from_filename()" << endl;
    cout << "{" << endl;
    cout << "  # find wav files in the current directory" << endl;
    cout << "  find -maxdepth '1'                             \\" << endl;
    cout << "       -type 'f'                                 \\" << endl;
    cout << "       -regextype 'posix-extended'               \\" << endl;
    cout << "       -regex '.+/track[[:digit:]]+\\.cdda\\.wav$' \\" << endl;
    cout << "       -printf '%P\\n'                            \\" << endl;
    cout << "  | sort --reverse                               \\" << endl;
    cout << "  | head --lines='1'                             \\" << endl;
    cout << "  | sed --quiet                                  \\" << endl;
    cout << "        --regexp-extended                        \\" << endl;
    cout << "        --expression=\"" << endl;
    cout << endl;
    cout << "          # crop digits" << endl;
    cout << "          s/^track([[:digit:]]+)\\.cdda\\.wav$/\\1/" << endl;
    cout << endl;
    cout << "          #remove leading zeros" << endl;
    cout << "          s/0*([[:digit:]]+)/\\1/" << endl;
    cout << endl;
    cout << "          # branch to end of script if all s commands failed" << endl;
    cout << "          T" << endl;
    cout << endl;
    cout << "          # print number" << endl;
    cout << "          p" << endl;
    cout << "        \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ---------------------" << endl;
    cout << "# get_offset_from_dbase" << endl;
    cout << "# ---------------------" << endl;
    cout << "#" << endl;
    cout << "# $1  filename of the database" << endl;
    cout << "# $2  hash value of the toc file" << endl;
    cout << "#" << endl;
    cout << "function get_offset_from_dbase()" << endl;
    cout << "{" << endl;
    cout << "  # get last matching offset" << endl;
    cout << "  tac \"$1\" 2>'/dev/null' | sed -nre \"s/^$2 ([[:digit:]]+)/\\1/p ; T ; q\"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ---------------" << endl;
    cout << "# update_log_file" << endl;
    cout << "# ---------------" << endl;
    cout << "#" << endl;
    cout << "# $1  log file" << endl;
    cout << "# $2  track number" << endl;
    cout << "#" << endl;
    cout << "function update_log_file()" << endl;
    cout << "{" << endl;
    cout << "  # check if log file exists" << endl;
    cout << "  if [ ! -f \"$1\" ]; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    warnmsg \"unable to open log file: \\\"$1\\\"\"" << endl;
    cout << endl;
    cout << "    # exit function" << endl;
    cout << "    return" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # create track number tag" << endl;
    cout << "  TRACK=$(printf 'TRACK %03d' \"$2\")" << endl;
    cout << endl;
    cout << "  # get current date" << endl;
    cout << "  RIPPED=$(date +'%Y-%m-%d %H:%M:%S')" << endl;
    cout << "  " << endl;
    cout << "  # update log file" << endl;
    cout << "  sed -i -re \"s/^ *(\\(== PROGRESS ==.+==\\)) *$/$TRACK @ $RIPPED \\1/\" \"$1\"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ---------" << endl;
    cout << "# rip_track" << endl;
    cout << "# ---------" << endl;
    cout << "#" << endl;
    cout << "# $1  track number" << endl;
    cout << "# $2  output filename" << endl;
    cout << "#" << endl;
    cout << "function rip_track()" << endl;
    cout << "{" << endl;
    cout << "  # show progress" << endl;
    cout << "  infomsg \"ripping track $1 to file \\\"$2\\\"\\n\"" << endl;
    cout << endl;
    cout << "  # set the name of the log file" << endl;
    cout << "  LOGFILE=\"$2.log\"" << endl;
    cout << endl;
    cout << "  # kill cdparanoia if this script gets killed" << endl;
    cout << "  trap '{ kill $PID ; truncate -s0 \"$2\" \"$LOGFILE\" ; } &>/dev/null' EXIT" << endl;
    cout << endl;
    cout << "  # rip given track in background" << endl;
    cout << "  cdparanoia --log-summary=\"$LOGFILE\"       \\" << endl;
    cout << "             --force-cdrom-device \"$DEVICE\" \\" << endl;
    cout << "             --force-read-speed \"$SPEED\"    \\" << endl;
    cout << "             --never-skip='100'             \\" << endl;
    cout << "             --abort-on-skip                \\" << endl;
    cout << "             \"$1\"                           \\" << endl;
    cout << "             \"$2\" &" << endl;
    cout << endl;
    cout << "  # get process ID" << endl;
    cout << "  PID=$!" << endl;
    cout << endl;
    cout << "  # output file's size" << endl;
    cout << "  FILESIZE=0" << endl;
    cout << "  LASTSIZE=0" << endl;
    cout << "  IDLETIME=0" << endl;
    cout << endl;
    cout << "  # while cdparanoia is running" << endl;
    cout << "  while kill -0 \"$PID\" &>'/dev/null'" << endl;
    cout << "  do" << endl;
    cout << endl;
    cout << "    # delay" << endl;
    cout << "    sleep 1" << endl;
    cout << endl;
    cout << "    # check if output file is present" << endl;
    cout << "    if [ -f \"$2\" ]; then" << endl;
    cout << endl;
    cout << "      # get current file size" << endl;
    cout << "      FILESIZE=$(stat --printf='%s' \"$2\")" << endl;
    cout << endl;
    cout << "    fi" << endl;
    cout << endl;
    cout << "    # check if some data has been ripped" << endl;
    cout << "    if (( FILESIZE == LASTSIZE )) ; then" << endl;
    cout << endl;
    cout << "      # increase idle time" << endl;
    cout << "      (( IDLETIME += 1 ))" << endl;
    cout << endl;
    cout << "      # idle for too long" << endl;
    cout << "      if (( IDLETIME > IDLEMAX )) ; then" << endl;
    cout << endl;
    cout << "        # terminate cdparanoia" << endl;
    cout << "        kill -SIGTERM \"$PID\" &>'/dev/null'" << endl;
    cout << endl;
    cout << "        # wait for child processes to terminate" << endl;
    cout << "        wait &>'/dev/null'" << endl;
    cout << endl;
    cout << "        # reset wav file" << endl;
    cout << "        truncate -s0 \"$2\"" << endl;
    cout << endl;
    cout << "        # update log file" << endl;
    cout << "        echo '(== PROGRESS == [~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| 000000 00 ] == ERROR ==)' >> \"$LOGFILE\"" << endl;
    cout << endl;
    cout << "        # add some space" << endl;
    cout << "        echo -e \"\\n\"" << endl;
    cout << endl;
    cout << "        # notify user" << endl;
    cout << "        failmsg \"unable to rip track $1\\n\"" << endl;
    cout << endl;
    cout << "        # exit loop" << endl;
    cout << "        break" << endl;
    cout << endl;
    cout << "      fi" << endl;
    cout << endl;
    cout << "    else" << endl;
    cout << endl;
    cout << "      # update last size" << endl;
    cout << "      LASTSIZE=\"$FILESIZE\"" << endl;
    cout << endl;
    cout << "      # reset idle time" << endl;
    cout << "      IDLETIME=0" << endl;
    cout << endl;
    cout << "    fi" << endl;
    cout << endl;
    cout << "  done" << endl;
    cout << endl;
    cout << "  # disable trap" << endl;
    cout << "  trap - EXIT" << endl;
    cout << endl;
    cout << "  # append paranoia tag to log file" << endl;
    cout << "  update_log_file \"$LOGFILE\" \"$1\"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# options                                                                options" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# set default values" << endl;
    cout << "NOSIZE='skip'" << endl;
    cout << "OFFGET='none'" << endl;
    cout << "OFFSET=0" << endl;
    cout << endl;
    cout << "# check passed options" << endl;
    cout << "while getopts ':Aad:ho:s:t:vz' OPTION \"$@\"" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  case \"$OPTION\" in" << endl;
    cout << endl;
    cout << "    'A') OFFGET='dbase'" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'a') OFFGET='files'" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'd') DEVICE=\"$OPTARG\"" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'h') show_help_and_exit" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'o') OFFGET='param'" << endl;
    cout << "         OFFSET=\"$OPTARG\"" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    's') SPEED=\"$OPTARG\"" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    't') IDLEMAX=\"$OPTARG\"" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'v') show_version_and_exit" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'z') NOSIZE='retry'" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    '?') failmsg \"unknown option: -$OPTARG\"" << endl;
    cout << "         exit 1" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    ':') failmsg \"missing argument: -$OPTARG <argument>\"" << endl;
    cout << "         exit 1" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "  esac" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# get number of positional parameters" << endl;
    cout << "PCOUNT=$(( $# - OPTIND + 1 ))" << endl;
    cout << endl;
    cout << "# check number of positional parameters" << endl;
    cout << "if (( PCOUNT != 0 )); then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg 'no positional parameters allowed'" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# get offset from wav files" << endl;
    cout << "if [ \"$OFFGET\" == 'files' ] ; then" << endl;
    cout << endl;
    cout << "  # get current offset" << endl;
    cout << "  OFFSET=$(get_offset_from_filename)" << endl;
    cout << endl;
    cout << "  # check offset" << endl;
    cout << "  if [ -z \"$OFFSET\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    failmsg 'unable to detect offset automatically'" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    exit 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# reset toc file" << endl;
    cout << "truncate -s0 \"$TOCFILE\" &>'/dev/null'" << endl;
    cout << endl;
    cout << "# show toc and create toc file" << endl;
    cout << "cdparanoia --force-cdrom-device \"$DEVICE\" \\" << endl;
    cout << "           --query                        \\" << endl;
    cout << "           2>&1                           \\" << endl;
    cout << "| tee \"$TOCFILE\"" << endl;
    cout << endl;
    cout << "# check toc file" << endl;
    cout << "if [ ! -s \"$TOCFILE\" ] ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg 'unable to create toc file'" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# create hash value of the created toc file" << endl;
    cout << "CHKSUM=$(sha1sum \"$TOCFILE\" | sed -re 's/ .+//')" << endl;
    cout << endl;
    cout << "# get offset from db file" << endl;
    cout << "if [ \"$OFFGET\" == 'dbase' ] ; then" << endl;
    cout << endl;
    cout << "  # get current offset" << endl;
    cout << "  OFFSET=$(get_offset_from_dbase \"$DBFILE\" \"$CHKSUM\")" << endl;
    cout << endl;
    cout << "  # check offset" << endl;
    cout << "  if [ -z \"$OFFSET\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    failmsg 'unable to get offset from database'" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    exit 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# check if a specific offset was given" << endl;
    cout << "if [ \"$OFFGET\" != 'none' ] ; then" << endl;
    cout << endl;
    cout << "  # check if db file is already pressent" << endl;
    cout << "  if [ -s \"$DBFILE\" ] ; then" << endl;
    cout << endl;
    cout << "    # remove db entries for this hash value" << endl;
    cout << "    sed --in-place                 \\" << endl;
    cout << "        --regexp-extended          \\" << endl;
    cout << "        --expression=\"/^$CHKSUM/d\" \\" << endl;
    cout << "        \"$DBFILE\"" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # save current offset" << endl;
    cout << "  echo \"$CHKSUM $OFFSET\" >> \"$DBFILE\"" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# get number of audio tracks" << endl;
    cout << "TRACKS=$(get_number_of_tracks \"$TOCFILE\")" << endl;
    cout << endl;
    cout << "# rip all tracks" << endl;
    cout << "for (( TNO = 1 ; TNO <= TRACKS ; TNO++ ))" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  # create output filename" << endl;
    cout << "  FILENAME=$(printf 'track%03d.cdda.wav' \"$(( OFFSET + TNO ))\")" << endl;
    cout << endl;
    cout << "  # always skip non-zero files" << endl;
    cout << "  if [ -s \"$FILENAME\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    warnmsg \"skipping track $TNO (file already exists: \\\"$FILENAME\\\")\\n\"" << endl;
    cout << endl;
    cout << "    # next cycle  " << endl;
    cout << "    continue" << endl;
    cout << endl;
    cout << "  # check what to do with empty files" << endl;
    cout << "  elif [ -f \"$FILENAME\" ] && [ \"$NOSIZE\" != 'retry' ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    warnmsg \"skipping track $TNO (file already exists: \\\"$FILENAME\\\")\\n\"" << endl;
    cout << endl;
    cout << "    # next cycle  " << endl;
    cout << "    continue" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # rip given track" << endl;
    cout << "  rip_track \"$TNO\" \"$FILENAME\"" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# show log summary" << endl;
    cout << "find -maxdepth '1'               \\" << endl;
    cout << "     -type 'f'                   \\" << endl;
    cout << "     -regextype 'posix-extended' \\" << endl;
    cout << "     -regex '.+\\.[Ll][Oo][Gg]$'  \\" << endl;
    cout << "     -print0                     \\" << endl;
    cout << "| sort -z                        \\" << endl;
    cout << "| xargs -0 cat                   \\" << endl;
    cout << "| grep 'PROGRESS'                \\" << endl;
    cout << "| nl" << endl;
    cout << endl;
    cout << "# insert an empty line at the end" << endl;
    cout << "echo" << endl;
    cout << endl;
    cout << "# open tray" << endl;
    cout << "eject \"$DEVICE\" &" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ---------------------
  // printBack2TrackScript
  // ---------------------
  /**
   * Print the bash code that decodes flac files back to wav.
   */
  bool printBack2TrackScript(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# settings                                                              settings" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# use dot as decimal separator" << endl;
    cout << "LC_NUMERIC='en_US.UTF-8'" << endl;
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
    cout << "# ---------------------" << endl;
    cout << "# show_version_and_exit" << endl;
    cout << "# ---------------------" << endl;
    cout << "#" << endl;
    cout << "# This function shows the script's version and terminates the script." << endl;
    cout << "#" << endl;
    cout << "function show_version_and_exit()" << endl;
    cout << "{" << endl;
    cout << "  echo" << endl;
    cout << "  echo 'version 2016-11-20.1'" << endl;
    cout << "  echo" << endl;
    cout << "  exit 1" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------" << endl;
    cout << "# show_help_and_exit" << endl;
    cout << "# ------------------" << endl;
    cout << "#" << endl;
    cout << "# This function shows the script's help and terminates it." << endl;
    cout << "#" << endl;
    cout << "function show_help_and_exit()" << endl;
    cout << "{" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"NAME\"" << endl;
    cout << "  echo \"    back2track - convert flac files back to wav\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"SYNOPSIS\"" << endl;
    cout << "  echo \"    back2track [options] [{directory|text file|flac file(s)}]\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"DESCRIPTION\"" << endl;
    cout << "  echo \"    back2track will decode and rename flac files from a given directory\"" << endl;
    cout << "  echo \"    in order that the wav files can be re-encoded by a ripgen script.\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"OPTIONS\"" << endl;
    cout << "  echo \"    -h        show help and exit\"" << endl;
    cout << "  echo \"    -n <num>  always use serial number starting with <num>\"" << endl;
    cout << "  echo \"    -t        use TRACKNUMBER if COMPILATIONINDEX is missing\"" << endl;
    cout << "  echo \"    -T        always use TRACKNUMBER\"" << endl;
    cout << "  echo \"    -v        show version and exit\"" << endl;
    cout << "  echo" << endl;
    cout << "  exit 1" << endl;
    cout << "}" << endl;
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
    cout << "# donemsg" << endl;
    cout << "# -------" << endl;
    cout << "#" << endl;
    cout << "# This function prints a done message via stderr." << endl;
    cout << "#" << endl;
    cout << "function donemsg()" << endl;
    cout << "{" << endl;
    cout << "  # push to stderr" << endl;
    cout << "  echo -e \"${GREEN}[DONE]${NONE} $1\" 1>&2" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------" << endl;
    cout << "# escape_lines" << endl;
    cout << "# ------------" << endl;
    cout << "#" << endl;
    cout << "# no arguments needed: use as filter" << endl;
    cout << "#" << endl;
    cout << "function escape_lines()" << endl;
    cout << "{" << endl;
    cout << "  # prepare (NL separated) lines so they can pass 'read' unchanged" << endl;
    cout << "  sed --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << "        # skip empty lines" << endl;
    cout << "        /^[[:space:]]*$/ { d }" << endl;
    cout << endl;
    cout << "        # skip comments" << endl;
    cout << "        /^[[:space:]]*#/ { d }" << endl;
    cout << endl;
    cout << "        # escape each internal escape character (colon) first" << endl;
    cout << "        s/:/:c/g" << endl;
    cout << endl;
    cout << "        # escape characters treated special by 'read'" << endl;
    cout << "        s/\\x09/:t/g" << endl;
    cout << "        s/\\x20/:s/g" << endl;
    cout << "        s/\\\\\\\\/:b/g" << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# -------------" << endl;
    cout << "# unescape_line" << endl;
    cout << "# -------------" << endl;
    cout << "#" << endl;
    cout << "# no arguments needed: use as filter" << endl;
    cout << "#" << endl;
    cout << "function unescape_line()" << endl;
    cout << "{" << endl;
    cout << "  sed --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << "        # unescape special characters first" << endl;
    cout << "        s/:t/\\x09/g" << endl;
    cout << "        s/:s/\\x20/g" << endl;
    cout << "        s/:b/\\\\\\\\/g" << endl;
    cout << endl;
    cout << "        # unescape each internal escape character at the end" << endl;
    cout << "        s/:c/:/g" << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ----------" << endl;
    cout << "# is_integer" << endl;
    cout << "# ----------" << endl;
    cout << "#" << endl;
    cout << "# $1  number" << endl;
    cout << "#" << endl;
    cout << "function is_integer()" << endl;
    cout << "{" << endl;
    cout << "  # no argument given" << endl;
    cout << "  [ -z \"$1\" ] && return 1" << endl;
    cout << endl;
    cout << "  # try to print as integer" << endl;
    cout << "  printf '%d' \"$1\" &>'/dev/null' || return 1" << endl;
    cout << endl;
    cout << "  # integer found" << endl;
    cout << "  return 0" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ----------" << endl;
    cout << "# get_number" << endl;
    cout << "# ----------" << endl;
    cout << "#" << endl;
    cout << "# This function returns the number for the name" << endl;
    cout << "# of the next wav file to create." << endl;
    cout << "#" << endl;
    cout << "# $1  flac file" << endl;
    cout << "#" << endl;
    cout << "function get_number()" << endl;
    cout << "{" << endl;
    cout << "  # reset return value" << endl;
    cout << "  TRACKNUMBER=''" << endl;
    cout << endl;
    cout << "  if [ \"$NUMSOURCE\" == 'COUNTER' ] ; then" << endl;
    cout << endl;
    cout << "    # set counter value" << endl;
    cout << "    TRACKNUMBER=\"$NUMCOUNTER\"" << endl;
    cout << endl;
    cout << "  elif [ \"$NUMSOURCE\" == 'TRACK_ALWAYS' ] ; then" << endl;
    cout << endl;
    cout << "    # get track number from flac file" << endl;
    cout << "    TRACKNUMBER=$(metaflac --show-tag='TRACKNUMBER' \"$1\" 2>'/dev/null' \\" << endl;
    cout << "                  | sed -nre 's/^[^=]+=0*([[:digit:]]+)$/\\1/p')" << endl;
    cout << endl;
    cout << "  elif [ \"$NUMSOURCE\" == 'TRACK_IF_MISSING' ] ; then" << endl;
    cout << endl;
    cout << "    # get compilation index from flac file first" << endl;
    cout << "    TRACKNUMBER=$(metaflac --show-tag='COMPILATIONINDEX' \"$1\" 2>'/dev/null' \\" << endl;
    cout << "                  | sed -nre 's/^[^=]+=0*([[:digit:]]+)$/\\1/p')" << endl;
    cout << endl;
    cout << "    # check if COMPILATIONINDEX is missing" << endl;
    cout << "    if [ -z \"$TRACKNUMBER\" ] ; then" << endl;
    cout << endl;
    cout << "      # get track number from flac file" << endl;
    cout << "      TRACKNUMBER=$(metaflac --show-tag='TRACKNUMBER' \"$1\" 2>'/dev/null' \\" << endl;
    cout << "                    | sed -nre 's/^[^=]+=0*([[:digit:]]+)$/\\1/p')" << endl;
    cout << endl;
    cout << "    fi" << endl;
    cout << endl;
    cout << "  elif [ \"$NUMSOURCE\" == 'INDEX' ] ; then" << endl;
    cout << endl;
    cout << "    # get compilation index from flac file" << endl;
    cout << "    TRACKNUMBER=$(metaflac --show-tag='COMPILATIONINDEX' \"$1\" 2>'/dev/null' \\" << endl;
    cout << "                  | sed -nre 's/^[^=]+=0*([[:digit:]]+)$/\\1/p')" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # check number" << endl;
    cout << "  if [ -z \"$TRACKNUMBER\" ] ; then" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    return 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # return value" << endl;
    cout << "  printf '%d' \"$TRACKNUMBER\"" << endl;
    cout << endl;
    cout << "  # signalize success" << endl;
    cout << "  return 0" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# --------" << endl;
    cout << "# flac2wav" << endl;
    cout << "# --------" << endl;
    cout << "#" << endl;
    cout << "# This function converts a flac file back to wav and renames" << endl;
    cout << "# it in the same way cdparamoia would do." << endl;
    cout << "#" << endl;
    cout << "# $1  flac file" << endl;
    cout << "#" << endl;
    cout << "function flac2wav()" << endl;
    cout << "{" << endl;
    cout << "  # get base name" << endl;
    cout << "  BASENAME=$(basename \"$1\")" << endl;
    cout << endl;
    cout << "  # get number for next filename" << endl;
    cout << "  TRACKNUMBER=$(get_number \"$1\")" << endl;
    cout << endl;
    cout << "  # check number" << endl;
    cout << "  if [ -z \"$TRACKNUMBER\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    failmsg \"unable to get track number: \\\"$BASENAME\\\"\"" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    return 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # check minimum value" << endl;
    cout << "  if (( TRACKNUMBER < 1 )) ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    failmsg \"invalid track number found: \\\"$TRACKNUMBER\\\" ($BASENAME)\"" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    return 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # set 'original' filename" << endl;
    cout << "  TRACKNAME=$(printf 'track%03d.cdda.wav' \"$TRACKNUMBER\")" << endl;
    cout << endl;
    cout << "  # check file" << endl;
    cout << "  if [ -s \"$TRACKNAME\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    failmsg \"file already exists: $TRACKNAME\"" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    return 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # show progress" << endl;
    cout << "  infomsg \"decoding file: $TRACKNAME  <--  $BASENAME\"" << endl;
    cout << endl;
    cout << "  # try to decode flac to wav" << endl;
    cout << "  if ! flac --totally-silent --decode --output-name=\"$TRACKNAME\" \"$1\" ; then" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    return 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # step counter" << endl;
    cout << "  (( NUMCOUNTER += 1 ))" << endl;
    cout << endl;
    cout << "  # signalize success" << endl;
    cout << "  return 0" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# -----------------" << endl;
    cout << "# operate_directory" << endl;
    cout << "# -----------------" << endl;
    cout << "#" << endl;
    cout << "# $1  directory" << endl;
    cout << "#" << endl;
    cout << "function operate_directory()" << endl;
    cout << "{" << endl;
    cout << "  find \"$1\"                           \\" << endl;
    cout << "       -maxdepth '1'                  \\" << endl;
    cout << "       -type 'f'                      \\" << endl;
    cout << "       -regextype 'posix-extended'    \\" << endl;
    cout << "       -regex '.+\\.[Ff][Ll][Aa][Cc]$' \\" << endl;
    cout << "       -print0                        \\" << endl;
    cout << "  | sort -z                           \\" << endl;
    cout << "  | while read -rd $'\\0' FILENAME" << endl;
    cout << "  do" << endl;
    cout << endl;
    cout << "    # try to decode flac file" << endl;
    cout << "    if ! flac2wav \"$FILENAME\" ; then" << endl;
    cout << endl;
    cout << "      # signalize trouble" << endl;
    cout << "      exit 1" << endl;
    cout << endl;
    cout << "    fi" << endl;
    cout << endl;
    cout << "  done" << endl;
    cout << endl;
    cout << "  # signalize success" << endl;
    cout << "  return 0" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# -----------------" << endl;
    cout << "# operate_text_file" << endl;
    cout << "# -----------------" << endl;
    cout << "#" << endl;
    cout << "# $1  text file" << endl;
    cout << "#" << endl;
    cout << "function operate_text_file()" << endl;
    cout << "{" << endl;
    cout << "  # read lines from text file" << endl;
    cout << "  while read ESCAPED" << endl;
    cout << "  do" << endl;
    cout << endl;
    cout << "    # get unescape version" << endl;
    cout << "    FILENAME=$(unescape_line <<< \"$ESCAPED\")" << endl;
    cout << endl;
    cout << "    # try to decode flac file" << endl;
    cout << "    if ! flac2wav \"$FILENAME\" ; then" << endl;
    cout << endl;
    cout << "      # signalize trouble" << endl;
    cout << "      exit 1" << endl;
    cout << endl;
    cout << "    fi" << endl;
    cout << endl;
    cout << "  done < <(cat \"$1\" | escape_lines)" << endl;
    cout << endl;
    cout << "  # signalize success" << endl;
    cout << "  return 0" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------" << endl;
    cout << "# operate_flac_files" << endl;
    cout << "# ------------------" << endl;
    cout << "#" << endl;
    cout << "# $@  list of flac files" << endl;
    cout << "#" << endl;
    cout << "function operate_flac_files()" << endl;
    cout << "{" << endl;
    cout << "  # handle all passed arguments" << endl;
    cout << "  while [ -n \"$1\" ]" << endl;
    cout << "  do" << endl;
    cout << endl;
    cout << "    # try to decode flac file" << endl;
    cout << "    if ! flac2wav \"$1\" ; then" << endl;
    cout << endl;
    cout << "      # signalize trouble" << endl;
    cout << "      exit 1" << endl;
    cout << endl;
    cout << "    fi" << endl;
    cout << endl;
    cout << "    # next file" << endl;
    cout << "    shift" << endl;
    cout << endl;
    cout << "  done" << endl;
    cout << endl;
    cout << "  # signalize success" << endl;
    cout << "  return 0" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# options                                                                options" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# set default source" << endl;
    cout << "NUMSOURCE='INDEX'" << endl;
    cout << endl;
    cout << "# set initial counter" << endl;
    cout << "NUMCOUNTER='1'" << endl;
    cout << endl;
    cout << "# check passed options" << endl;
    cout << "while getopts ':hn:tTv' OPTION \"$@\"" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  case \"$OPTION\" in" << endl;
    cout << endl;
    cout << "    'h') show_help_and_exit" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'n') NUMSOURCE='COUNTER'" << endl;
    cout << "         NUMCOUNTER=\"$OPTARG\"" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    't') NUMSOURCE='TRACK_IF_MISSING'" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'T') NUMSOURCE='TRACK_ALWAYS'" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'v') show_version_and_exit" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    '?') failmsg \"unknown option: -$OPTARG\"" << endl;
    cout << "         exit 1" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    ':') failmsg \"missing argument: -$OPTARG <argument>\"" << endl;
    cout << "         exit 1" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "  esac" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# drop parsed options" << endl;
    cout << "shift $(( OPTIND - 1 ))" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# check source of numbers" << endl;
    cout << "if [ \"$NUMSOURCE\" == 'COUNTER' ] ; then" << endl;
    cout << endl;
    cout << "  # check counter" << endl;
    cout << "  if ! is_integer \"$NUMCOUNTER\" ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    failmsg \"invalid number given: \\\"$NUMCOUNTER\\\"\"" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    exit 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# get mime type of the first (positional) parameter" << endl;
    cout << "MTYPE=$(file --brief --mime-type \"$1\" 2>'/dev/null')" << endl;
    cout << endl;
    cout << "# directory passed" << endl;
    cout << "if [ \"$MTYPE\" == 'inode/directory' ] ; then" << endl;
    cout << endl;
    cout << "  # get all flac files from directory" << endl;
    cout << "  if ! operate_directory \"$1\" ; then" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    exit 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "# text file passed" << endl;
    cout << "elif [ \"$MTYPE\" == 'text/plain' ] ; then" << endl;
    cout << endl;
    cout << "  # get all flac files from text file" << endl;
    cout << "  if ! operate_text_file \"$1\" ; then" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    exit 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "# flac file(s) passed" << endl;
    cout << "elif [ \"$MTYPE\" == 'audio/x-flac' ] ; then" << endl;
    cout << endl;
    cout << "  # get all flac files from command-line" << endl;
    cout << "  if ! operate_flac_files \"$@\" ; then" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    exit 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "# unknown resource" << endl;
    cout << "else" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"don't know how to handle this: \\\"$1\\\"\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ---------------
  // printLSDBScript
  // ---------------
  /**
   * Print the bash code that lists the different albums from all cd files.
   */
  bool printLSDBScript(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# list all different albums" << endl;
    cout << "find -mindepth '2'                   \\" << endl;
    cout << "     -maxdepth '2'                   \\" << endl;
    cout << "     -type 'f'                       \\" << endl;
    cout << "     -regextype 'posix-extended'     \\" << endl;
    cout << "     -regex '.+\\.[Cc][Dd]$'          \\" << endl;
    cout << "     -print0                         \\" << endl;
    cout << "| ripgen -dz                         \\" << endl;
    cout << "| sed -nre 's/.*/&\\n&\\n&/p'          \\" << endl;
    cout << "| sed -nre '1~3 { s/.*\\|CDFILE=([^\\|]*).*/\\1/      ; h }" << endl;
    cout << "            2~3 { s/.*\\|ALBUMARTIST=([^\\|]*).*/\\1/ ; H }" << endl;
    cout << "            3~3 { s/.*\\|ALBUM=([^\\|]*).*/\\1/       ; H ; g ; s/\\n/|/g ; p }' \\" << endl;
    cout << "| sort               \\" << endl;
    cout << "| uniq               \\" << endl;
    cout << "| sort -t '|' -k '2' \\" << endl;
    cout << "| sed -re 's/\\|/ - /g'" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ---------------------
  // printMkDBImportScript
  // ---------------------
  /**
   * Print the bash code that creates the importable tag file(s).
   */
  bool printMkDBImportScript(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# settings                                                              settings" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "set -o errtrace  # any trap on ERR is inherited by shell functions," << endl;
    cout << "                 #   command substitutions, and commands executed in a subshell" << endl;
    cout << "                 #   environment" << endl;
    cout << "set -o nounset   # treat unset variables and parameters other than the special" << endl;
    cout << "                 #   parameters \"@\" and \"*\" as an error when performing" << endl;
    cout << "                 #   parameter expansion" << endl;
    cout << "set -o pipefail  # the return value of a pipeline is the value of the last" << endl;
    cout << "                 #   (rightmost) command to exit with a non-zero status, or" << endl;
    cout << "                 #   zero if all commands in the pipeline exit successfully" << endl;
    cout << endl;
    cout << "# set language" << endl;
    cout << "export LANG=\"en_US.UTF-8\"" << endl;
    cout << endl;
    cout << "# use dot as decimal separator" << endl;
    cout << "export LC_NUMERIC=\"en_US.UTF-8\"" << endl;
    cout << endl;
    cout << "# terminal colors" << endl;
    cout << "readonly    NONE=$(tput sgr0)" << endl;
    cout << "readonly     RED=$(tput setaf 1)" << endl;
    cout << "readonly   GREEN=$(tput setaf 2)" << endl;
    cout << "readonly  YELLOW=$(tput setaf 3)" << endl;
    cout << "readonly    BLUE=$(tput setaf 4)" << endl;
    cout << "readonly MAGENTA=$(tput setaf 5)" << endl;
    cout << "readonly    CYAN=$(tput setaf 6)" << endl;
    cout << "readonly   WHITE=$(tput setaf 7)" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# functions                                                            functions" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# -------" << endl;
    cout << "# failmsg" << endl;
    cout << "# -------" << endl;
    cout << "#" << endl;
    cout << "# This function prints a red colored message via stderr." << endl;
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
    cout << "# This function prints a yellow colored message via stderr." << endl;
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
    cout << "# This function prints a blue colored message via stderr." << endl;
    cout << "#" << endl;
    cout << "function infomsg()" << endl;
    cout << "{" << endl;
    cout << "  # push to stderr" << endl;
    cout << "  echo -e \"${BLUE}[INFO]${NONE} $1\" 1>&2" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# -------" << endl;
    cout << "# donemsg" << endl;
    cout << "# -------" << endl;
    cout << "#" << endl;
    cout << "# This function prints a green colored message via stderr." << endl;
    cout << "#" << endl;
    cout << "function donemsg()" << endl;
    cout << "{" << endl;
    cout << "  # push to stderr" << endl;
    cout << "  echo -e \"${GREEN}[DONE]${NONE} $1\" 1>&2" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ---------------" << endl;
    cout << "# semicolon_split" << endl;
    cout << "# ---------------" << endl;
    cout << "#" << endl;
    cout << "# ... | semicolon_split | ..." << endl;
    cout << "#" << endl;
    cout << "function semicolon_split()" << endl;
    cout << "{" << endl;
    cout << "  sed --quiet           \\" << endl;
    cout << "      --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << endl;
    cout << "        :again" << endl;
    cout << endl;
    cout << "          # copy entire line to hold space" << endl;
    cout << "          h" << endl;
    cout << endl;
    cout << "          # print first semicolon separated value" << endl;
    cout << "          s/([^=]+)=([^;]+);[[:space:]]*(.+)/\\1=\\2/p" << endl;
    cout << endl;
    cout << "          # restore initial version" << endl;
    cout << "          g" << endl;
    cout << endl;
    cout << "          # remove first semicolon separated value" << endl;
    cout << "          s/([^=]+)=([^;]+);[[:space:]]*(.+)/\\1=\\3/" << endl;
    cout << endl;
    cout << "        t again" << endl;
    cout << endl;
    cout << "        # print last value" << endl;
    cout << "        s/([^=]+)=(.*[^[:space:]])[[:space:]]*/\\1=\\2/p" << endl;
    cout << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ----------" << endl;
    cout << "# utf82ascii" << endl;
    cout << "# ----------" << endl;
    cout << "#" << endl;
    cout << "# This filter converts a given UTF-8 stream to ascii." << endl;
    cout << "#" << endl;
    cout << "# ... | utf82ascii | ..." << endl;
    cout << "#" << endl;
    cout << "function utf82ascii()" << endl;
    cout << "{" << endl;
    cout << "  sed --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << "      # escape all colons" << endl;
    cout << "      s/:/:c/g" << endl;
    cout << endl;
    cout << "      # escape all native question marks (iconv may introduce much more)" << endl;
    cout << "      s/\\\\?/:q/g" << endl;
    cout << endl;
    cout << "      # collection of characters that aren't translated (suitable) by iconv" << endl;
    cout << "      s/¦/|/g" << endl;
    cout << "      s/¡/!/g" << endl;
    cout << "      s/¿/:q/g" << endl;
    cout << "      s/„/\\\"/g" << endl;
    cout << "      s/«/\\\"/g" << endl;
    cout << "      s/»/\\\"/g" << endl;
    cout << "      s/÷/\\//g" << endl;
    cout << "      s/±/+-/g" << endl;
    cout << "      s/¹/^1/g" << endl;
    cout << "      s/²/^2/g" << endl;
    cout << "      s/³/^3/g" << endl;
    cout << "      s/Ä/Ae/g" << endl;
    cout << "      s/ä/ae/g" << endl;
    cout << "      s/Ö/Oe/g" << endl;
    cout << "      s/ö/oe/g" << endl;
    cout << "      s/Ü/Ue/g" << endl;
    cout << "      s/ü/ue/g" << endl;
    cout << "      s/Ø/Oe/g" << endl;
    cout << "      s/ø/oe/g" << endl;
    cout << "      s/Ð/Dh/g" << endl;
    cout << "      s/ð/dh/g" << endl;
    cout << "      s/Þ/Th/g" << endl;
    cout << "      s/þ/th/g" << endl;
    cout << "    \"                                         \\" << endl;
    cout << "  | iconv --from-code \"UTF-8\"                 \\" << endl;
    cout << "          --to-code \"ASCII//TRANSLIT//IGNORE\" \\" << endl;
    cout << "  | sed --regexp-extended                     \\" << endl;
    cout << "        --expression=\"" << endl;
    cout << "        # remove all question marks introduced by iconv" << endl;
    cout << "        s/\\\\?//g" << endl;
    cout << endl;
    cout << "        # restore all escaped question marks" << endl;
    cout << "        s/:q/?/g" << endl;
    cout << endl;
    cout << "        # restore all escaped colons" << endl;
    cout << "        s/:c/:/g" << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------" << endl;
    cout << "# escape_lines" << endl;
    cout << "# ------------" << endl;
    cout << "#" << endl;
    cout << "# This function escapes all characters in a line of text," << endl;
    cout << "# that 'read' would treat in a special way." << endl;
    cout << "#" << endl;
    cout << "# ... | escape_lines | ..." << endl;
    cout << "#" << endl;
    cout << "function escape_lines()" << endl;
    cout << "{" << endl;
    cout << "  # prepare (NL separated) lines so they can pass 'read' unchanged" << endl;
    cout << "  sed --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << endl;
    cout << "        # skip empty lines" << endl;
    cout << "        /^[[:space:]]*$/ { d }" << endl;
    cout << endl;
    cout << "        # escape each internal escape character (colon) first" << endl;
    cout << "        s/:/:c/g" << endl;
    cout << endl;
    cout << "        # escape characters treated special by 'read'" << endl;
    cout << "        s/\\x09/:t/g" << endl;
    cout << "        s/\\x20/:s/g" << endl;
    cout << "        s/\\\\\\\\/:b/g" << endl;
    cout << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# -------------" << endl;
    cout << "# unescape_line" << endl;
    cout << "# -------------" << endl;
    cout << "#" << endl;
    cout << "# This function restores lines that have been modified by 'escape_lines'." << endl;
    cout << "#" << endl;
    cout << "# ... | unescape_line | ..." << endl;
    cout << "#" << endl;
    cout << "function unescape_line()" << endl;
    cout << "{" << endl;
    cout << "  sed --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << endl;
    cout << "        # unescape special characters first" << endl;
    cout << "        s/:t/\\x09/g" << endl;
    cout << "        s/:s/\\x20/g" << endl;
    cout << "        s/:b/\\\\\\\\/g" << endl;
    cout << endl;
    cout << "        # unescape each internal escape character at the end" << endl;
    cout << "        s/:c/:/g" << endl;
    cout << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# options                                                                options" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# set default values" << endl;
    cout << "OPMODE=\"DEFAULT\"" << endl;
    cout << "DBDIR=\"$HOME/Musik/rip/db/\"" << endl;
    cout << endl;
    cout << "# set options to accept" << endl;
    cout << "readonly KNOWN_OPTIONS=\":hvd:\"" << endl;
    cout << endl;
    cout << "# check passed options" << endl;
    cout << "while getopts \"$KNOWN_OPTIONS\" OPTION \"$@\"" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  case \"$OPTION\" in" << endl;
    cout << endl;
    cout << "    # show help" << endl;
    cout << "    \"h\") OPMODE=\"SHOW_HELP\"" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    # show version" << endl;
    cout << "    \"v\") OPMODE=\"SHOW_VERSION\"" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    # set db directory" << endl;
    cout << "    \"d\") DBDIR=\"$OPTARG\"" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    \"?\") failmsg \"unknown option: -$OPTARG\"" << endl;
    cout << "         exit 1" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    \":\") failmsg \"missing argument: -$OPTARG <argument>\"" << endl;
    cout << "         exit 1" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "  esac" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# get number of positional parameters" << endl;
    cout << "PPNUM=$(( $# - OPTIND + 1 ))" << endl;
    cout << endl;
    cout << "# drop all parsed options" << endl;
    cout << "shift $(( OPTIND - 1 ))" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# show version" << endl;
    cout << "if [ \"$OPMODE\" == \"SHOW_VERSION\" ] ; then" << endl;
    cout << endl;
    cout << "  # this is the tool's version" << endl;
    cout << "  echo \"v2018-03-08\"" << endl;
    cout << endl;
    cout << "  # don't proceed" << endl;
    cout << "  exit 0" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# show help" << endl;
    cout << "if [ \"$OPMODE\" == \"SHOW_HELP\" ] ; then" << endl;
    cout << endl;
    cout << "  # show syntax" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"$(basename \"$0\") creates small files that can be imported by vim's :r command\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"$(basename \"$0\") [-hv] [-d <dir>]\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"Options:\"" << endl;
    cout << "  echo \"  -h        show help and exit\"" << endl;
    cout << "  echo \"  -v        show version and exit\"" << endl;
    cout << "  echo \"  -d <dir>  set database directory\"" << endl;
    cout << "  echo" << endl;
    cout << endl;
    cout << "  # don't proceed" << endl;
    cout << "  exit 0" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# check database directory" << endl;
    cout << "if [ ! -d \"$DBDIR\" ] ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"unable to locate directory: \\\"$DBDIR\\\"\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# create temporary file" << endl;
    cout << "TEMPFILE=$(mktemp)" << endl;
    cout << endl;
    cout << "# remove temporary file on exit" << endl;
    cout << "trap 'rm -f \"$TEMPFILE\"' EXIT" << endl;
    cout << endl;
    cout << "# show progress" << endl;
    cout << "infomsg \"reading database\"" << endl;
    cout << endl;
    cout << "# print database lines once" << endl;
    cout << "find \"$DBDIR\"                    \\" << endl;
    cout << "     -maxdepth \"1\"               \\" << endl;
    cout << "     -type \"f\"                   \\" << endl;
    cout << "     -regextype \"posix-extended\" \\" << endl;
    cout << "     -regex \".+\\.[Cc][Dd]$\"      \\" << endl;
    cout << "     -print0                     \\" << endl;
    cout << "| ripgen -zd                     \\" << endl;
    cout << "> \"$TEMPFILE\"" << endl;
    cout << endl;
    cout << "# set keys to list" << endl;
    cout << "for KEY in \"COMPOSER\"  \\" << endl;
    cout << "           \"ARRANGER\"  \\" << endl;
    cout << "           \"LYRICIST\"  \\" << endl;
    cout << "           \"CONDUCTOR\" \\" << endl;
    cout << "           \"ENSEMBLE\"  \\" << endl;
    cout << "           \"PERFORMER\" \\" << endl;
    cout << "           \"OPUS\"      \\" << endl;
    cout << "           \"ARTIST\"" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  # show progress" << endl;
    cout << "  infomsg \"operating key: $KEY\"" << endl;
    cout << endl;
    cout << "  # get all unique values" << endl;
    cout << "  sed -nre \"s/.*\\|($KEY=[^\\|]+)\\|.*/\\1/p\" \"$TEMPFILE\" \\" << endl;
    cout << "  | sort            \\" << endl;
    cout << "  | uniq            \\" << endl;
    cout << "  | semicolon_split \\" << endl;
    cout << "  | sort            \\" << endl;
    cout << "  | uniq            \\" << endl;
    cout << "  | escape_lines" << endl;
    cout << endl;
    cout << "# create files" << endl;
    cout << "done | while read KEYANDVALUE" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  # unescape line" << endl;
    cout << "  PAIR=$(unescape_line <<< \"$KEYANDVALUE\")" << endl;
    cout << endl;
    cout << "  # get key and value" << endl;
    cout << "  KEY=$(sed -re \"s/([^=]+)=(.+)/\\1/\" <<< \"$PAIR\")" << endl;
    cout << "  VAL=$(sed -re \"s/([^=]+)=(.+)/\\2/\" <<< \"$PAIR\")" << endl;
    cout << endl;
    cout << "  # set filename and name of the directory" << endl;
    cout << "  DNAME=$(sed -re \"s/[^[:alnum:]]+/_/g ; s/^_// ; s/_$// ; s/.+/\\L&/\" <<< \"$KEY\" | utf82ascii)" << endl;
    cout << "  FNAME=$(sed -re \"s/[^[:alnum:]]+/_/g ; s/^_// ; s/_$// ; s/.+/\\L&/\" <<< \"$VAL\" | utf82ascii)" << endl;
    cout << endl;
    cout << "  # skip invalid names" << endl;
    cout << "  if [ -z \"$DNAME\" ] || [ -z \"$FNAME\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    warnmsg \"skipping invalid pair: \\\"$PAIR\\\"\"" << endl;
    cout << endl;
    cout << "    # next cycle" << endl;
    cout << "    continue" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # create directory (if missing)" << endl;
    cout << "  mkdir -p \"$DNAME\"" << endl;
    cout << endl;
    cout << "  # create file" << endl;
    cout << "  echo \"$PAIR\" > \"$DNAME/$FNAME\"" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // --------------------
  // printMkDBIndexScript
  // --------------------
  /**
   * Print the bash code that creates the index.html (and related pre) file(s).
   */
  bool printMkDBIndexScript(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# settings                                                              settings" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# use dot as decimal separator" << endl;
    cout << "LC_NUMERIC='en_US.UTF-8'" << endl;
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
    cout << "# donemsg" << endl;
    cout << "# -------" << endl;
    cout << "#" << endl;
    cout << "# This function prints a done message via stderr." << endl;
    cout << "#" << endl;
    cout << "function donemsg()" << endl;
    cout << "{" << endl;
    cout << "  # push to stderr" << endl;
    cout << "  echo -e \"${GREEN}[DONE]${NONE} $1\" 1>&2" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# -------------" << endl;
    cout << "# find_cd_files" << endl;
    cout << "# -------------" << endl;
    cout << "#" << endl;
    cout << "#" << endl;
    cout << "#" << endl;
    cout << "function find_cd_files()" << endl;
    cout << "{" << endl;
    cout << "  # find all cd files" << endl;
    cout << "  find -mindepth '2'               \\" << endl;
    cout << "       -maxdepth '2'               \\" << endl;
    cout << "       -type 'f'                   \\" << endl;
    cout << "       -regextype 'posix-extended' \\" << endl;
    cout << "       -regex '.+\\.[Cc][Dd]$'      \\" << endl;
    cout << "       -print0" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ---------" << endl;
    cout << "# mkprefile" << endl;
    cout << "# ---------" << endl;
    cout << "#" << endl;
    cout << "# $1  cd file" << endl;
    cout << "#" << endl;
    cout << "function mkprefile()" << endl;
    cout << "{" << endl;
    cout << "  # get filenames" << endl;
    cout << "  DNAME=$(dirname \"$1\")" << endl;
    cout << "  BNAME=$(basename \"$1\")" << endl;
    cout << "  HNAME=$(sed -re 's/\\.[^\\.\\/]+$// ; s/$/.html/' <<< \"$BNAME\")" << endl;
    cout << endl;
    cout << "  # get title for the html file" << endl;
    cout << "  TITLE=$(sed -re 's/&/\\&amp;/g ; s/</\\&lt;/g ; s/>/\\&gt;/g' <<< \"$BNAME\")" << endl;
    cout << endl;
    cout << "  # show progress" << endl;
    cout << "  infomsg \"creating source file: \\\"$DNAME/$HNAME\\\"\"" << endl;
    cout << endl;
    cout << "  # redirect entire group" << endl;
    cout << "  {" << endl;
    cout << "    echo \"<!doctype html>\"" << endl;
    cout << "    echo \"<html lang=\\\"en\\\">\"" << endl;
    cout << "    echo \"<head>\"" << endl;
    cout << "    echo \"<meta charset=\\\"utf-8\\\" />\"" << endl;
    cout << "    echo \"<title>$TITLE</title>\"" << endl;
    cout << "    echo \"</head>\"" << endl;
    cout << "    echo \"<body>\"" << endl;
    cout << "    echo \"<pre>\"" << endl;
    cout << endl;
    cout << "    sed -re 's/&/\\&amp;/g ; s/</\\&lt;/g ; s/>/\\&gt;/g' \"$1\"" << endl;
    cout << endl;
    cout << "    echo \"</pre>\"" << endl;
    cout << "    echo \"</body>\"" << endl;
    cout << "    echo \"</html>\"" << endl;
    cout << endl;
    cout << "  } > \"$DNAME/$HNAME\"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# set name of the index file" << endl;
    cout << "INDEX='index.html'" << endl;
    cout << endl;
    cout << "# create html files" << endl;
    cout << "find_cd_files | sort -z | while read -rd $'\\0' FILENAME" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  # create html files" << endl;
    cout << "  mkprefile \"$FILENAME\"" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# show progress" << endl;
    cout << "infomsg \"creating index file: \\\"$INDEX\\\"\"" << endl;
    cout << endl;
    cout << "# redirect entire group" << endl;
    cout << "{" << endl;
    cout << "  echo \"<!doctype html>\"" << endl;
    cout << "  echo \"<html lang=\\\"en\\\">\"" << endl;
    cout << "  echo \"<head>\"" << endl;
    cout << "  echo \"<meta charset=\\\"utf-8\\\" />\"" << endl;
    cout << "  echo \"<title>dbindex</title>\"" << endl;
    cout << "  echo \"</head>\"" << endl;
    cout << "  echo \"<body style=\\\"font-family:sans-serif; font-size:small\\\">\"" << endl;
    cout << "  echo \"<ul>\"" << endl;
    cout << endl;
    cout << "  # create index file" << endl;
    cout << "  find_cd_files            \\" << endl;
    cout << "  | ripgen -dz             \\" << endl;
    cout << "  | sed -e 's/.*/&\\n&\\n&/' \\" << endl;
    cout << "  | sed --quiet            \\" << endl;
    cout << "        --regexp-extended  \\" << endl;
    cout << "        --expression=\"" << endl;
    cout << endl;
    cout << "          # first out of three lines" << endl;
    cout << "          1~3 {" << endl;
    cout << endl;
    cout << "            # get CDFILE value" << endl;
    cout << "            s/.*\\|CDFILE=([^\\|]*).*/\\1/" << endl;
    cout << endl;
    cout << "            # copy line to hold space" << endl;
    cout << "            h" << endl;
    cout << "          }" << endl;
    cout << endl;
    cout << "          # second out of three lines" << endl;
    cout << "          2~3 {" << endl;
    cout << endl;
    cout << "            # get ALBUMARTIST value" << endl;
    cout << "            s/.*\\|ALBUMARTIST=([^\\|]*).*/\\1/" << endl;
    cout << endl;
    cout << "            # escape special html characters" << endl;
    cout << "            s/&/\\&amp;/g" << endl;
    cout << "            s/</\\&lt;/g" << endl;
    cout << "            s/>/\\&gt;/g" << endl;
    cout << endl;
    cout << "            # append line to hold space" << endl;
    cout << "            H" << endl;
    cout << "          }" << endl;
    cout << endl;
    cout << "          # third out of three lines" << endl;
    cout << "          3~3 {" << endl;
    cout << endl;
    cout << "            # get ALBUM value" << endl;
    cout << "            s/.*\\|ALBUM=([^\\|]*).*/\\1/" << endl;
    cout << endl;
    cout << "            # escape special html characters" << endl;
    cout << "            s/&/\\&amp;/g" << endl;
    cout << "            s/</\\&lt;/g" << endl;
    cout << "            s/>/\\&gt;/g" << endl;
    cout << endl;
    cout << "            # append line to hold space" << endl;
    cout << "            H" << endl;
    cout << endl;
    cout << "            # copy hold space to pattern space" << endl;
    cout << "            g" << endl;
    cout << endl;
    cout << "            # replace NL characters" << endl;
    cout << "            s/\\n/|/g" << endl;
    cout << endl;
    cout << "            # print these lines" << endl;
    cout << "            /[^\\|]+\\|[^\\|]+\\|[^\\|]+/p" << endl;
    cout << "          }" << endl;
    cout << "        \"                 \\" << endl;
    cout << "  | sort                  \\" << endl;
    cout << "  | uniq                  \\" << endl;
    cout << "  | sort -t '|' -k '2'    \\" << endl;
    cout << "  | sed --quiet           \\" << endl;
    cout << "        --regexp-extended \\" << endl;
    cout << "        --expression=\"" << endl;
    cout << endl;
    cout << "          # set link target" << endl;
    cout << "          s/^([^\\|]+)\\.[^\\|\\.]+\\|/<li><a href=\\\"\\1.html\\\" target=\\\"_blank\\\">/" << endl;
    cout << endl;
    cout << "          # replace second pipe" << endl;
    cout << "          s/\\|/ - /" << endl;
    cout << endl;
    cout << "          # close list item" << endl;
    cout << "          s/$/<\\/a><\\/li>/p" << endl;
    cout << "        \"" << endl;
    cout << endl;
    cout << "  echo \"</ul>\"" << endl;
    cout << "  echo \"</body>\"" << endl;
    cout << "  echo \"</html>\"" << endl;
    cout << endl;
    cout << "} > \"$INDEX\"" << endl;
    cout << endl;
    cout << "# show progress" << endl;
    cout << "donemsg \"index file created: \\\"$INDEX\\\"\"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;
    cout << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // --------------------
  // printMkTagFileScript
  // --------------------
  /**
   * Print the bash code that creates single tag files.
   */
  bool printMkTagFileScript(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# functions                                                            functions" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# ------------" << endl;
    cout << "# escape_lines" << endl;
    cout << "# ------------" << endl;
    cout << "#" << endl;
    cout << "# no arguments needed: use as filter" << endl;
    cout << "#" << endl;
    cout << "function escape_lines()" << endl;
    cout << "{" << endl;
    cout << "  # prepare (NL separated) lines so they can pass 'read' unchanged" << endl;
    cout << "  sed --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << "        # skip empty lines" << endl;
    cout << "        /^[[:space:]]*$/ { d }" << endl;
    cout << endl;
    cout << "        # skip comments" << endl;
    cout << "        /^[[:space:]]*#/ { d }" << endl;
    cout << endl;
    cout << "        # escape each internal escape character (colon) first" << endl;
    cout << "        s/:/:c/g" << endl;
    cout << endl;
    cout << "        # escape characters treated special by 'read'" << endl;
    cout << "        s/\\x09/:t/g" << endl;
    cout << "        s/\\x20/:s/g" << endl;
    cout << "        s/\\\\\\\\/:b/g" << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# -------------" << endl;
    cout << "# unescape_line" << endl;
    cout << "# -------------" << endl;
    cout << "#" << endl;
    cout << "# no arguments needed: use as filter" << endl;
    cout << "#" << endl;
    cout << "function unescape_line()" << endl;
    cout << "{" << endl;
    cout << "  sed --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << "        # unescape special characters first" << endl;
    cout << "        s/:t/\\x09/g" << endl;
    cout << "        s/:s/\\x20/g" << endl;
    cout << "        s/:b/\\\\\\\\/g" << endl;
    cout << endl;
    cout << "        # unescape each internal escape character at the end" << endl;
    cout << "        s/:c/:/g" << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ----------" << endl;
    cout << "# utf82ascii" << endl;
    cout << "# ----------" << endl;
    cout << "#" << endl;
    cout << "# This filter converts a given UTF-8 stream to ascii." << endl;
    cout << "#" << endl;
    cout << "# ... | utf82ascii | ..." << endl;
    cout << "#" << endl;
    cout << "function utf82ascii()" << endl;
    cout << "{" << endl;
    cout << "  sed --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << "      # escape all colons" << endl;
    cout << "      s/:/:c/g" << endl;
    cout << endl;
    cout << "      # escape all native question marks (iconv may introduce much more)" << endl;
    cout << "      s/\\\\?/:q/g" << endl;
    cout << endl;
    cout << "      # collection of characters that aren't translated (suitable) by iconv" << endl;
    cout << "      s/¦/|/g" << endl;
    cout << "      s/¡/!/g" << endl;
    cout << "      s/¿/:q/g" << endl;
    cout << "      s/„/\\\"/g" << endl;
    cout << "      s/«/\\\"/g" << endl;
    cout << "      s/»/\\\"/g" << endl;
    cout << "      s/÷/\\//g" << endl;
    cout << "      s/±/+-/g" << endl;
    cout << "      s/¹/^1/g" << endl;
    cout << "      s/²/^2/g" << endl;
    cout << "      s/³/^3/g" << endl;
    cout << "      s/Ä/Ae/g" << endl;
    cout << "      s/Ö/Oe/g" << endl;
    cout << "      s/Ü/Ue/g" << endl;
    cout << "      s/Ø/Oe/g" << endl;
    cout << "      s/ø/oe/g" << endl;
    cout << "      s/Ð/Dh/g" << endl;
    cout << "      s/ð/dh/g" << endl;
    cout << "      s/Þ/Th/g" << endl;
    cout << "      s/þ/th/g" << endl;
    cout << "    \"                                         \\" << endl;
    cout << "  | iconv --from-code 'UTF-8'                 \\" << endl;
    cout << "          --to-code 'ASCII//TRANSLIT//IGNORE' \\" << endl;
    cout << "  | sed --regexp-extended                     \\" << endl;
    cout << "        --expression=\"" << endl;
    cout << "        # remove all question marks introduced by iconv" << endl;
    cout << "        s/\\\\?//g" << endl;
    cout << endl;
    cout << "        # restore all escaped question marks" << endl;
    cout << "        s/:q/?/g" << endl;
    cout << endl;
    cout << "        # restore all escaped colons" << endl;
    cout << "        s/:c/:/g" << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# --------------" << endl;
    cout << "# ascii2filename" << endl;
    cout << "# --------------" << endl;
    cout << "#" << endl;
    cout << "# This function creates a string that can be used as filename (ext4)." << endl;
    cout << "#" << endl;
    cout << "# ... | ascii2filename | ..." << endl;
    cout << "#" << endl;
    cout << "function ascii2filename()" << endl;
    cout << "{" << endl;
    cout << "  sed --regexp-extended \\" << endl;
    cout << "      --expression=\"" << endl;
    cout << endl;
    cout << "        # convert everything to lowercase" << endl;
    cout << "        s/.*/\\L&/" << endl;
    cout << endl;
    cout << "        # keep only numbers and letters" << endl;
    cout << "        s/[^[:alnum:]]+/_/g" << endl;
    cout << endl;
    cout << "        # remove leading and trailing underscores" << endl;
    cout << "        s/^_//" << endl;
    cout << "        s/_$//" << endl;
    cout << endl;
    cout << "        # assert maximum length" << endl;
    cout << "        s/^(.{255}).+/\\1/" << endl;
    cout << endl;
    cout << "      \"" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# read lines from stdin" << endl;
    cout << "escape_lines | while read ESCLINE" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  # restore initial version (protect leading and trailing spaces)" << endl;
    cout << "  LINE=$(unescape_line <<< \"~$ESCLINE~\")" << endl;
    cout << endl;
    cout << "  # print initial version" << endl;
    cout << "  sed -re 's/^~(.*)~$/\\1/' <<< \"$LINE\"" << endl;
    cout << endl;
    cout << "  # get key and value" << endl;
    cout << "  KEY=$(sed -nre 's/^~([^=[:space:]]+)=([^@].*)~$/\\1/p' <<< \"$LINE\")" << endl;
    cout << "  VAL=$(sed -nre 's/^~([^=[:space:]]+)=([^@].*)~$/\\2/p' <<< \"$LINE\")" << endl;
    cout << endl;
    cout << "  # skip empty values" << endl;
    cout << "  [ -z \"$KEY\" ] && continue" << endl;
    cout << "  [ -z \"$VAL\" ] && continue" << endl;
    cout << endl;
    cout << "  # create filename" << endl;
    cout << "  FILENAME=$(utf82ascii <<< \"$VAL\" | ascii2filename)" << endl;
    cout << endl;
    cout << "  # create tag file" << endl;
    cout << "  echo \"$KEY=$VAL\" > \"$FILENAME\"" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // -------------------
  // printSimplifyScript
  // -------------------
  /**
   * Print the bash code that simplifies track listings.
   */
  bool printSimplifyScript(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# simplify track listings" << endl;
    cout << "sed --regexp-extended \\" << endl;
    cout << "    --expression=\"" << endl;
    cout << endl;
    cout << "      # remove track numbers from the beginning" << endl;
    cout << "      s/^[[:space:]]*[[:digit:]]+\\.[[:space:]]*//" << endl;
    cout << endl;
    cout << "      # remove time data and all following characters" << endl;
    cout << "      s/[[:space:]]*[[:digit:]]+:[[:digit:]]{2}.*$//" << endl;
    cout << endl;
    cout << "      # replace TAB characters with PIPE characters" << endl;
    cout << "      s/\\x09/\\|/g" << endl;
    cout << endl;
    cout << "      # reduce and unify [:space:] characters" << endl;
    cout << "      s/[[:space:]]+/ /g" << endl;
    cout << endl;
    cout << "      # unify dashes" << endl;
    cout << "      s/[‐‑‒–—―]/-/g" << endl;
    cout << endl;
    cout << "      # unify apostrophes" << endl;
    cout << "      s/[\\`´‘’′]/'/g" << endl;
    cout << endl;
    cout << "      # replace all square brackets with round brackets" << endl;
    cout << "      s/\\[/(/g" << endl;
    cout << "      s/\\]/)/g" << endl;
    cout << endl;
    cout << "      # get text from inside quotation marks" << endl;
    cout << "      s/^[^\\\"]*\\\"([^\\\"]*)\\\"[^\\\"]*$/\\1/" << endl;
    cout << endl;
    cout << "      # remove leading and trailing spaces" << endl;
    cout << "      s/^ +//" << endl;
    cout << "      s/ +$//" << endl;
    cout << endl;
    cout << "      # capitalize initial letters" << endl;
    cout << "      s/^([[:alpha:]])/\\U\\1/" << endl;
    cout << "      s/ ([[:alpha:]])/ \\U\\1/g" << endl;
    cout << "      s/\\(([[:alpha:]])/(\\U\\1/g" << endl;
    cout << endl;
    cout << "      # don't print empty lines" << endl;
    cout << "      /^[[:space:]]*$/ { d }" << endl;
    cout << endl;
    cout << "    \"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // -------------------
  // printAddTitleScript
  // -------------------
  /**
   * Print the bash code that inserts TITLE keys at the beginning of each line.
   */
  bool printAddTitleScript(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# tag lines that hold at least one [:alnum:] character" << endl;
    cout << "sed --quiet           \\" << endl;
    cout << "    --regexp-extended \\" << endl;
    cout << "    --expression=\"" << endl;
    cout << endl;
    cout << "      # skip empty lines" << endl;
    cout << "      /^[[:space:]]*$/ { d }" << endl;
    cout << endl;
    cout << "      # remove key from the beginning" << endl;
    cout << "      s/^TITLE=//" << endl;
    cout << endl;
    cout << "      # add key and remove trailing spaces" << endl;
    cout << "      s/^(.+)[[:space:]]*$/TITLE=\\1/p" << endl;
    cout << endl;
    cout << "    \"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ---------------
  // printDig3Script
  // ---------------
  /**
   * Print the bash code that renames two-digit wav files.
   */
  bool printDig3Script(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# settings                                                              settings" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# use dot as decimal separator" << endl;
    cout << "LC_NUMERIC='en_US.UTF-8'" << endl;
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
    cout << "# ---------------------" << endl;
    cout << "# show_version_and_exit" << endl;
    cout << "# ---------------------" << endl;
    cout << "#" << endl;
    cout << "# This function shows the script's version and terminates the script." << endl;
    cout << "#" << endl;
    cout << "function show_version_and_exit()" << endl;
    cout << "{" << endl;
    cout << "  echo" << endl;
    cout << "  echo 'version 2016-11-21.1'" << endl;
    cout << "  echo" << endl;
    cout << "  exit 1" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------" << endl;
    cout << "# show_help_and_exit" << endl;
    cout << "# ------------------" << endl;
    cout << "#" << endl;
    cout << "# This function shows the script's help and terminates it." << endl;
    cout << "#" << endl;
    cout << "function show_help_and_exit()" << endl;
    cout << "{" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"NAME\"" << endl;
    cout << "  echo \"    dig3 - three-digit track numbers\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"SYNOPSIS\"" << endl;
    cout << "  echo \"    dig3 [options] [<directory>]\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"DESCRIPTION\"" << endl;
    cout << "  echo \"    dig3 renames wav files from cdparanoia that have a two-digit track number:\"" << endl;
    cout << "  echo \"    track01.cdda.wav  -->  track001.cdda.wav\"" << endl;
    cout << "  echo \"    track02.cdda.wav  -->  track002.cdda.wav\"" << endl;
    cout << "  echo \"                      ...\"" << endl;
    cout << "  echo \"    track99.cdda.wav  -->  track099.cdda.wav\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"OPTIONS\"" << endl;
    cout << "  echo \"    -h        show help and exit\"" << endl;
    cout << "  echo \"    -v        show version and exit\"" << endl;
    cout << "  echo \"    -a <num>  add <num> to each two-digit track number\"" << endl;
    cout << "  echo" << endl;
    cout << "  exit 1" << endl;
    cout << "}" << endl;
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
    cout << "# donemsg" << endl;
    cout << "# -------" << endl;
    cout << "#" << endl;
    cout << "# This function prints a done message via stderr." << endl;
    cout << "#" << endl;
    cout << "function donemsg()" << endl;
    cout << "{" << endl;
    cout << "  # push to stderr" << endl;
    cout << "  echo -e \"${GREEN}[DONE]${NONE} $1\" 1>&2" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ----------" << endl;
    cout << "# is_integer" << endl;
    cout << "# ----------" << endl;
    cout << "#" << endl;
    cout << "# $1  number" << endl;
    cout << "#" << endl;
    cout << "function is_integer()" << endl;
    cout << "{" << endl;
    cout << "  # no argument given" << endl;
    cout << "  [ -z \"$1\" ] && return 1" << endl;
    cout << endl;
    cout << "  # try to print as integer" << endl;
    cout << "  printf '%d' \"$1\" &>'/dev/null' || return 1" << endl;
    cout << endl;
    cout << "  # integer found" << endl;
    cout << "  return 0" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# options                                                                options" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# set default offset" << endl;
    cout << "OFFSET=0" << endl;
    cout << endl;
    cout << "# check passed options" << endl;
    cout << "while getopts ':hva:' OPTION \"$@\"" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  case \"$OPTION\" in" << endl;
    cout << endl;
    cout << "    'h') show_help_and_exit" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'v') show_version_and_exit" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    'a') OFFSET=\"$OPTARG\"" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    '?') failmsg \"unknown option: -$OPTARG\"" << endl;
    cout << "         exit 1" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "    ':') failmsg \"missing argument: -$OPTARG <argument>\"" << endl;
    cout << "         exit 1" << endl;
    cout << "         ;;" << endl;
    cout << endl;
    cout << "  esac" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# drop parsed options" << endl;
    cout << "shift $(( OPTIND - 1 ))" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# set default value" << endl;
    cout << "TOPDIR='.'" << endl;
    cout << endl;
    cout << "# check first positional parameter" << endl;
    cout << "if [ -n \"$1\" ] ; then" << endl;
    cout << endl;
    cout << "  # use given directory" << endl;
    cout << "  TOPDIR=\"$1\"" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# check directory" << endl;
    cout << "if [ ! -d \"$TOPDIR\" ] ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"unable to locate directory: \\\"$TOPDIR\\\"\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# check offset" << endl;
    cout << "if ! is_integer \"$OFFSET\" ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"invalid number: \\\"$OFFSET\\\"\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# find all wav files with two digits in the current directory" << endl;
    cout << "find \"$TOPDIR\"                                   \\" << endl;
    cout << "     -maxdepth '1'                               \\" << endl;
    cout << "     -type 'f'                                   \\" << endl;
    cout << "     -regextype 'posix-extended'                 \\" << endl;
    cout << "     -regex '.+/track[[:digit:]]{2}\\.cdda\\.wav$' \\" << endl;
    cout << "     -print0                                     \\" << endl;
    cout << "| sort -z                                        \\" << endl;
    cout << "| while read -rd $'\\0' FILENAME" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  # split filename" << endl;
    cout << "  DNAME=$(dirname \"$FILENAME\")" << endl;
    cout << "  BNAME=$(basename \"$FILENAME\")" << endl;
    cout << endl;
    cout << "  # get track number" << endl;
    cout << "  NUMBER=$(sed -nre 's|^track0*([[:digit:]]+)\\.cdda\\.wav$|\\1|p' <<< \"$BNAME\")" << endl;
    cout << endl;
    cout << "  # add offset" << endl;
    cout << "  NUMBER=$(bc <<< \"$NUMBER + $OFFSET\")" << endl;
    cout << endl;
    cout << "  # set new filename" << endl;
    cout << "  NEWNAME=$(printf '%s/track%03d.cdda.wav' \"$DNAME\" \"$NUMBER\")" << endl;
    cout << endl;
    cout << "  # check if file already exists" << endl;
    cout << "  if [ -f \"$NEWNAME\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    failmsg \"file already exists: \\\"$NEWNAME\\\"\"" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    exit 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # show progress" << endl;
    cout << "  echo \"$FILENAME  -->  $NEWNAME\"" << endl;
    cout << endl;
    cout << "  # rename file" << endl;
    cout << "  mv --no-clobber \"$FILENAME\" \"$NEWNAME\"" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ----------------------
  // printUpdateGenreScript
  // ----------------------
  /**
   * Print the bash code that updates the GENRE comment.
   */
  bool printUpdateGenreScript(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# settings                                                              settings" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# use dot as decimal separator" << endl;
    cout << "LC_NUMERIC='en_US.UTF-8'" << endl;
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
    cout << "# donemsg" << endl;
    cout << "# -------" << endl;
    cout << "#" << endl;
    cout << "# This function prints a done message via stderr." << endl;
    cout << "#" << endl;
    cout << "function donemsg()" << endl;
    cout << "{" << endl;
    cout << "  # push to stderr" << endl;
    cout << "  echo -e \"${GREEN}[DONE]${NONE} $1\" 1>&2" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# check number of arguments" << endl;
    cout << "if (( $# != 2 )) ; then" << endl;
    cout << endl;
    cout << "  # get own filename" << endl;
    cout << "  THISNAME=$(basename \"$0\")" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"exactly two arguments needed: $THISNAME <genre> <directory>\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# name positional arguments" << endl;
    cout << "GENRE=$(sed -re 's/^GENRE=//' <<< \"$1\")" << endl;
    cout << "START=\"$2\"" << endl;
    cout << endl;
    cout << "# check genre" << endl;
    cout << "if [ -z \"$GENRE\" ] ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"no genre given\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# check directory" << endl;
    cout << "if [ ! -d \"$START\" ] ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"this is no directory: \\\"$START\\\"\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# find flac files" << endl;
    cout << "find \"$START\"                       \\" << endl;
    cout << "     -type 'f'                      \\" << endl;
    cout << "     -regextype 'posix-extended'    \\" << endl;
    cout << "     -regex '.+\\.[Ff][Ll][Aa][Cc]$' \\" << endl;
    cout << "     -print0                        \\" << endl;
    cout << "| sort -z                           \\" << endl;
    cout << "| while read -rd $'\\0' FILENAME" << endl;
    cout << "do" << endl;
    cout << endl;
    cout << "  # check if file is empty" << endl;
    cout << "  if [ ! -s \"$FILENAME\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    warnmsg \"skipping file (empty file): \\\"$FILENAME\\\"\"" << endl;
    cout << endl;
    cout << "    # next cycle" << endl;
    cout << "    continue" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # check if file is writable" << endl;
    cout << "  if [ ! -w \"$FILENAME\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    warnmsg \"skipping file (no write permission): \\\"$FILENAME\\\"\"" << endl;
    cout << endl;
    cout << "    # next cycle" << endl;
    cout << "    continue" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # get base name" << endl;
    cout << "  BASENAME=$(basename \"$FILENAME\")" << endl;
    cout << endl;
    cout << "  # show progress" << endl;
    cout << "  echo -e \"${BLUE}${BASENAME}${NONE}\"" << endl;
    cout << endl;
    cout << "  # remove all tags whose field name is 'GENRE'" << endl;
    cout << "  metaflac --remove-tag=\"GENRE\" \"$FILENAME\"" << endl;
    cout << endl;
    cout << "  # set new genre" << endl;
    cout << "  metaflac --set-tag=\"GENRE=$GENRE\" \"$FILENAME\"" << endl;
    cout << endl;
    cout << "  # show comments" << endl;
    cout << "  metaflac --list --block-type='VORBIS_COMMENT' \"$FILENAME\"" << endl;
    cout << endl;
    cout << "  # insert empty line" << endl;
    cout << "  echo" << endl;
    cout << endl;
    cout << "done" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ------------------------
  // printCommonGeneratorPart
  // ------------------------
  /**
   *
   */
  void printCommonGeneratorPart(ofstream& cout)
  {
    cout << "#!/bin/bash" << endl;
    cout << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# settings                                                              settings" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# terminal colors" << endl;
    cout << "NONE=$(tput sgr0)" << endl;
    cout << " RED=$(tput setaf 1)" << endl;
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
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << "# commands                                                              commands" << endl;
    cout << "# ------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "# count passed arguments" << endl;
    cout << "if (( $# != 1 )) ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg 'no filename given'" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# get absolute path" << endl;
    cout << "SRCFILE=$(readlink -e \"$1\")" << endl;
    cout << endl;
    cout << "# check path" << endl;
    cout << "if [ -z \"$SRCFILE\" ] ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"unable to locate file: \\\"$SRCFILE\\\"\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# get image type" << endl;
    cout << "IMAGETYPE=$(identify -format '%m' \"$SRCFILE\" 2>'/dev/null')" << endl;
    cout << endl;
    cout << "# check image type" << endl;
    cout << "if [ \"$IMAGETYPE\" != 'JPEG' ] \\" << endl;
    cout << "&& [ \"$IMAGETYPE\" != 'PNG'  ] ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"invalid image type: \\\"$SRCFILE\\\"\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# get width and height of the image" << endl;
    cout << "DIMENSION=$(identify -format '%wx%h' \"$SRCFILE\")" << endl;
    cout << endl;
    cout << "# check dimension" << endl;
    cout << "if [ \"$DIMENSION\" != '300x300' ] ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"invalid image size: \\\"$DIMENSION\\\" (expected: 300x300)\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# calculate sha1 sum" << endl;
    cout << "CHKSUM=$(sha1sum \"$1\" | sed -re 's/ .+//')" << endl;
    cout << endl;
    cout << "# reset extension" << endl;
    cout << "IMGEXT=''" << endl;
    cout << endl;
    cout << "# set appropriate extension" << endl;
    cout << "[ \"$IMAGETYPE\" == 'JPEG' ] && IMGEXT='jpg'" << endl;
    cout << "[ \"$IMAGETYPE\" == 'PNG'  ] && IMGEXT='png'" << endl;
    cout << endl;
    cout << "# get directory from image file" << endl;
    cout << "DIRNAME=$(dirname \"$SRCFILE\")" << endl;
    cout << endl;
    cout << "# set output filenames" << endl;
    cout << "TAGFILE=\"$DIRNAME/$CHKSUM.cd\"" << endl;
    cout << "IMGFILE=\"$DIRNAME/$CHKSUM.$IMGEXT\"" << endl;
    cout << endl;
    cout << "# check name of the tag file" << endl;
    cout << "if [ -s \"$TAGFILE\" ] ; then" << endl;
    cout << endl;
    cout << "  # notify user" << endl;
    cout << "  failmsg \"file already exists: \\\"$TAGFILE\\\"\"" << endl;
    cout << endl;
    cout << "  # signalize trouble" << endl;
    cout << "  exit 1" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
    cout << "# check if filenames are different" << endl;
    cout << "if [ \"$SRCFILE\" != \"$IMGFILE\" ] ; then" << endl;
    cout << endl;
    cout << "  # check if target file already exists" << endl;
    cout << "  if [ -s \"$IMGFILE\" ] ; then" << endl;
    cout << endl;
    cout << "    # notify user" << endl;
    cout << "    failmsg \"file already exists: \\\"$IMGFILE\\\"\"" << endl;
    cout << endl;
    cout << "    # signalize trouble" << endl;
    cout << "    exit 1" << endl;
    cout << endl;
    cout << "  fi" << endl;
    cout << endl;
    cout << "  # rename image" << endl;
    cout << "  mv \"$SRCFILE\" \"$IMGFILE\"" << endl;
    cout << endl;
    cout << "fi" << endl;
    cout << endl;
  }

  // ------------
  // printTGalbum
  // ------------
  /**
   *
   */
  bool printTGalbum(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    printCommonGeneratorPart(cout);

    cout << "# create tag file" << endl;
    cout << "{" << endl;
    cout << endl;
    cout << "  echo \"FILENAME=%f|\\$ARTIST|/%f|\\$DATE \\$ALBUM|/%f|%2z~\\$TRACKNUMBER~ \\$TITLE|.flac\"" << endl;
    cout << "  echo \"IMAGE=\\$COMPILATIONID.$IMGEXT\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"COMPILATIONID=$CHKSUM\"" << endl;
    cout << "  echo \"ARTIST=@\"" << endl;
    cout << "  echo \"ALBUMARTIST=\\$ARTIST\"" << endl;
    cout << "  echo \"ALBUM=@\"" << endl;
    cout << "  echo \"GENRE=@\"" << endl;
    cout << "  echo \"DATE=@\"" << endl;
    cout << "  echo \"TRACKTOTAL=@\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"TITLE=@\"" << endl;
    cout << endl;
    cout << "} > \"$TAGFILE\"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ---------------------
  // printTGvariousArtists
  // ---------------------
  /**
   * A template generator for sampler CDs with various artists.
   */
  bool printTGvariousArtists(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    printCommonGeneratorPart(cout);

    cout << "# create tag file" << endl;
    cout << "{" << endl;
    cout << endl;
    cout << "  echo \"FILENAME=%f|\\$GENRE|/%f|\\$ALBUM \\$DATE|/%f|%2z~\\$TRACKNUMBER~ \\$TITLE|.flac\"" << endl;
    cout << "  echo \"IMAGE=\\$COMPILATIONID.$IMGEXT\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"COMPILATIONID=$CHKSUM\"" << endl;
    cout << "  echo \"ALBUMARTIST=Various Artists\"" << endl;
    cout << "  echo \"ALBUM=@\"" << endl;
    cout << "  echo \"GENRE=@\"" << endl;
    cout << "  echo \"DATE=@\"" << endl;
    cout << "  echo \"TRACKTOTAL=@\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"ARTIST=@\"" << endl;
    cout << "  echo \"TITLE=@\"" << endl;
    cout << endl;
    cout << "} > \"$TAGFILE\"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ----------------
  // printTGaudiobook
  // ----------------
  /**
   *
   */
  bool printTGaudiobook(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    printCommonGeneratorPart(cout);

    cout << "# create tag file" << endl;
    cout << "{" << endl;
    cout << endl;
    cout << "  echo \"FILENAME=%f|\\$ARTIST|/%f|\\$ALBUM|/%f|%3z~\\$TRACKNUMBER~ \\$TITLE|.flac\"" << endl;
    cout << "  echo \"IMAGE=\\$COMPILATIONID.$IMGEXT\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"COMPILATIONID=$CHKSUM\"" << endl;
    cout << "  echo \"AUTHOR=@LASTNAME, @FIRSTNAME\"" << endl;
    cout << "  echo \"PERFORMER=@LASTNAME, @FIRSTNAME: Rezitator\"" << endl;
    cout << "  echo \"ARTIST=\\$AUTHOR\"" << endl;
    cout << "  echo \"ALBUMARTIST=\\$ARTIST\"" << endl;
    cout << "  echo \"ALBUM=@BOOK - %1c~\\$PERFORMER~ (\\$DATE)\"" << endl;
    cout << "  echo \"GENRE=Speech (Audiobook)\"" << endl;
    cout << "  echo \"DATE=@\"" << endl;
    cout << "  echo \"TRACKTOTAL=@\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"TITLE=@\"" << endl;
    cout << endl;
    cout << "} > \"$TAGFILE\"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ----------------
  // printTGaudioplay
  // ----------------
  /**
   *
   */
  bool printTGaudioplay(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    printCommonGeneratorPart(cout);

    cout << "# create tag file" << endl;
    cout << "{" << endl;
    cout << endl;
    cout << "  echo \"FILENAME=%f|\\$ARTIST|/%f|\\$ALBUM|/%f|%3z~\\$TRACKNUMBER~ \\$TITLE|.flac\"" << endl;
    cout << "  echo \"IMAGE=\\$COMPILATIONID.$IMGEXT\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"COMPILATIONID=$CHKSUM\"" << endl;
    cout << "  echo \"ARTIST=@SERIAL\"" << endl;
    cout << "  echo \"ALBUMARTIST=\\$ARTIST\"" << endl;
    cout << "  echo \"ALBUM=@NUMBER - @EPISODE\"" << endl;
    cout << "  echo \"GENRE=Speech (Audio Play)\"" << endl;
    cout << "  echo \"DATE=@\"" << endl;
    cout << "  echo \"TRACKTOTAL=@\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"TITLE=@\"" << endl;
    cout << endl;
    cout << "} > \"$TAGFILE\"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ----------------
  // printTGeducation
  // ----------------
  /**
   *
   */
  bool printTGeducation(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    printCommonGeneratorPart(cout);

    cout << "# create tag file" << endl;
    cout << "{" << endl;
    cout << endl;
    cout << "  echo \"FILENAME=%f|\\$ARTIST|/%f|\\$ALBUM|/%f|%3z~\\$TRACKNUMBER~ \\$TITLE|.flac\"" << endl;
    cout << "  echo \"IMAGE=\\$COMPILATIONID.$IMGEXT\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"COMPILATIONID=$CHKSUM\"" << endl;
    cout << "  echo \"ARTIST=@SERIAL\"" << endl;
    cout << "  echo \"ALBUMARTIST=\\$ARTIST\"" << endl;
    cout << "  echo \"ALBUM=@NUMBER - @EPISODE\"" << endl;
    cout << "  echo \"GENRE=Speech (Education)\"" << endl;
    cout << "  echo \"DATE=@\"" << endl;
    cout << "  echo \"TRACKTOTAL=@\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"TITLE=@\"" << endl;
    cout << endl;
    cout << "} > \"$TAGFILE\"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // ----------------
  // printTGclassical
  // ----------------
  /**
   *
   */
  bool printTGclassical(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    printCommonGeneratorPart(cout);

    cout << "# create tag file" << endl;
    cout << "{" << endl;
    cout << endl;
    cout << "  echo \"FILENAME=%f|%1c~\\$COMPOSER~|/%f|\\$OPUS|/%f|\\$_RECID|/%f|%2z~\\$TRACKNUMBER~ \\$TITLE|.flac\"" << endl;
    cout << "  echo \"IMAGE=\\$COMPILATIONID.png\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"COMPILATIONID=$CHKSUM\"" << endl;
    cout << "  echo \"GENRE=Classical\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"ARTIST=%1c~\\$COMPOSER~\"" << endl;
    cout << "  echo \"ALBUMARTIST=\\$ARTIST\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"ALBUM=\\$OPUS - \\$_RECID\"" << endl;
    cout << "  echo \"#ALBUM=\\$OPUS (Version: \\$VERSION) (arr. %1c~\\$ARRANGER~) - \\$_RECID\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"COMPOSER=@LASTNAME, @FIRSTNAME (@BORN-@DIED)\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"_RECID=%1c~\\$CONDUCTOR~ (\\$DATE)\"" << endl;
    cout << "  echo \"#_RECID=%1c~\\$PERFORMER~ (\\$DATE)\"" << endl;
    cout << "  echo \"#_RECID=%1c~\\$PERFORMER~; %1c~\\$CONDUCTOR~ (\\$DATE)\"" << endl;
    cout << "  echo \"#_RECID=%q>%1c+%1s~\\$PERFORMER~+; %1c+%2s~\\$PERFORMER~+; %1c~\\$CONDUCTOR~ (\\$DATE)>\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"CONDUCTOR=@LASTNAME, @FIRSTNAME (@BORN-@DIED)\"" << endl;
    cout << "  echo \"ENSEMBLE=@ORCHESTRA; @CHORUS\"" << endl;
    cout << "  echo \"#PERFORMER=@LASTNAME, @FIRSTNAME: @INSTRUMENT; @LASTNAME, @FIRSTNAME: @INSTRUMENT\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"#COMMENT=@Konzertmitschnitt\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"DATE=@YYYY\"" << endl;
    cout << "  echo \"OPUS=@OPUS\"" << endl;
    cout << "  echo \"#VERSION=@VERSION\"" << endl;
    cout << "  echo \"#ARRANGER=@LASTNAME, @FIRSTNAME (@BORN-@DIED)\"" << endl;
    cout << "  echo \"#LYRICIST=@LASTNAME, @FIRSTNAME (@BORN-@DIED)\"" << endl;
    cout << "  echo \"TRACKTOTAL=@ONTHISCOMPILATION\"" << endl;
    cout << "  echo \"TITLE=@TITLE\"" << endl;
    cout << endl;
    cout << "} > \"$TAGFILE\"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // -----------------
  // printTGsoundtrack
  // -----------------
  /**
   *
   */
  bool printTGsoundtrack(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    printCommonGeneratorPart(cout);

    cout << "# create tag file" << endl;
    cout << "{" << endl;
    cout << endl;
    cout << "  echo \"FILENAME=%f|\\$ALBUM \\$DATE|/%f|%2z~\\$TRACKNUMBER~ \\$TITLE|.flac\"" << endl;
    cout << "  echo \"IMAGE=\\$COMPILATIONID.$IMGEXT\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"COMPILATIONID=$CHKSUM\"" << endl;
    cout << "  echo \"ARTIST=@\"" << endl;
    cout << "  echo \"ALBUMARTIST=\\$ARTIST\"" << endl;
    cout << "  echo \"ALBUM=@MOVIE\"" << endl;
    cout << "  echo \"GENRE=Soundtrack\"" << endl;
    cout << "  echo \"DATE=@\"" << endl;
    cout << "  echo \"TRACKTOTAL=@\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"TITLE=@\"" << endl;
    cout << endl;
    cout << "} > \"$TAGFILE\"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // -------------------------------
  // printTGsoundtrackVariousArtists
  // -------------------------------
  /**
   *
   */
  bool printTGsoundtrackVariousArtists(const string& filename)
  {
    // open file for writing
    ofstream cout( filename.c_str() );

    // check if file has been opened
    if ( !cout.is_open() )
    {
      // notify user
      msg::err( msg::catq("unable to open file: ", filename) );

      // signalize trouble
      return false;
    }

    printCommonGeneratorPart(cout);

    cout << "# create tag file" << endl;
    cout << "{" << endl;
    cout << endl;
    cout << "  echo \"FILENAME=%f|\\$ALBUM \\$DATE|/%f|%2z~\\$TRACKNUMBER~ \\$TITLE|.flac\"" << endl;
    cout << "  echo \"IMAGE=\\$COMPILATIONID.$IMGEXT\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"COMPILATIONID=$CHKSUM\"" << endl;
    cout << "  echo \"ALBUMARTIST=Various Artists\"" << endl;
    cout << "  echo \"ALBUM=@MOVIE\"" << endl;
    cout << "  echo \"GENRE=Soundtrack\"" << endl;
    cout << "  echo \"DATE=@\"" << endl;
    cout << "  echo \"TRACKTOTAL=@\"" << endl;
    cout << "  echo" << endl;
    cout << "  echo \"ARTIST=@\"" << endl;
    cout << "  echo \"TITLE=@\"" << endl;
    cout << endl;
    cout << "} > \"$TAGFILE\"" << endl;
    cout << endl;
    cout << "# signalize success" << endl;
    cout << "exit 0" << endl;

    // check state of stream
    if ( !cout )
    {
      // notify user
      msg::err( msg::catq("unable to create script: ", filename) );

      // close file
      cout.close();

      // signalize trouble
      return false;
    }

    // close file
    cout.close();

    // signalize success
    return true;
  }

  // -------------
  // setExecutable
  // -------------
  /**
   *
   */
  bool setExecutable(const string& filename)
  {
    // define stat struct named buf
    struct stat buf;

    // get file status
    stat(filename.c_str(), &buf);

    // try to add executable flag for owner
    if (chmod(filename.c_str(), (buf.st_mode | S_IXUSR)) < 0)
    {
      // notify user
      msg::err( msg::catq("unable to set owner's execute flag: ", filename) );

      // signalize error
      return false;
    }

    // flag successfully set
    return true;
  }

  // ----------------
  // createAllScripts
  // ----------------
  /*
   *
   */
  bool createAllScripts()
  {
    string filename;

    filename = "trip";
    if ( !printTripScript(filename) ) return false;
    if ( !setExecutable(filename)   ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "back2track";
    if ( !printBack2TrackScript(filename) ) return false;
    if ( !setExecutable(filename)         ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "lsdb";
    if ( !printLSDBScript(filename) ) return false;
    if ( !setExecutable(filename)   ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "mkdbimport";
    if ( !printMkDBImportScript(filename) ) return false;
    if ( !setExecutable(filename)         ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "mkdbindex";
    if ( !printMkDBIndexScript(filename) ) return false;
    if ( !setExecutable(filename)        ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "mktagfile";
    if ( !printMkTagFileScript(filename) ) return false;
    if ( !setExecutable(filename)        ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "simplify";
    if ( !printSimplifyScript(filename) ) return false;
    if ( !setExecutable(filename)       ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "addtitle";
    if ( !printAddTitleScript(filename) ) return false;
    if ( !setExecutable(filename)       ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "dig3";
    if ( !printDig3Script(filename) ) return false;
    if ( !setExecutable(filename)   ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "updateGENRE";
    if ( !printUpdateGenreScript(filename) ) return false;
    if ( !setExecutable(filename)          ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "new.album";
    if ( !printTGalbum(filename)  ) return false;
    if ( !setExecutable(filename) ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "new.album_va";
    if ( !printTGvariousArtists(filename) ) return false;
    if ( !setExecutable(filename)         ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "new.book";
    if ( !printTGaudiobook(filename) ) return false;
    if ( !setExecutable(filename)    ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "new.play";
    if ( !printTGaudioplay(filename) ) return false;
    if ( !setExecutable(filename)    ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "new.education";
    if ( !printTGeducation(filename) ) return false;
    if ( !setExecutable(filename)    ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "new.classical";
    if ( !printTGclassical(filename) ) return false;
    if ( !setExecutable(filename)    ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "new.ost";
    if ( !printTGsoundtrack(filename) ) return false;
    if ( !setExecutable(filename)     ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    filename = "new.ost_va";
    if ( !printTGsoundtrackVariousArtists(filename) ) return false;
    if ( !setExecutable(filename)                   ) return false;
    msg::nfo( msg::cat("script created: ", filename) );

    // signalize success
    return true;
  }

}

