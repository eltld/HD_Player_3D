
#ifndef INCLUDE_YUV_FILE_PARAMS
#define INCLUDE_YUV_FILE_PARAMS

#include <string>

// ----------------------------------------------------------------------
//
// CLASS:	YUVFileParams
//
// PURPOSE:	This class is designed to hold the parameters required to
//		read a YUV video file.  Such parameters include things
//		like the frame height and width, but they also include
//		things like the buffer offsets required to properly
//		read an image into a PicBuffer object as the JSVM code
//		seems to expect.
//
// MODIFIED:	Tue Mar 14, 2006 (EM) Created initial version.
//
// ----------------------------------------------------------------------


struct YUVFileParams {
  YUVFileParams():_height(0), _width(0) {}

  //YUVFileParams(const std::string& fileName, 
  //		const int height, const int width);
  YUVFileParams(const std::string& fileName, const int view_id, 
		const int height, const int width);
  YUVFileParams( const YUVFileParams& other); //copy constructor

  std::string     _fileName;  // name of file to read from
  unsigned int	  _view_id; // view-id used in SPS	
  unsigned int    _height;    // height of an image frame
  unsigned int    _width;     // width of an image frame
  unsigned int    _lumaSize;  // number of pixels in image frame include
                              // padding for margins
  unsigned int    _bufSize;   // total size of buffer for luma and chroma
  unsigned int    _lumaOffset;// point in buffer for first luma pixel
  unsigned int    _cbOffset;  // point in buffer for first Cb pixel
  unsigned int    _crOffset;  // point in buffer for first Cr pixel
  unsigned int    _stride;    // how far to move in buffer to get from luma
                              // pixel at position (x,y) to (x,y+1)
};

#endif
