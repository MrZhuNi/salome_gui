// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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
//

#ifndef GRAPHICSVIEW_PRSIMAGE_H
#define GRAPHICSVIEW_PRSIMAGE_H

#include "GraphicsView.h"

#include "GraphicsView_Object.h"

class GraphicsView_PrsImageFrame;

/*
  Class       : GraphicsView_PrsImage
  Description : Presentation for image object
*/
class GRAPHICSVIEW_API GraphicsView_PrsImage : public GraphicsView_Object
{
public:
  GraphicsView_PrsImage();
  virtual ~GraphicsView_PrsImage();

public:
  void                            updateTransform();

  void                            setImage( const QImage& theImage );
  QImage                          getImage() const;

  QTransform                      getTransform( const bool theIsPreview = false ) const;

  void                            setPosition( const double thePosX, const double thePosY );
  void                            getPosition( double& thePosX, double& thePosY ) const;

  void                            setScaling( const double theScaleX, const double theScaleY );
  void                            getScaling( double& theScaleX, double& theScaleY ) const;

  void                            setRotationAngle( const double theRotationAngle );
  void                            getRotationAngle( double& theRotationAngle ) const;

  void                            setIsLockAspectRatio( const bool theIsLockAspectRatio );
  bool                            getIsLockAspectRatio() const;

  void                            setIsSmoothTransformation( const bool theIsSmoothTransformation );
  bool                            getIsSmoothTransformation() const;

public:
  // from QGraphicsItem
  virtual QRectF                  boundingRect() const;

  // from GraphicsView_Object
  virtual void                    compute();

  virtual void                    addTo( GraphicsView_ViewPort* theViewPort );
  virtual void                    removeFrom( GraphicsView_ViewPort* theViewPort );

  virtual bool                    checkHighlight( double theX, double theY, QCursor& theCursor ) const;

  virtual bool                    select( double theX, double theY, const QRectF& theRect );
  virtual void                    unselect();
  virtual void                    setSelected( bool theState );

  virtual void                    move( double theDX, double theDY, bool theIsAtOnce = false );
  virtual bool                    finishMove( bool theStatus );

protected:
  void                            processResize( const int theAnchor,
                                                 const double theDX,
                                                 const double theDY );
  void                            finishResize();

  double                          computeRotationAngle( const QPointF& thePoint1,
                                                        const QPointF& thePoint2 ) const;

  void                            processRotate( const double theAngle );
  void                            finishRotate();

protected:
  void                            enablePreview( const bool theState );

  static double                   computeAngle( const QVector2D& theVector1,
                                                const QVector2D& theVector2 );

protected:
  QPixmap                         myPixmap;

  bool                            myIsLockAspectRatio;

  QGraphicsPixmapItem*            myPixmapItem;
  QGraphicsPixmapItem*            myPreviewPixmapItem;

  GraphicsView_PrsImageFrame*     myPrsImageFrame;

  double                          myPosX;
  double                          myPosY;
  double                          myScaleX;
  double                          myScaleY;
  double                          myRotationAngle;

  double                          myPreviewPosX;
  double                          myPreviewPosY;
  double                          myPreviewScaleX;
  double                          myPreviewScaleY;
  double                          myPreviewRotationAngle;

private:
  friend class GraphicsView_PrsImageFrame;
};

#endif
