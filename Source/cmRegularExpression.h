/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// Original Copyright notice:
// Copyright (C) 1991 Texas Instruments Incorporated.
//
// Permission is granted to any individual or institution to use, copy, modify,
// and distribute this software, provided that this complete copyright and
// permission notice is maintained, intact, in all copies and supporting
// documentation.
//
// Texas Instruments Incorporated provides this software "as is" without
// express or implied warranty.
//
// Created: MNF 06/13/89  Initial Design and Implementation
// Updated: LGO 08/09/89  Inherit from Generic
// Updated: MBN 09/07/89  Added conditional exception handling
// Updated: MBN 12/15/89  Sprinkled "const" qualifiers all over the place!
// Updated: DLS 03/22/91  New lite version
//

#ifndef cmRegularExpression_h
#define cmRegularExpression_h

#include "cmStandardIncludes.h"

const int NSUBEXP = 10;

/** \class cmRegularExpression
 * \brief Implements pattern matching with regular expressions.
 *
 * This is the header file for the regular expression class.  An object of
 * this class contains a regular expression, in a special "compiled" format.
 * This compiled format consists of several slots all kept as the objects
 * private data.  The cmRegularExpression class provides a convenient way to
 * represent regular expressions.  It makes it easy to search for the same
 * regular expression in many different strings without having to compile a
 * string to regular expression format more than necessary.
 *
 * This class implements pattern matching via regular expressions.
 * A regular expression allows a programmer to specify  complex
 * patterns  that  can  be searched for and matched against the
 * character string of a string object. In its simplest form, a
 * regular  expression  is  a  sequence  of  characters used to
 * search for exact character matches. However, many times  the
 * exact  sequence to be found is not known, or only a match at
 * the beginning or end of a string is desired. The cmRegularExpression regu-
 * lar  expression  class implements regular expression pattern
 * matching as is found and implemented in many  UNIX  commands
 * and utilities.
 *
 * Example: The perl code
 * 
 *    $filename =~ m"([a-z]+)\.cc";
 *    print $1;
 *    
 * Is written as follows in C++
 *
 *    cmRegularExpression re("([a-z]+)\\.cc");
 *    re.find(filename);
 *    cerr << re.match(1);
 *
 *
 * The regular expression class provides a convenient mechanism
 * for  specifying  and  manipulating  regular expressions. The
 * regular expression object allows specification of such  pat-
 * terns  by using the following regular expression metacharac-
 * ters:
 *
 *  ^        Matches at beginning of a line
 *
 *  $        Matches at end of a line
 *
 * .         Matches any single character
 *
 * [ ]       Matches any character(s) inside the brackets
 *
 * [^ ]      Matches any character(s) not inside the brackets
 *
 *  -        Matches any character in range on either side of a dash
 *
 *  *        Matches preceding pattern zero or more times
 *
 *  +        Matches preceding pattern one or more times
 *
 *  ?        Matches preceding pattern zero or once only
 *
 * ()        Saves a matched expression and uses it in a  later match
 *
 * Note that more than one of these metacharacters can be  used
 * in  a  single  regular expression in order to create complex
 * search patterns. For example, the pattern [^ab1-9]  says  to
 * match  any  character  sequence that does not begin with the
 * characters "ab"  followed  by  numbers  in  the  series  one
 * through nine.
 *
 * There are three constructors for cmRegularExpression.  One just creates an
 * empty cmRegularExpression object.  Another creates a cmRegularExpression
 * object and initializes it with a regular expression that is given in the
 * form of a char*.  The third takes a reference to a cmRegularExpression
 * object as an argument and creates an object initialized with the
 * information from the given cmRegularExpression object.
 *
 * The  find  member function  finds   the  first  occurence   of  the regualr
 * expression of that object in the string given to find as an argument.  Find
 * returns a boolean, and  if true,  mutates  the private  data appropriately.
 * Find sets pointers to the beginning and end of  the thing last  found, they
 * are pointers into the actual string  that was searched.   The start and end
 * member functions return indicies  into the searched string that  correspond
 * to the beginning   and  end pointers  respectively.   The    compile member
 * function takes a char* and puts the  compiled version of the char* argument
 * into the object's private data fields.  The == and  != operators only check
 * the  to see  if   the compiled  regular  expression   is the same, and  the
 * deep_equal functions also checks  to see if the  start and end pointers are
 * the same.  The is_valid  function returns false if  program is set to NULL,
 * (i.e. there is no valid compiled exression).  The set_invalid function sets
 * the  program to NULL  (Warning: this deletes the compiled  expression). The
 * following examples may help clarify regular expression usage:
 *
 *   *  The regular expression  "^hello" matches  a "hello"  only at  the
 *      beginning of a  line.  It would match "hello  there" but not "hi,
 *      hello there".
 *
 *   *  The regular expression "long$" matches a  "long"  only at the end
 *      of a line. It would match "so long\0", but not "long ago".
 *
 *   *  The regular expression "t..t..g"  will match anything that  has a
 *      "t" then any two characters, another "t", any  two characters and
 *      then a "g".   It will match  "testing", or "test again" but would
 *      not match "toasting"
 *
 *   *  The regular  expression "[1-9ab]" matches any  number one through
 *      nine, and the characters  "a" and  "b".  It would match "hello 1"
 *      or "begin", but would not match "no-match".
 *
 *   *  The  regular expression "[^1-9ab]"  matches any character that is
 *      not a number one  through nine, or  an "a" or "b".   It would NOT
 *      match "hello 1" or "begin", but would match "no-match".
 *
 *   *  The regular expression "br* " matches  something that begins with
 *      a "b", is followed by zero or more "r"s, and ends in a space.  It
 *      would match "brrrrr ", and "b ", but would not match "brrh ".
 *
 *   *  The regular expression "br+ " matches something  that begins with
 *      a "b", is followed by one or more "r"s, and ends in  a space.  It
 *      would match "brrrrr ",  and  "br ", but would not  match "b  " or
 *      "brrh ".
 *
 *   *  The regular expression "br? " matches  something that begins with
 *      a "b", is followed by zero or one "r"s, and ends in  a space.  It
 *      would  match  "br ", and "b  ", but would not match  "brrrr "  or
 *      "brrh ".
 *
 *   *  The regular expression "(..p)b" matches  something ending with pb
 *      and beginning with whatever the two characters before the first p
 *      encounterd in the line were.  It would find  "repb" in "rep drepa
 *      qrepb".  The regular expression "(..p)a"  would find "repa qrepb"
 *      in "rep drepa qrepb"
 *
 *   *  The regular expression "d(..p)" matches something ending  with p,
 *      beginning with d, and having  two characters  in between that are
 *      the same as the two characters before  the first p  encounterd in
 *      the line.  It would match "drepa qrepb" in "rep drepa qrepb".
 *
 */
