#
# Check configure option --disable-HDF=[yes|no] taking into account that
# DISABLE_HDF may be already set during build_configure,
# Set DISABLE_HDF to yes|no
#
# Author : Sergey LITONIN (OCC, 2012)
#

AC_DEFUN([CHECK_HDF],[

AC_ARG_WITH(HDF,
	    [  --disable-HDF default=no ],
	    disable_HDF="$withval",disable_HDF="${DISABLE_HDF}")

case $disable_HDF in
  yes)
#         AC_MSG_RESULT(************************************************)
#         AC_MSG_RESULT(*****   DISABLE HDF configuration   ******)
#         AC_MSG_RESULT(************************************************)
        DISABLE_HDF="yes"
        AC_SUBST(DISABLE_HDF)
esac

 
])dnl
