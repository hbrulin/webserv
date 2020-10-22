#ifndef BUFFERS_HPP
# define BUFFERS_HPP

#include <string>
#include "../utils/definitions.hpp"
#include "../utils/libft.h"

class Buffers {
	
	public:
		std::string headers;
		std::string body;
		char *m_buffer;
		int		m_id;
		long int			m_content_length;
		long int			track_length;
		bool				body_parse_chunk;
		bool				body_parse_length;
		unsigned int 		header_length;

		Buffers(int id);
		void clean_buf();
		virtual ~Buffers() {
			free(m_buffer);
		}
};

#endif