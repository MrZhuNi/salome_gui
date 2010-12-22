#
# Check configure option --disable-graphicsView=[yes|no|DIR] taking into account that
# DISABLE_GRAPHICSVIEW may be already set during build_configure,
# Set DISABLE_GRAPHICSVIEW to yes|no
#
# Author : Kirill Afonshin (DTN, 2010)
#

AC_DEFUN([CHECK_GRAPHICSVIEW],[

AC_ARG_WITH(graphicsView,
	    [  --disable-graphicsView default=no ],
	    disable_graphicsView="$withval",disable_graphicsView="${DISABLE_GRAPHICSVIEW}")

case $disable_graphicsView in
  yes)
#         AC_MSG_RESULT(************************************************)
#         AC_MSG_RESULT(*****  DISABLE GraphicsView configuration  *****)
#         AC_MSG_RESULT(************************************************)
        DISABLE_GRAPHICSVIEW="yes"
        AC_SUBST(DISABLE_GRAPHICSVIEW)
esac

 
])dnl