class cmRegularExpression 
{
public:
  /**
   * Instantiate cmRegularExpression with program=NULL.
   */
  inline cmRegularExpression ();	

  /**
   * Instantiate cmRegularExpression with compiled char*.
   */
  inline cmRegularExpression (char const*);
  
  /**
   * Instantiate cmRegularExpression as a copy of another regular expression.
   */
  cmRegularExpression (cmRegularExpression const&);

  /**
   * Destructor.
   */
  inline ~cmRegularExpression();

  /**
   * Compile a regular expression into internal code
   * for later pattern matching.
   */
  void compile (char const*);

  /**
   * Matches the regular expression to the given string.
   * Returns true if found, and sets start and end indexes accordingly.
   */
  bool find (char const*);

  /**
   * Matches the regular expression to the given std string.
   * Returns true if found, and sets start and end indexes accordingly.
   */
  bool find (std::string const&);		

  /**
   * Index to start of first find.
   */
  inline std::string::size_type start() const;

  /**
   * Index to end of first find.
   */
  inline std::string::size_type end() const;

  /**
   * Returns true if two regular expressions have the same
   * compiled program for pattern matching.
   */
  bool operator== (cmRegularExpression const&) const;

  /**
   * Returns true if two regular expressions have different
   * compiled program for pattern matching.
   */
  inline bool operator!= (cmRegularExpression const&) const;

  /**
   * Returns true if have the same compiled regular expressions
   * and the same start and end pointers.
   */
  bool deep_equal (cmRegularExpression const&) const;
  
  /**
   * True if the compiled regexp is valid.
   */
  inline bool is_valid() const;

  /**
   * Marks the regular expression as invalid.
   */
  inline void set_invalid();		

  /**
   * Destructor.
   */
  // awf added
  std::string::size_type start(int n) const;
  std::string::size_type end(int n) const;
  std::string match(int n) const;
  
private: 
  const char* startp[NSUBEXP];
  const char* endp[NSUBEXP];
  char  regstart;			// Internal use only
  char  reganch;			// Internal use only
  const char* regmust;			// Internal use only
  int   regmlen;			// Internal use only
  char* program;   
  int   progsize;
  const char* searchstring;
}; 

/**
 * Create an empty regular expression.
 */
inline cmRegularExpression::cmRegularExpression () 
{ 
  this->program = NULL;
}

/**
 * Creates a regular expression from string s, and
 * compiles s.
 */
inline cmRegularExpression::cmRegularExpression (const char* s) 
{  
  this->program = NULL;
  compile(s);
}

/**
 * Destroys and frees space allocated for the regular expression.
 */
inline cmRegularExpression::~cmRegularExpression () 
{
//#ifndef WIN32
  delete [] this->program;
//#endif
}

/**
 * Set the start position for the regular expression.
 */
inline std::string::size_type cmRegularExpression::start () const 
{
  return(this->startp[0] - searchstring);
}


/**
 * Returns the start/end index of the last item found.
 */
inline std::string::size_type cmRegularExpression::end () const 
{
  return(this->endp[0] - searchstring);
}

/**
 * Returns true if two regular expressions have different
 * compiled program for pattern matching.
 */
inline bool cmRegularExpression::operator!= (const cmRegularExpression& r) const 
{
  return(!(*this == r));
}

/**
 * Returns true if a valid regular expression is compiled
 * and ready for pattern matching.
 */
inline bool cmRegularExpression::is_valid () const 
{
  return (this->program != NULL);
}


inline void cmRegularExpression::set_invalid () 
{
//#ifndef WIN32
  delete [] this->program;
//#endif
  this->program = NULL;
}

/**
 * Return start index of nth submatch. start(0) is the start of the full match.
 */
inline std::string::size_type cmRegularExpression::start(int n) const
{
  return this->startp[n] - searchstring;
}


/**
 * Return end index of nth submatch. end(0) is the end of the full match.
 */
inline std::string::size_type cmRegularExpression::end(int n) const
{
  return this->endp[n] - searchstring;
}

/**
 * Return nth submatch as a string.
 */
inline std::string cmRegularExpression::match(int n) const
{
  return std::string(this->startp[n], this->endp[n] - this->startp[n]);
}

#endif // cmRegularExpressionh
