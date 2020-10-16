#ifndef RETURNCODES_HPP
# define RETURNCODES_HPP

#include <map>
#include <utility>
#include <string>

static std::pair<int, std::string> codes[] = {
		std::make_pair(100, " Continue\r\n"),
		std::make_pair(101, " Switching Protocols\r\n"),
		std::make_pair(102, " Processing\r\n"),
		std::make_pair(103, " Early hints\r\n"),

		std::make_pair(200, " OK\r\n"),
		std::make_pair(201, " Created\r\n"),
		std::make_pair(202, " Accepted\r\n"),
		std::make_pair(203, " Non-Authoritative Information\r\n"),
		std::make_pair(204, " No Content\r\n"),
		std::make_pair(205, " Reset Content\r\n"),
		std::make_pair(206, " Partial Content\r\n"),
		std::make_pair(207, " Multi-Status\r\n"),
		std::make_pair(208, " Already Reported\r\n"),
		std::make_pair(210, " Content Different\r\n"),
		std::make_pair(226, " IM Used\r\n"),

		std::make_pair(300, " Multiple Choices\r\n"),
		std::make_pair(301, " Moved permanently\r\n"),
		std::make_pair(302, " Found\r\n"),
		std::make_pair(303, " See Other\r\n"),
		std::make_pair(304, " Not Modified\r\n"),
		std::make_pair(305, " Use Proxy\r\n"),
		std::make_pair(306, " Switch Proxy\r\n"),
		std::make_pair(307, " Temporary Redirect\r\n"),
		std::make_pair(308, " Permanent Redirect\r\n"),
		std::make_pair(310, " Too Many Redirects\r\n"),

		std::make_pair(400, " Bad Request\r\n"),
		std::make_pair(401, " Unauthorized\r\n"),
		std::make_pair(402, " Payment Required\r\n"),
		std::make_pair(403, " Forbidden\r\n"),
		std::make_pair(404, " Not Found\r\n"),
		std::make_pair(405, " Method Not Allowed\r\n"),
		std::make_pair(406, " Not Acceptable\r\n"),
		std::make_pair(407, " Proxy Authentication Required\r\n"),
		std::make_pair(408, " Request Time-Out\r\n"),
		std::make_pair(409, " Conflict\r\n"),
		std::make_pair(410, " Gone\r\n"),
		std::make_pair(411, " Length Required\r\n"),
		std::make_pair(412, " Precondition Failed\r\n"),
		std::make_pair(413, " Request Entity Too Large\r\n"),
		std::make_pair(414, " Request-URI Too Long\r\n"),
		std::make_pair(415, " Unsupported Media Type\r\n"),
		std::make_pair(416, " Requested range unsatisfiable\r\n"),
		std::make_pair(417, " Expectation failed\r\n"),
		std::make_pair(418, " I'm a teapot\r\n"),
		std::make_pair(421, " Bad mapping\r\n"),
		std::make_pair(422, " Unprocessable entity\r\n"),
		std::make_pair(423, " Locked\r\n"),
		std::make_pair(424, " Method failure\r\n"),
		std::make_pair(425, " Unordered Collection\r\n"),
		std::make_pair(426, " Upgrade Required\r\n"),
		std::make_pair(428, " Precondition Required\r\n"),
		std::make_pair(429, " Too Many Requests\r\n"),
		std::make_pair(431, " Request Header Fileds Too Large\r\n"),
		std::make_pair(449, " Retry With\r\n"),
		std::make_pair(450, " Blocked by Windows Parental Control\r\n"),
		std::make_pair(451, " Unavailable for Legal Reasons\r\n"),
		std::make_pair(456, " Unrecoverable Error\r\n"),
		
		std::make_pair(444, " No Response\r\n"),
		std::make_pair(495, " SSL Certificate Error\r\n"),
		std::make_pair(496, " SSL Certificate Required\r\n"),
		std::make_pair(497, " HTTP Request Sent to HTTPS Port\r\n"),
		std::make_pair(498, " Token Expired/Invalid\r\n"),
		std::make_pair(499, " Client Closed Request\r\n"),

		std::make_pair(500, " Internal Server Error\r\n"),
		std::make_pair(501, " Not Implemented\r\n"),
		std::make_pair(502, " Bad Gateway\r\n"),
		std::make_pair(503, " Service Unavailable\r\n"),
		std::make_pair(504, " Gateway Time-Out\r\n"),
		std::make_pair(505, " HTTP Version non supported\r\n"),
		std::make_pair(506, " Variant Also Negotiates\r\n"),
		std::make_pair(507, " Insufficient storage\r\n"),
		std::make_pair(508, " Loop Detected\r\n"),
		std::make_pair(509, " Bandwidth Limit Exceeded\r\n"),
		std::make_pair(510, " Not extended\r\n"),
		std::make_pair(511, " Network authentication required\r\n"),

		std::make_pair(520, " Unknown Error\r\n"),
		std::make_pair(521, " Web Server Is Down\r\n"),
		std::make_pair(522, " Connection Timed Out\r\n"),
		std::make_pair(523, " Origin is Unreachable\r\n"),
		std::make_pair(524, " A Timeout Occurred\r\n"),
		std::make_pair(525, " SSL Handshake Failed\r\n"),
		std::make_pair(526, " Invalid SSL Certificate\r\n"),
		std::make_pair(527, " Railgun Error\r\n")

};

static std::map<int, std::string > codeTab(codes, codes + 83);

#endif