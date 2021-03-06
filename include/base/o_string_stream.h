// $Id: o_string_stream.h,v 1.1.1.1 2008/03/19 06:41:21 gdiso Exp $

#ifndef __o_string_stream_h__
#define __o_string_stream_h__



#include "config.h"


#ifdef HAVE_SSTREAM

// C++ includes
#include <sstream>

// Local Includes
#include "genius_common.h"


// Forward Declarations



/*
 * Some compilers, at least HP \p aCC do not even
 * accept empty classes derived from \p std::ostringstream.
 * Therefore, resort to preprocessor definitions.
 */

#ifndef BROKEN_IOSTREAM
 /*
  * ---------------------------------------------------------------------------------
  * Everything for a clean iostream
  */

# include <iomanip>

 /*
  * Outputs \p std::string \p d with width
  * \p v left-formatted to stream \p o.
  */
# define OSSStringleft(o,v,d)       (o).width(v);  (o) << std::left << (d)

 /*
  * Outputs \p std::string \p d with width
  * \p v right-formatted to stream \p o.
  */
# define OSSStringright(o,v,d)      (o).width(v);  (o) << std::right << (d)

 /*
  * Outputs \p Real \p d with width \p v and
  * precision \p p to stream \p o (padded with
  * whitespaces).
  */
# define OSSRealleft(o,v,p,d)       (o).width(v);  (o).precision(p); (o).fill(' '); (o) << (d)

 /*
  * Outputs \p Real \p d with width \p v and
  * precision \p p to stream \p o (padded with
  * zeros).
  */
# define OSSRealzeroleft(o,v,p,d)   (o).width(v); (o).precision(p); (o).fill('0'); (o) << std::showpoint << std::left << (d)

 /*
  * Outputs \p Real \p d with width \p v and
  * precision \p p to stream \p o (padded with
  * whitespaces).
  */
# define OSSRealright(o,v,p,d)       (o).width(v);  (o).precision(p); (o).fill(' '); (o) << std::right << (d)

 /*
  * Outputs \p Real \p d with width \p v and
  * precision \p p to stream \p o (padded with
  * zeros).
  */
# define OSSRealzeroright(o,v,p,d)   (o).width(v);  (o).precision(p); (o).fill('0'); (o) << std::right << (d)

 /*
  * Outputs \p Real \p d with width \p v
  * in scientific format to stream \p o.
  */
# define OSSRealscientific(o,v,d)   (o) << std::setw(v) << std::scientific << (d)

 /*
  * Outputs \p int \p d with width
  * \p v to stream \p o.
  */
# define OSSInt(o,v,d)              (o).width(v);  (o) << (d)

 /*
  * class alias
  */
# define OStringStream           std::ostringstream





#else
 /*
  * ---------------------------------------------------------------------------------
  * Everything for broken iostream
  */

# include <cstdio>

 /*
  * Outputs \p std::string \p d with width
  * \p v left-formatted to stream \p o.
  */
# define OSSStringleft(o,v,d)       (o).left( (v), (d) )

 /*
  * Outputs \p std::string \p d with width
  * \p v right-formatted to stream \p o.
  */
# define OSSStringright(o,v,d)      (o).right( (v), (d) )

 /*
  * Outputs \p Real \p d with width \p v and
  * precision \p p to stream \p o (padded with
  * whitespaces).
  */
# define OSSRealleft(o,v,p,d)       (o).left( (v), (p), (d) , ' ')

 /*
  * Outputs \p Real \p d with width \p v and
  * precision \p p to stream \p o (padded with
  * zeros).
  */
# define OSSRealzeroleft(o,v,p,d)       (o).left( (v), (p), (d) , '0')

 /*
  * Outputs \p Real \p d with width \p v and
  * precision \p p to stream \p o (padded with
  * whitespaces).
  */
# define OSSRealright(o,v,p,d)      (o).right( (v), (p), (d) ,' ')

 /*
  * Outputs \p Real \p d with width \p v and
  * precision \p p to stream \p o (padded with
  * zeros).
  */
# define OSSRealzeroright(o,v,p,d)      (o).right( (v), (p), (d) ,'0')

 /*
  * Outputs \p Real \p d with width \p v
  * in scientific format to stream \p o.
  */
# define OSSRealscientific(o,v,d)   (o).scientific( (v), (d) )

 /*
  * Outputs \p int \p d with width
  * \p v to stream \p o.
  */
# define OSSInt(o,v,d)              (o).left( (v), (d) )

