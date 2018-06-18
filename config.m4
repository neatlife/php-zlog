dnl $Id$
dnl config.m4 for extension zlog

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(zlog, for zlog support,
dnl Make sure that the comment is aligned:
dnl [  --with-zlog             Include zlog support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(zlog, whether to enable zlog support,
dnl Make sure that the comment is aligned:
[  --enable-zlog           Enable zlog support])

if test "$PHP_ZLOG" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-zlog -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/zlog.h"  # you most likely want to change this
  dnl if test -r $PHP_ZLOG/$SEARCH_FOR; then # path given as parameter
  dnl   ZLOG_DIR=$PHP_ZLOG
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for zlog files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ZLOG_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ZLOG_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the zlog distribution])
  dnl fi

  dnl # --with-zlog -> add include path
  dnl PHP_ADD_INCLUDE($ZLOG_DIR/include)
  PHP_ADD_INCLUDE("zlog/src")

  dnl # --with-zlog -> check for lib and symbol presence
  dnl LIBNAME=zlog # you may want to change this
  dnl LIBSYMBOL=zlog # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ZLOG_DIR/$PHP_LIBDIR, ZLOG_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ZLOGLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong zlog lib version or lib not found])
  dnl ],[
  dnl   -L$ZLOG_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ZLOG_SHARED_LIBADD)

  PHP_NEW_EXTENSION(zlog, zlog/src/*.c zlog.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
