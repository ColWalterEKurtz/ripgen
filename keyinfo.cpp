// -----------------------------------------------------------------------------
// keyinfo.cpp                                                       keyinfo.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file defines all members of the @ref keyinfo namespace.
 * @author     Col. Walter E. Kurtz
 * @version    2016-12-01
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include "keyinfo.h"


// -----------------------------------------------------------------------------
// Definitions                                                       Definitions
// -----------------------------------------------------------------------------
namespace keyinfo
{

  /// the list of key names (must be synchronous with the KEYID enum)
  const char *const keys[] =
  {
    "ALBUM",
    "ALBUMARTIST",
    "ARRANGER",
    "ARTIST",
    "AUTHOR",
    "COMMENT",
    "COMPILATIONID",
    "COMPILATIONINDEX",
    "COMPOSER",
    "CONDUCTOR",
    "DATE",
    "ENSEMBLE",
    "FILENAME",
    "GENRE",
    "IMAGE",
    "LYRICIST",
    "OPUS",
    "PERFORMER",
    "TITLE",
    "TRACKNUMBER",
    "TRACKTOTAL",
    "VERSION"
  };

  // number of keys
  const unsigned keyCount = 22;

  // COMPILATIONINDEX
  const unsigned maxNameSize = 16;


  // ----
  // name
  // ----
  /*
   *
   */
  string name(unsigned keyID)
  {
    return keys[keyID];
  }

  // ---------
  // isDefined
  // ---------
  /*
   *
   */
  bool isDefined(unsigned keyID)
  {
    // check if ID is less than count
    return (keyID < keyCount);
  }

  // ----------
  // isWritable
  // ----------
  /*
   *
   */
  bool isWritable(unsigned keyID)
  {
    // only COMPILATIONINDEX is not writable
    if (keyID == COMPILATIONINDEX) return false;

    // all other keys are writable
    return true;
  }

  // ---------------
  // isVorbisComment
  // ---------------
  /*
   *
   */
  bool isVorbisComment(unsigned keyID)
  {
    // no vorbis comments
    if (keyID == FILENAME) return false;
    if (keyID == IMAGE)    return false;

    // most keys are vorbis comments
    return true;
  }

  // ---------
  // isDefined
  // ---------
  /*
   *
   */
  bool isDefined(const string& key)
  {
    // search given key
    for(unsigned i = 0; i < keyCount; i++)
    {
      // key found
      if (key == keys[i])
      {
        return true;
      }
    }

    // key not found
    return false;
  }

  // ----------
  // isWritable
  // ----------
  /*
   *
   */
  bool isWritable(const string& key)
  {
    // search given key
    for(unsigned i = 0; i < keyCount; i++)
    {
      // key found
      if (key == keys[i])
      {
        // call this function
        return isWritable(i);
      }
    }

    // key not found
    return false;
  }

  // ---------------
  // isVorbisComment
  // ---------------
  /*
   *
   */
  bool isVorbisComment(const string& key)
  {
    // search given key
    for(unsigned i = 0; i < keyCount; i++)
    {
      // key found
      if (key == keys[i])
      {
        // call this function
        return isVorbisComment(i);
      }
    }

    // key not found
    return false;
  }

}

