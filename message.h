// -----------------------------------------------------------------------------
// message.h                                                           message.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file declares 'public' members of the @ref msg namespace.
 * @author     Col. Walter E. Kurtz
 * @version    2016-10-30
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef MESSAGE_H_INCLUDE_NO1
#define MESSAGE_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>


// ---
// msg
// ---
/**
 * @brief  The @a msg namespace provides some functions that
 *         print (tagged) messages via stderr.
 *
 * The functions found here can be grouped as follows:
 * * Printing
 *   - msg::nfo()
 *   - msg::wrn()
 *   - msg::err()
 * * Concatenation
 *   - msg::cat()
 *   - msg::catq()
 *   - msg::qcat()
 *   - msg::ins()
 *   - msg::insq()
 * * Type conversion
 *   - msg::str( int )
 *   - msg::str( unsigned int )
 *   - msg::str( double )
 */
namespace msg
{

  // ---
  // nfo
  // ---
  /**
   * @brief  This function prints a tagged @a info message via stderr.
   *
   * @par Example
   *      @code{.cpp}
   *        msg::nfo("Hello, world!");  // [INFO] Hello, world!
   *      @endcode
   *
   * @param message  holds the line to print.
   *
   * @see  msg::wrn()
   * @see  msg::err()
   */
  void nfo(const std::string& message);

  // ---
  // wrn
  // ---
  /**
   * @brief  This function prints a tagged @a warning message via stderr.
   *
   * @par Example
   *      @code{.cpp}
   *        msg::wrn("Hello, world!");  // [WARNING] Hello, world!
   *      @endcode
   *
   * @param message  holds the line to print.
   *
   * @see  msg::nfo()
   * @see  msg::err()
   */
  void wrn(const std::string& message);

  // ---
  // err
  // ---
  /**
   * @brief  This function prints a tagged @a error message via stderr.
   *
   * @par Example
   *      @code{.cpp}
   *        msg::err("Hello, world!");  // [ERROR] Hello, world!
   *      @endcode
   *
   * @param message  holds the line to print.
   *
   * @see  msg::nfo()
   * @see  msg::wrn()
   */
  void err(const std::string& message);

  // ---
  // cat
  // ---
  /**
   * @brief  This function concatenates the two passed arguments.
   *
   * @par Example
   *      @code{.cpp}
   *        msg::cat("Hello, ", "world!");  // Hello, world!
   *      @endcode
   *
   * @param s1  holds the first part of the resulting string.
   * @param s2  holds the second part of the resulting string.
   *
   * @return  s1 + s2
   *
   * @see  msg::catq()
   * @see  msg::qcat()
   */
  std::string cat(const std::string& s1, const std::string& s2);

  // ----
  // catq
  // ----
  /**
   * @brief  This function concatenates the two passed arguments
   *         and puts the second argument in quotation marks.
   *
   * @par Example
   *      @code{.cpp}
   *        msg::catq("Hello, ", "world!");  // Hello, "world!"
   *      @endcode
   *
   * @param s1  holds the first part of the resulting string.
   * @param s2  holds the second part of the resulting string.
   *
   * @return  s1 + "s2"
   *
   * @see  msg::cat()
   * @see  msg::qcat()
   */
  std::string catq(const std::string& s1, const std::string& s2);

  // ----
  // qcat
  // ----
  /**
   * @brief  This function concatenates the two passed arguments
   *         and puts the first argument in quotation marks.
   *
   * @par Example
   *      @code{.cpp}
   *        msg::qcat("Hello", ", world!");  // "Hello", world!
   *      @endcode
   *
   * @param s1  holds the first part of the resulting string.
   * @param s2  holds the second part of the resulting string.
   *
   * @return  "s1" + s2
   *
   * @see  msg::cat()
   * @see  msg::catq()
   */
  std::string qcat(const std::string& s1, const std::string& s2);

  // ---
  // ins
  // ---
  /**
   * @brief  This function replaces each ~ (tilde) character in the
   *         first argument with the second one.
   *
   * @par Example
   *      @code{.cpp}
   *        msg::ins("Hello, ~!", "world");  // Hello, world!
   *      @endcode
   *
   * @param s1  defines the 'template' string.
   * @param s2  holds the string to insert.
   *
   * @return  s1a + s2 + s1b
   *
   * @see  msg::insq()
   */
  std::string ins(const std::string& s1, const std::string& s2);

  // ----
  // insq
  // ----
  /**
   * @brief  This function replaces each ~ (tilde) character in the
   *         first argument with the second one quoted.
   *
   * @par Example
   *      @code{.cpp}
   *        msg::insq("Hello, ~!", "world");  // Hello, "world"!
   *      @endcode
   *
   * @param s1  defines the 'template' string.
   * @param s2  holds the string to insert.
   *
   * @return  s1a + "s2" + s1b
   *
   * @see  msg::ins()
   */
  std::string insq(const std::string& s1, const std::string& s2);

  // ---
  // str
  // ---
  /**
   * @brief  This function converts its argument to string.
   *
   * @param num  is the number to convert.
   *
   * @return  a string of decimal digits representing the passed number
   *
   * @see  msg::str( unsigned int )
   * @see  msg::str( double )
   */
  std::string str(int num);

  // ---
  // str
  // ---
  /**
   * @brief  This function converts its argument to string.
   *
   * @param num  is the number to convert.
   *
   * @return  a string of decimal digits representing the passed number
   *
   * @see  msg::str( int )
   * @see  msg::str( double )
   */
  std::string str(unsigned int num);

  // ---
  // str
  // ---
  /**
   * @brief  This function converts its argument to string.
   *
   * @param num  is the number to convert.
   *
   * @return  a string of decimal digits representing the passed number
   *
   * @see  msg::str( int )
   * @see  msg::str( unsigned int )
   */
  std::string str(double num);

}

#endif  /* #ifndef MESSAGE_H_INCLUDE_NO1 */
