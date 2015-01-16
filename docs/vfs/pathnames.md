# Resource pathnames

The *flos* operating system uses specific pathname sytaxes for addressing resources in local and external data bases. Mainly it follows UNIX style resource pathname formats. But it has many extensions.

# Virtual file system pathname aka POSIX-style pathname

VFS path is base pathname sytax for accessing resource located in local and mounted data bases. It follows POSIX-style path format with delimiter `/` (slash).

It should follow pathname format defined in POSIX standard [4.11][POSIX 4.11]:
> Each filename in the pathname is located in the directory specified by its predecessor (for example, in the pathname fragment **a/b**, file b is located in directory **a**).
> [..]
>If the pathname begins with a slash, the predecessor of the first filename in the pathname shall be taken to be the root directory of the process (such pathnames are referred to as "absolute pathnames").
> 
> If the pathname does not begin with a slash, the predecessor of the first filename of the pathname shall be taken to be the current working directory of the process (such pathnames are referred to as "relative pathnames").
> [..]
> The special filename dot shall refer to the directory specified by its predecessor. The special filename dot-dot shall refer to the parent directory of its predecessor directory. As a special case, in the root directory, dot-dot may refer to the root directory itself.
>
>A pathname consisting of a single slash shall resolve to the root directory of the process. A null pathname shall not be successfully resolved. A pathname that begins with two successive slashes may be interpreted in an implementation-defined manner, although more than two leading slashes shall be treated as a single slash.

If source contains symbolic constance `_POSIX_VERSION` then pathname should follow solely this pathname syntax and "VFS pathname resolution" flag must be added when opening files.
If error occurs during VFS pathname resolution, error string and number is set up for specific POSIX error.

## URI type path names

URI pathnames are extension to VFS pathnames to support more understandable resource addressing. URI pathname syntax are not obligated to fully follow URI standard.

URI pathnames syntax is as follows:
```
<scheme name> : <hierarchical part> [ ? <query> ] [ # <fragment> ]
```
where

 - **scheme name** consists of a sequence of characters beginning with a letter and followed by any combination of letters, digits, plus
   ("+"), period ("."), or hyphen ("-").
 - **hierarchical part** of the URI is intended to hold identification information hierarchical in nature. If this part begins with a double forward slash ("//"), it is followed by an authority part and a path.   If the hierarchical part doesn't begin with ("//") it contains only a path.
    - The **authority part** holds an optional user-information part, terminated with "@" (e.g. username:password@); a hostname (e.g., domain name or IP address); and an optional port number, preceded by a colon ":".
   - The **path part**, if present, may optionally begin with a single forward slash ("/"). It may not begin with two slash characters ("//"). The path is a sequence of segments (conceptually similar to directories, though not necessarily representing them) separated by a forward slash ("/"). Historically, each segment was specified to contain parameters separated from it using a semicolon (";"), though this was rarely used in practice and current specifications allow but no longer specify such semantics.
 - The **query** is an optional part, separated by a question mark ("?"), that contains additional identification information that is not hierarchical in nature. The syntax of the query string is not well defined; however by convention it is most often a sequence of <key>=<value> pairs separated by a semicolon (";") or an ampersand ("&").
 - The **fragment** is an optional part separated from the front parts by a hash ("#"). It holds additional identifying information that provides direction to a secondary resource, e.g., a section heading (in an article) identified by the remainder of the URI. When the primary resource is an HTML document, the fragment is often an *id* attribute of a specific element and web browsers will make sure this element is visible.

The *flos* system should interpret URI pathnames as function calls to certain programs or servers. The *flos* system does not handler percent encoding in URIs.

If vfs `/bin` directory contains executable named as scheme, core should run that execute with pipeline to calling executable.
For example if pathname is `git://github.com/user/project-name.git` and `/bin/git` exists then git should handle URI and provide file or file tree for accessing, browsing git repository.

If otherwise access to git repositories is provided by file system server's extension then core should fall back to default file system server for handling certain URI address. If VFS path name exists for URI pathname (as relative VFS pathname) then VFS file is used. Although if "URI pathname resolution" flag is specified in opening then pathname if handled solely as URI and "Incorrect URI syntax" error is returned when needed.
If file handler or executable cannot handle URI address, it should send back error "File or filesystem not found".

Example 1.:
```C
int f = open("ftp://example.com/dir/file.ext", "r");

if(errstr) {
	return;
}
/* Operations with f */
```

Example 2.:
```C
int f = open("udp://127.0.0.1:456");
char *msg = "<message></message>";

write(f, msg, strsz(msg)); 
```


[POSIX 4.11]: http://pubs.opengroup.org/onlinepubs/009695399/basedefs/xbd_chap04.html#tag_04_11



