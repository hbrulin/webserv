# webserv

Nonblocking fds : https://medium.com/@copyconstruct/nonblocking-i-o-99948ad7c957
 When a descriptor is set in nonblocking mode, an I/O system call on that descriptor will return immediately, even if that request can’t be immediately completed (and will therefore result in the process being blocked otherwise). The return value can be either of the following:
— an error: when the operation cannot be completed at all
— a partial count: when the input or output operation can be partially completed
— the entire result: when the I/O operation could be fully completed

-> That's why we use select() before read/recv or write/send.
select() works by blocking until something happens on a file descriptor (aka a socket).

Server with select() and non-blocking fds: 
- https://www.lowtek.com/sockets/select.html
- https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm 