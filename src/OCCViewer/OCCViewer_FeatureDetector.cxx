// Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com

// File   : OCCViewer_FeatureDetector.cxx
// Author : Renaud NEDELEC, Open CASCADE S.A.S.

#include "OCCViewer_FeatureDetector.h"
#include <stdio.h>
#include "utilities.h"

using namespace cv;

/*!
  Constructor
  \param theFilename - image to process
*/
OCCViewer_FeatureDetector::OCCViewer_FeatureDetector(const QString& theFilename): 
  corners()
{
  cornerCount = 2000;
  rect=cvRect(0,0,0,0);
  imagePath = theFilename.toStdString();
  // Store the dimensions of the picture
  IplImage* bg_img = cvLoadImage (imagePath.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
  imgHeight = bg_img->height;
  imgWidth  = bg_img->width; 
}

/*!
  Computes the corners of the image located at imagePath
*/
void OCCViewer_FeatureDetector::ComputeCorners(){
  
  // Parameters for the corner detection
  double qualityLevel = 0.2;
  double minDistance = 1;
 
  // Images to be used for detection
  IplImage *eig_img, *temp_img, *src_img_gray;
  
  // Load image
  src_img_gray = cvLoadImage (imagePath.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
  
  if ( rect.width > 1 )
  {
    // If a ROI as been set use it for detection
    cvSetImageROI( src_img_gray, rect );
  }
  
  eig_img = cvCreateImage (cvGetSize (src_img_gray), IPL_DEPTH_32F, 1);
  temp_img = cvCreateImage (cvGetSize (src_img_gray), IPL_DEPTH_32F, 1);
  corners = (CvPoint2D32f *) cvAlloc (cornerCount * sizeof (CvPoint2D32f));
  
  // image height and width
  imgHeight = src_img_gray->height;
  imgWidth  = src_img_gray->width;

  // Corner detection using cvCornerMinEigenVal 
  // (one of the methods available inOpenCV, there is also a cvConerHarris method that can be used by setting a flag in cvGoodFeaturesToTrack)
  cvGoodFeaturesToTrack (src_img_gray, eig_img, temp_img, corners, &cornerCount, /*quality-level=*/qualityLevel, /*min-distance=*/minDistance);
  cvFindCornerSubPix (src_img_gray, corners, cornerCount,
                    cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));

  cvReleaseImage (&eig_img);
  cvReleaseImage (&temp_img);
  cvReleaseImage (&src_img_gray);

}

/*!
  Computes the contours of the image located at imagePath
*/
bool OCCViewer_FeatureDetector::ComputeContours(){
 
  // Initialising images
  Mat src, src_gray;
  Mat detected_edges;

  // Thresholds for Canny detector
  int lowThreshold = 100;
  int ratio = 3;
  int kernel_size = 3; // 3,5 or 7
  
  bool isRectSelected = (rect.width > 1);
  if (isRectSelected)
    src = _colorFiltering();
  else
  {
    src = imread( imagePath.c_str() );
  }

  if( !src.data )
  { return false; }

  // Convert the image to grayscale
  if (src.channels() == 3)
    cvtColor( src, src_gray, CV_BGR2GRAY );
  else if (src.channels() == 1)
    src_gray = src;
  
  if(isRectSelected)
  {
    _detectAndRetrieveContours( src_gray );
  }
  else
  {
    // Reduce noise with a kernel 3x3               
    blur( src_gray, detected_edges, Size(3,3) );

    // Canny detector
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size, /*L2gradient =*/true );    // The problem is that with that filter the detector detects double contours
    
    // Retrieve contours on the Canny result
    _detectAndRetrieveContours( detected_edges );
  }
  
  return true;
  
}

/*!
  Stores a region of interest given by user in rect
  \param theRect - Region Of Interest of the image located at imagePath 
*/
void OCCViewer_FeatureDetector::SetROI( const QRect& theRect )
{
  if (!theRect.isEmpty()){
    rect = cvRect(theRect.x(),theRect.y(),theRect.width(),theRect.height());
  }
}

/*!
  Performs contours detection and store them in contours 
  \param src - src image to find contours of 
*/
void OCCViewer_FeatureDetector::_detectAndRetrieveContours( Mat src )
{
  src = src > 1; 
  int method ;//= CV_CHAIN_APPROX_SIMPLE;
//   if ( rect.width > 1 )
  method = CV_CHAIN_APPROX_NONE;//CV_CHAIN_APPROX_TC89_KCOS;//CV_CHAIN_APPROX_TC89_L1;
  findContours( src, contours, hierarchy,CV_RETR_CCOMP, method);
  // Other possible approximations CV_CHAIN_APPROX_TC89_L1, CV_CHAIN_APPROX_SIMPLE cf. OpenCV documentation 
  // for precise information
}

/*!
  Performs color filtering from the image sample contained in the ROI rect of the image 
  located at imagePath
  Thresholds the result in order ot obtain a binary image
  \return binary image resulting from filtering and thersholding
*/
Mat OCCViewer_FeatureDetector::_colorFiltering()
{  
  IplImage* find_image = cvLoadImage(imagePath.c_str(),CV_LOAD_IMAGE_COLOR);
  if ( !rect.width > 1 )
    return Mat(find_image);
  
  // Crop the image to build an histogram from the selected part
  cvSetImageROI(find_image, rect);
  IplImage* test_image = cvCreateImage(cvGetSize(find_image),
                                      find_image->depth,
                                      find_image->nChannels);
  cvCopy(find_image, test_image, NULL);
  cvResetImageROI(find_image);
  
  IplImage* test_hsv = cvCreateImage(cvGetSize(test_image),8,3);
  IplImage* test_hue = cvCreateImage(cvGetSize(test_image),8,1);

  CvHistogram* hist;

  cvCvtColor(test_image, test_hsv, CV_BGR2HSV);
  cvCvtPixToPlane(test_hsv, test_hue, 0, 0, 0);
  
  //create hist
  int size_hist = 10;
  float hranges[] = {0, 180};
  float* ranges = hranges;
  hist = cvCreateHist(1, &size_hist, CV_HIST_ARRAY, &ranges, 1);
  
  //calculate hue` histogram
  cvCalcHist(&test_hue, hist, 0 ,0);

  //calculate back projection of hue plane of input image
  IplImage* backproject = cvCreateImage(cvGetSize(find_image), 8, 1);
  IplImage* binary_backproject = cvCreateImage(cvGetSize(find_image), 8, 1);
  IplImage* find_hsv = cvCreateImage(cvGetSize(find_image),8,3);
  IplImage* find_hue = cvCreateImage(cvGetSize(find_image),8,1);
  
  cvCvtColor(find_image, find_hsv, CV_BGR2HSV);
  cvCvtPixToPlane(find_hsv, find_hue, 0, 0, 0);
  cvCalcBackProject(&find_hue, backproject, hist);
  
  // Threshold in order to obtain binary image
  cvThreshold(backproject, binary_backproject, 1, 255, CV_THRESH_BINARY);  // NOTE it would be good to think about the best threshold to use (1 here)
  cvReleaseImage(&test_image);
  cvReleaseImage(&test_hsv);
  cvReleaseImage(&test_hue);
  cvReleaseImage(&backproject);
  
  return Mat(binary_backproject);
}