//  /*
//   * class alias
//   */
// # define OSSOStringStream           OStringStream



 /**
  * This class provides a compatibility class for broken
  * features in the \p ostringstream of the older \p GCC
  * versions.  Other compilers do not see this class.
  */

 // ------------------------------------------------------------
 // OStringStream class definition
 class OStringStream : public std::ostringstream
 {
 public:

  /**
   * Default constructor.
   */
  OStringStream () : std::ostringstream() {};

  /**
   * Default destructor.
   */
  ~OStringStream () {};

  /**
   * convenient typedef
   */
  typedef std::string::size_type sizetype;

  /**
   * Outputs in a \p OStringStream, where \p s
   * was directed in ragged left style with
   * size \p w.
   */
  void left (const sizetype w,
	     const std::string& s);

  /**
   * Outputs in a \p OStringStream, where \p r
   * was directed in ragged left style with
   * size \p w and precision \p prec.
   */
  void left (const sizetype w,
	     const sizetype prec,
	     const Real r,
	     const char c = ' ');

  /**
   * Outputs in a \p OStringStream, where \p r
   * was directed in ragged left style with
   * size \p w, padded with chars \p c, defaults
   * to whitespace.
   */
  void left (const sizetype w,
	     const int n);

  /**
   * Outputs in a \p OStringStream, where \p s
   * was directed in ragged right style with
   * size \p w.
   */
  void right (const sizetype w,
	      const std::string& s);


  /**
   * Outputs in a \p OStringStream, where \p r
   * was directed in ragged right style with
   * size \p w and precision \p prec.  The leading
   * characters are \p c, defaults to whitespaces.
   */
  void right (const sizetype w,
	      const sizetype prec,
	      const Real r,
	      const char c = ' ');

  /**
   * Outputs in a \p OStringStream, where \p r
   * was directed in scientific style with
   * size \p w.
   */
  void scientific (const sizetype w,
		   const Real r);

 protected:
  /**
   * Appends \p n chars (defaults to whitespaces)
   * to the string \p s.
   */
  void print_ws (const sizetype n,
		 const char c = ' ');

 };



 // ------------------------------------------------------------
 // OStringStream inline methods
 inline
 void OStringStream::left (const sizetype w,
			   const std::string& s)
 {
   *this << s;
   // pad with whitespaces afterwards
   print_ws ((w-s.size()));
 }


 inline
 void OStringStream::left (const sizetype w,
			   const sizetype prec,
			   const Real r,
			   const char c)
 {
   assert (w < 30);
   char buf[30];
   char format[8];
   // form the format for r
   // ALTERNATIVE: sprintf (format, "%%%d.%df", int((w-prec)/2), prec);
   sprintf (format, "%%.%df", prec);
   // form string as desired
   sprintf (buf, format, r);
   *this << buf;
   // pad with whitespaces afterwards
   print_ws (w-std::string(buf).size(), c);
   // ALTERNATIVE: print_ws (w-int((w-prec)/2));
 }


 inline
 void OStringStream::left (const sizetype w,
			   const int n)
 {
   assert (w < 30);
   char buf[30];
   // form string as desired
   sprintf (buf, "%d", n);
   *this << buf;
   // pad with whitespaces afterwards
   print_ws (w-std::string(buf).size());
 }


 inline
 void OStringStream::right (const sizetype w,
			    const std::string& s)
 {
   // first pad with whitespaces
   print_ws ((w-s.size()));
   *this << s;
 }


 inline
 void OStringStream::right (const sizetype w,
			    const sizetype prec,
			    const Real r,
			    const char c)
 {
   assert (w < 30);
   char buf[30];
   char format[8];
   // form the format for r
   sprintf (format, "%%.%df", prec);
   // form string as desired
   sprintf (buf, format, r);
   // first pad with the user-defined char
   print_ws (w-std::string(buf).size(), c);
   // then print the float
   *this << buf;
 }


 inline
 void OStringStream::scientific (const sizetype w,
				 const Real r)
 {
   assert (w < 30);
   char buf[30];
   char format[8];
   // form the format for r
   sprintf (format, "%%%de", w);
   // form string as desired
   sprintf (buf, format, r);
   *this << buf;
 }


 inline
 void OStringStream::print_ws (const sizetype n,
			       const char c)
 {
   for (sizetype i = 0; i < n; i++)
     *this << c;
 }


#endif // ifndef ... else ... BROKEN_IOSTREAM




#endif // ifdef HAVE_SSTREAM

#endif // ifndef __o_string_stream_h__

