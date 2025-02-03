PHP_ARG_ENABLE(commandutils, whether to enable commandutils extension,
[  --enable-commandutils    Enable commandutils extension], no)

if test "$PHP_COMMANDUTILS" != "no"; then
    AC_DEFINE(HAVE_COMMANDUTILS, 1, [Have commandutils extension])
    PHP_NEW_EXTENSION(commandutils, src/ext-commandutils.c, $ext_shared)
fi
