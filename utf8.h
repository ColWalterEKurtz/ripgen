// -----------------------------------------------------------------------------
// utf8.h                                                                 utf8.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file declares 'public' members of the @ref utf8 namespace.
 * @author     Col. Walter E. Kurtz
 * @version    2016-10-30
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef UTF8_H_INCLUDE_NO1
#define UTF8_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <vector>
#include <string>


// ----
// utf8
// ----
/**
 * @brief  The @a utf8 namespace provides some functions that
 *         operate utf8 encoded strings.
 */
namespace utf8
{

  // ------------
  // utf82unicode
  // ------------
  /**
   * @param[in]  utf8     utf8 encoded string
   * @param[out] unicode  vector of unicode numbers
   *
   * @return
   */
  bool utf82unicode(const std::string& utf8, std::vector<unsigned>& unicode);

  // ------------
  // unicode2utf8
  // ------------
  /**
   * @param unicode number
   *
   * @return  an utf8 encoded character
   */
  std::string unicode2utf8(unsigned unicode);

  // ------------
  // getCodePoint
  // ------------
  /**
   * @param unicode
   *
   * @return
   */
  std::string getCodePoint(unsigned unicode);

  // -----------
  // getUTF8Size
  // -----------
  /**
   * @param utf8
   *
   * @return
   */
  unsigned getUTF8Size(const std::string& utf8);

  // ------------
  // isWhitespace
  // ------------
  /**
   * @param unicode
   *
   * @return
   */
  bool isWhitespace(unsigned unicode);

  // ----------------
  // isLatinGraphical
  // ----------------
  /**
   * @param unicode
   *
   * @return
   */
  bool isLatinGraphical(unsigned unicode);

}

#endif  /* #ifndef UTF8_H_INCLUDE_NO1 */

