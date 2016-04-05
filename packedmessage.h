#ifndef PACKEDMESSAGE_H
#define PACKEDMESSAGE_H
#include <memory>

using data_buffer = std::vector<char>;
const int HEADER_SIZE = 4;

template <typename T>
class PackedMessage
{
public:
	using MessagePointer = std::shared_ptr<T>;

	PackedMessage(MessagePointer msg = MessagePointer()) : m_msg(msg)
	{
	}

	void set_msg(MessagePointer msg)
	{
		m_msg = msg;
	}

	MessagePointer get_msg()
	{
		return m_msg;
	}

	bool pack(data_buffer& buf)
	{
		if (!m_msg)
			return false;
		auto msg_size = m_msg->ByteSize();
		buf.resize(HEADER_SIZE + msg_size);
		encode_header(buf, msg_size);
		return m_msg->SerializeToArray(&buf[HEADER_SIZE], msg_size);
	}

	bool unpack(const data_buffer& buf)
	{
		return m_msg->ParseFromArray(&buf[HEADER_SIZE], buf.size() - HEADER_SIZE);
	}

	static void encode_header(data_buffer& buf, unsigned size)
	{
		assert(buf.size() >= HEADER_SIZE);
		buf[0] = static_cast<char>(size >> 24 & 0xFF);
		buf[1] = static_cast<char>(size >> 16 & 0xFF);
		buf[2] = static_cast<char>(size >> 8 & 0xFF);
		buf[3] = static_cast<char>(size & 0xFF);
	}

	static unsigned decode_header(const data_buffer& buf) 
	{
		if (buf.size() < HEADER_SIZE)
			return 0;
		unsigned size = 0;
		for (auto i = 0; i < HEADER_SIZE; ++i)
			size = size * 256 + static_cast<unsigned>(buf[i]) & 0xff;
		return size;
	}


private:
	MessagePointer m_msg;
};

#endif 

