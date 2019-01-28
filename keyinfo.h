// -----------------------------------------------------------------------------
// keyinfo.h                                                           keyinfo.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file declares 'public' members of the @ref keyinfo namespace.
 * @author     Col. Walter E. Kurtz
 * @version    2016-12-01
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef KEYINFO_H_INCLUDE_NO1
#define KEYINFO_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <vector>
#include <string>


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -------
// keyinfo
// -------
/**
 * @brief  The @a keyinfo namespace defines functions that provide
 *         information about the keys used in tag files (KEY=VALUE).
 */
namespace keyinfo
{

  typedef enum
  {
    ALBUM,
    ALBUMARTIST,
    ARRANGER,
    ARTIST,
    AUTHOR,
    COMMENT,
    COMPILATIONID,
    COMPILATIONINDEX,
    COMPOSER,
    CONDUCTOR,
    DATE,
    ENSEMBLE,
    FILENAME,
    GENRE,
    IMAGE,
    LYRICIST,
    OPUS,
    PERFORMER,
    TITLE,
    TRACKNUMBER,
    TRACKTOTAL,
    VERSION
  }
  KEYID;

  /// number of keys defined in KEYID
  extern const unsigned keyCount;

  /// number of characters in the longest key name
  extern const unsigned maxNameSize;


  // ----
  // name
  // ----
  /**
   *
   */
  string name(unsigned keyID);

  // ---------
  // isDefined
  // ---------
  /**
   *
   */
  bool isDefined(unsigned keyID);

  // ----------
  // isWritable
  // ----------
  /**
   *
   */
  bool isWritable(unsigned keyID);

  // ---------------
  // isVorbisComment
  // ---------------
  /**
   *
   */
  bool isVorbisComment(unsigned keyID);

  // ---------
  // isDefined
  // ---------
  /**
   *
   */
  bool isDefined(const string& key);

  // ----------
  // isWritable
  // ----------
  /**
   *
   */
  bool isWritable(const string& key);

  // ---------------
  // isVorbisComment
  // ---------------
  /**
   *
   */
  bool isVorbisComment(const string& key);

}

#endif  /* #ifndef KEYINFO_H_INCLUDE_NO1 */

