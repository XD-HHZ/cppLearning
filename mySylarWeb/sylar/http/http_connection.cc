#include "http_connection.h"
#include "http_parser.h"

namespace sylar {
namespace http {

HttpConnection::HttpConnection(Socket::ptr sock, bool owner)
	:SocketStream(sock, owner) {
}

HttpResponse::ptr HttpConnection::recvResponse() {
	HttpResponseParser::ptr parser(new HttpResponseParser);
	uint64_t buff_size = HttpRequestParser::GetHttpRequestBufferSize();
	//uint64_t buff_size = 150;
	std::shared_ptr<char> buffer(
			new char[buff_size + 1], [](char* ptr){
				delete[] ptr;
			});
	char* data = buffer.get();
	int offset = 0;
	do {
		int len = read(data + offset, buff_size - offset);
		if(len <= 0) {
			close();
			return nullptr;
		}
		len += offset;
		data[len] = '\0';
		size_t nparse = parser->execute(data, len, true);
		if(parser->hasError()) {
			close();
			return nullptr;
		}
		offset = len - nparse;
		if(offset == (int)buff_size) {
			close();
			return nullptr;
		}
		if(parser->isFinished()) {
			break;
		}
	} while(true);
	auto client_parser = parser->getParser();
	if(client_parser.chunked) {
		std::string body;
		int len = offset;
		do {
			do {
				int rt = read(data + len, buff_size - len);
				if(rt <= 0) {
					close();
					return nullptr;
				}
				len += rt;
				data[len] = '\0';
				size_t nparse = parser->execute(data, len, true);
				if(parser->hasError()) {
					close();
					return nullptr;
				}
				len -= nparse;
				if(len == (int)buff_size) {
					close();
					return nullptr;
				}
			} while(!parser->isFinished());
			len -= 2;

			if(client_parser.content_len <= len) {
				body.append(data, client_parser.content_len);
				memmove(data, data + client_parser.content_len
						, len - client_parser.content_len);
				len -= client_parser.content_len;
			} else {
				body.append(data, len);
				int left = client_parser.content_len - len;
				while(left > 0) {
					int rt = read(data, left > (int)buff_size ? (int)buff_size : left);
					if(rt <= 0) {
						close();
						return nullptr;
					}
					body.append(data, rt);
					left -= rt;
				}
				len = 0;
			}
		} while(!client_parser.chunks_done);
		parser->getData()->setBody(body);

	} else {
		int64_t length = parser->getContentLength();
		if(length > 0) {
			std::string body;
			body.resize(length);

			int len = 0;
			if(length >= offset) {
				memcpy(&body[0], data, offset);
				len = offset;
			} else {
				memcpy(&body[0], data, length);
				len = length;
			}
			length -= offset;
			if(length > 0) {
				if(readFixSize(&body[len], length) <= 0) {
					close();
					return nullptr;
				}
			}
			parser->getData()->setBody(body);
		}
	}
	return parser->getData();
}

int HttpConnection::sendRequest(HttpRequest::ptr rsp) {
	std::stringstream ss;
	ss << *rsp;
	std::string data = ss.str();
	writeFixSize(data.c_str(), data.size());
}

}
}
