// -----------------------------------------------------------------------------
// TestHandler.cpp                                               TestHandler.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref TestHandler class.
 * @author     Col. Walter E. Kurtz
 * @version    2016-06-20
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <iostream>
#include "TestHandler.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// -----------
// TestHandler
// -----------
/*
 *
 */
TestHandler::TestHandler()
{
  // initialization
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
void TestHandler::OnBeginParsing(const string& filename)
{
  cout << "OnBeginParsing(\"" << filename << "\")" << endl;
}

// ------------
// OnEndParsing
// ------------
/*
 *
 */
void TestHandler::OnEndParsing(bool healthy)
{
  cout << "OnEndParsing(" << (healthy ? "true" : "false") << ")" << endl;
}

// ------
// OnData
// ------
/*
 *
 */
void TestHandler::OnData(const string& key, const string& value)
{
  cout << "OnData(\"" << key << "\", \"" << value << "\")" << endl;
}

