#include "Buffers.hpp"

Buffers::Buffers(int id): m_id(id), track_length(0), body_parse_chunk(0), body_parse_length(0), header_length(0) {
	m_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	memset((void *)m_buffer, 0, BUFFER_SIZE + 1);
	headers = "";
	body = "";
}

void Buffers::clean_buf() {
	m_id = 0;
	track_length = 0;
	body_parse_chunk = 0;
	body_parse_length = 0;
	header_length = 0;
	memset((void *)m_buffer, 0, BUFFER_SIZE + 1);
	headers = "";
	body = "";
}
