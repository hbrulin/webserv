# webserv

Nonblocking fds : https://medium.com/@copyconstruct/nonblocking-i-o-99948ad7c957
 When a descriptor is set in nonblocking mode, an I/O system call on that descriptor will return immediately, even if that request can’t be immediately completed (and will therefore result in the process being blocked otherwise). The return value can be either of the following:
— an error: when the operation cannot be completed at all
— a partial count: when the input or output operation can be partially completed
— the entire result: when the I/O operation could be fully completed

By setting a socket to non-blocking, you can effectively “poll” the socket for information. If you try to read from a non-blocking socket and there’s no data there, it’s not allowed to block—it will return -1 and errno will be set to EAGAIN or EWOULDBLOCK.

-> That's why we use select() before read/recv or write/send.
select() works by blocking until something happens on a file descriptor (aka a socket).

Server with select() and non-blocking fds: 
- https://www.lowtek.com/sockets/select.html
- https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm 
- https://github.com/codehoose/barebones-webserver

HTTP 1.1
https://www.w3.org/Protocols/rfc2616/rfc2616.html

Install nginx : https://www.javatpoint.com/installing-nginx-on-mac

CGI :
http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm

General guide : https://beej.us/guide/bgnet/html/

Setup www-auth : https://www.digitalocean.com/community/tutorials/how-to-set-up-password-authentication-with-nginx-on-ubuntu-14-04 

Tester use : 
sh testeur/test.sh
telnet localhost 8080
telnet localhost 8080 < monfichier
sudo nginx -s stop