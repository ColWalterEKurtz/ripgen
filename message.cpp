// -----------------------------------------------------------------------------
// message.cpp                                                       message.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file defines all members of the @ref msg namespace.
 * @author     Col. Walter E. Kurtz
 * @version    2016-11-22
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <sstream>
#include <iostream>
#include "message.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Definitions                                                       Definitions
// -----------------------------------------------------------------------------
namespace msg
{

  // ---
  // nfo
  // ---
  /*
   * format: blue
   */
  void nfo(const string& message)
  {
    // show info
    cerr << "\033[34m" << "[INFO]" << "\033[0m" << " " << message << endl;
  }

  // ---
  // wrn
  // ---
  /*
   * format: yellow
   */
  void wrn(const string& message)
  {
    // show warning
    cerr << "\033[33m" << "[WARN]" << "\033[0m" << " " << message << endl;
  }

  // ---
  // err
  // ---
  /*
   * format: red
   */
  void err(const string& message)
  {
    // show error
    cerr << "\033[31m" << "[FAIL]" << "\033[0m" << " " << message << endl;
  }

  // ---
  // cat
  // ---
  /*
   * s1 + s2
   */
  string cat(const string& s1, const string& s2)
  {
    stringstream concat;
    concat << s1;
    concat << s2;

    return concat.str();
  }

  // ----
  // catq
  // ----
  /*
   * s1 + "s2"
   */
  string catq(const string& s1, const string& s2)
  {
    stringstream concat;
    concat << s1;
    concat << "\"";
    concat << s2;
    concat << "\"";

    return concat.str();
  }

  // ----
  // qcat
  // ----
  /*
   * "s1" + s2
   */
  string qcat(const string& s1, const string& s2)
  {
    stringstream concat;
    concat << "\"";
    concat << s1;
    concat << "\"";
    concat << s2;

    return concat.str();
  }

  // ---
  // ins
  // ---
  /*
   * s1a + s2 + s1b
   */
  string ins(const string& s1, const string& s2)
  {
    stringstream merge;

    // check all characters in s1
    for(string::size_type i = 0; i < s1.size(); i++)
    {
      // tilde found
      if (s1[i] == '~')
      {
        merge << s2;
      }

      // 'normal' character found
      else
      {
        merge << s1[i];
      }
    }

    return merge.str();
  }

  // ----
  // insq
  // ----
  /*
   * s1a + "s2" + s1b
   */
  string insq(const string& s1, const string& s2)
  {
    stringstream merge;

    // check all characters in s1
    for(string::size_type i = 0; i < s1.size(); i++)
    {
      // tilde found
      if (s1[i] == '~')
      {
        merge << "\"";
        merge << s2;
        merge << "\"";
      }

      // 'normal' character found
      else
      {
        merge << s1[i];
      }
    }

    return merge.str();
  }

  // ---
  // str
  // ---
  /*
   *
   */
  string str(int num)
  {
    stringstream convert;
    convert << num;

    return convert.str();
  }

  // ---
  // str
  // ---
  /*
   *
   */
  string str(unsigned int num)
  {
    stringstream convert;
    convert << num;

    return convert.str();
  }

  // ---
  // str
  // ---
  /*
   *
   */
  string str(double num)
  {
    stringstream convert;
    convert << num;

    return convert.str();
  }

}
