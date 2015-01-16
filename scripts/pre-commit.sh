#!/usr/bin/env sh

#**
#* @file    scripts/pre-commit.sh
#* @brief   Pre commition script
#* @version 0.1
#* @date    29.12.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

version=`gnuindent --version 2>/dev/null`
if test "x$version" = "x"; then
	version=`indent --version 2>/dev/null`
	if test "x$version" = "x"; then
		echo "GStreamer git pre-commit hook:"
		echo "Did not find GNU indent, please install it before continuing."
		exit 1
	fi
	INDENT=indent
else
	INDENT=gnuindent
fi

case `$INDENT --version` in
	GNU*)
	;;
	default)
		echo "git pre-commit hook:"
		echo "Did not find GNU indent, please install it before continuing."
		echo "(Found $INDENT, but it doesn't seem to be GNU indent)"
		exit 1
	;;
esac

INDENT_PARAMETERS="--braces-on-if-line \
	--case-brace-indentation0 \
	--case-indentation2 \
	--braces-after-struct-decl-line \
	--line-length80 \
	--no-tabs \
	--cuddle-else \
	--dont-line-up-parentheses \
	--continuation-indentation4 \
	--honour-newlines \
	--tab-size8 \
	--indent-level2 \
	--leave-preprocessor-space"



