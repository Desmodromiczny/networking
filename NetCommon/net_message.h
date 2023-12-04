#pragma once

#include "net_common.h"
#include <vector>
#include <iostream>
#include <type_traits>  // Added for static_assert
#include <cstring>      // Added for std::memcpy


namespace olc
{
	namespace net
	{
		template <typename T>
		struct message_header {
			T id;
			uint32_t size = 0;
		};

		template <typename T>
		struct message {
			message_header<T> header; //\added
			std::vector<uint8_t> body;

			size_t size() const {
				return sizeof(message_header<T>) + body.size();
			}

			friend std::ostrem& operator << (std::ostream& os, const message<T>& msg)
			{
				os << "ID:" << int(msg.header.id) << " Size: " << msg.header.size;
				return os;
			}


			// Pushes any POD-like data into message buffer
			template<typename DataType>
			friend message<T>& operator << (message<T>& msg, const DataType& data)
			{
				//check that the type of the data being pushed is trivialy copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

				// Cache current size of vector, as this will be the point we inster the data
				size_t i = msg.body.size();

				// Resize the vector by the size of the data being pushed
				msg.body.resize(msg.body.size() + sizeof(DataType));
				std::memcpy(msg.body.data() + i, &data, sizeof(DataType)); //added

				// Recalculate the target message so it can be "chained"
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}


			template<typename DataType>
			friend message<T>& operator >> (message<T>& msg, DataType& data) {
				//check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be ";
				
				// Cache the location towards the end of the vector where the pulled data starts
				size_t i = msg.body.size() - sizeof(DataType);

				// Physically copy the data from the vector into the user variable
				std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

				// Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(i);
			}
		};
	}
}
