###**NAME**
+ ftrest - **client**
+ ftrestd - **server**

###**SYNOPSIS**

+ **ftrest** COMMAND REMOTE-PATH \[LOCAL-PATH\]
+ **ftrestd** \[-r ROOT-FOLDER\] \[-p PORT\]

###**DESCRIPTION**

###SERVER
**ftrestd** creates a server on the port defined by **PORT**, otherwise port 6677 is considered with root folder defined by **ROOT-FOLDER**. If the option is not present, current folder is considered. 

#####Command line examples
1. **ftrestd** **-r home/james/foo/bar/ -p 1234**
Creates a server with **ROOT-FOLDER** home/james/foo/bar/ and port 1234
2. **ftrestd** **-p 1234**
Creates a server with current folder as a **ROOT-FOLDER**  and port 1234
3. **ftrestd -r home/richard/foo/bar/foo**
Creates a server with  current folder as a **ROOT-FOLDER** and default port 6677

###CLIENT
**ftrest**  sends request on the server on address **REMOTE-PATH**
requesting action defined by **COMMAND**. **LOCAL-PATH** may  only be used with commands **get** and **put** described in commands further in the documentation.  **LOCAL-PATH** is a path in local file system. If the option is not present with the command **get** current path is considered. If the option is not present with the command **put** error occurs, see return values.

####CLIENT COMMANDS

**del** - deletes a file pointed to by **REMOTE-PATH** on the server
**get** - copies a file pointed to by  **REMOTE-PATH** to local folder or local file system pointed to by **LOCAL-PATH**.
**put** - copies a file pointed to by **LOCAL-PATH** to folder 					  pointed to by  **REMOTE-PATH**. **REMOTE-PATH** can be folder and file with the same name as a file to be copied. If it is a folder file is copied to the folder with same name as in the **LOCAL-PATH**.
**lst** - prints content of the **REMOTE-PATH** to STDOUT. Format is similar to ls command.
**mkd** - creates a folder pointed to by  **REMOTE-PATH** on the server
**rmd** - deletes a folder pointed to by  **REMOTE-PATH** on the server

#####Command line examples

1. **ftrest mkd  ht<span>tp://localh</span>ost:12345/bar2**
 Creates the folder bar2 on the server pointed to by  **REMOTE-PATH**.
2.  **ftrest rmd  ht<span>tp://localh</span>ost:12345/bar2** 
Removes the folder bar2 on the server pointed to by  **REMOTE-PATH**.
3.  **ftrest get ht<span>tp://localh</span>ost:12345/bar2/file.txt** **/local/path**
Copies the file file.txt pointed to by  **REMOTE-PATH** to **LOCAL-PATH** folder.
4.  **ftrest del ht<span>tp://localh</span>ost:12345/bar2/file.txt**
Removes the file  file.txt on the server pointed to by  **REMOTE-PATH**.
5.  **ftrest** **put ht**<span>**tp://loca**</span>**lhost:12345/tonda/foo/bar/** 
**~/doc.txt**
Copies the file doc.txt specified by the **LOCAL-PATH** to the folder specified by the **REMOTE-PATH**.

###**RETURN VALUES**

On success returns 0;

####ERROR RETURN VALUES
On error returns EXIT_FAILURE with a specified message to STDERR.

+ **"Not a directory."** when **REMOTE-PATH** points to the file, but the **lst, rmd** commands are used
+  **"Directory not found."** when **REMOTE-PATH** does not point to an existing object when **lst, rmd** commands are used.
+  **"Directory not empty."** when **REMOTE-PATH** points to the folder, which is not empty and **rmdir** command is used.
+  **"Already exists."** when **REMOTE-PATH** points to the folder/file, which already exists and **mkd** or **put** commands are used.
+ **"Not a file."** when **REMOTE-PATH** points on the folder, but **del, get** commands are used.
+ **"File not found."** when **REMOTE-PATH** doesn not point to any existing object when **del, get** commands are used.
+  **"Unknown error."** for all other errors.

###**LIMITATIONS**

When using **put** or **get** command only files that can be opened by fopen can be copied.

###**BUGS**

When using **lst** command. It sometimes prints badly formatted output.

####**COPYRIGHT**
Created by Tomáš Holík, March 2017 as a student project for Computer Communications and Networks in school VUT Faculty of information and technology Brno.

This  is free software.  There is NO warranty. Not
even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

###**Makefile**
**make all** 
to make ftrest and ftrestd from source files.
**make clean**
to clean *.o and ftrest and ftrestd