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

echo "Pre-commit hook: formating source files"

version=`gnuindent --version 2>/dev/null`
if test "x$version" = "x"; then
	version=`indent --version 2>/dev/null`
	if test "x$version" = "x"; then
		echo "git pre-commit hook:"
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

INDENT_PARAMETERS="\
        --braces-after-struct-decl-line \
        --braces-on-func-def-line \
        --braces-on-if-line \
        --break-after-boolean-operator \
        --case-brace-indentation0 \
        --case-indentation4 \
        --comment-delimiters-on-blank-lines \
        --comment-indentation40 \
        --continuation-indentation4 \
        --continue-at-parentheses \
        --cuddle-else \
        --dont-break-procedure-type \
        --dont-format-comments \
        --honour-newlines \
        --indent-level4 \
        --leave-optional-blank-lines \
        --leave-preprocessor-space \
        --line-length80 \
        --no-parameter-indentation \
        --no-space-after-casts \
        --no-space-after-for \
        --no-space-after-function-call-names \
        --no-space-after-if \
        --no-space-after-parentheses \
        --no-space-after-while \
        --no-tabs \
        --preprocessor-indentation4 \
        --start-left-side-of-comments \
        --tab-size4 \
        "

__indent_C() {
        if test ! -f $1
        then
            return;
        fi
 
        "$INDENT" $INDENT_PARAMETERS "$1"
        git add "$1"
}

find . -not -path './gcc-cc/*' -not -path './.*' | while read file;
do 
    ext=$(expr "$file" : ".*\(\..*\)")
    case $ext in
    .c|.h)
        __indent_C $file
    ;;
    esac
done
