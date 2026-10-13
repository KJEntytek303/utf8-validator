#ifndef KJE_UTF8_VALIDATION_LIBRARY
#define KJE_UTF8_VALIDATION_LIBRARY

#include <stdint.h>
//Constants
constexpr int KJE_UTF8_SUCCESS = 0;
constexpr int KJE_UTF8_ERR_MALFORMED_UTF = -1;
constexpr int KJE_UTF8_ERR_INVALID_FIRST_BYTE = -2;
constexpr int KJE_UTF8_ERR_UTF_RANGE_OVERFLOW = -3;
constexpr int KJE_UTF8_ERR_OVERLONG = -4;
constexpr int KJE_UTF8_ERR_SURROGATE = -5;


constexpr uint8_t KJE_UTF8_LENGTH_FROM_1ST_BYTE[] = {
	//0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	//0xEE - surrogates
	  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 3, 4
};

//Macros
#define KJEUtf8GetUTFByteLength( x ) KJE_UTF8_LENGTH_FROM_1ST_BYTE[ ( (x) & 0xFF) >> 4]

#define KJEUtf8IsValidTrail( x ) ( ((x) & 0xC0) == 0x80 )

#define KJEUtf8IsNotSurrogate( x ) ( (x) & 0x20 != 0 )

//Functions
extern signed int kjeUtf8Validate(const unsigned char* restrict const string);


#endif //KJE_UTF8_VALIDATION_LIBRARY


//Implementation BEGIN
#ifdef KJE_UTF8_VALIDATION_IMPL
#undef KJE_UTF8_VALIDATION_IMPL


signed int kjeUtf8Validate(const unsigned char* restrict const string) {

	int overlong_check_case = 0;
	int char_length;
	int bytes_to_read = 0;
	for( int i=0; buffer[i] != '\0'; bytes_to_read--, i++) {/* {{{ */

		//First byte
		if(bytes_to_read == 0) {/* {{{ */

			bytes_to_read = KJEUtf8GetUTFByteLength( buffer[i] );

			//Invalid
			if(bytes_to_read == 0) {
				return KJE_UTF8_ERR_INVALID_FIRST_BYTE;
			}

			char_length = bytes_to_read;

			overlong_check_case = 0;

			//Poking at bits to see if there's potential for overlong
			switch( char_length ) {
				case 2:
					if( (buffer[i] & 0x1E) == 0 ) {
						return KJE_UTF8_ERR_OVERLONG;
					}
					continue;

				case 3:
					if( (buffer[i] & 0x0F ) == 0) {
						overlong_check_case = 3;
					}
					continue;

				case 4:
					//Additional check for Unicode overflow ( x > 0x1FFFFF )
					if( buffer[i] != 0xF0 ) {
						return KJE_UTF8_ERR_MALFORMED_UTF;
					}
					overlong_check_case = 4;
					continue;
			}

			continue;
		}/* }}} */

		//check if the byte starts with 0b10xx xxxx
		if( ! KJEUtf8IsValidTrail( buffer[i] ) ) {
			return KJE_UTF8_ERR_MALFORMED_UTF;
		}

		//Examining 2nd byte will answer our question.
		switch (overlong_check_case) {
			case 3:
				if( (buffer[i] & 0x20) == 0 ) {
					return KJE_UTF8_ERR_OVERLONG;
				}
				overlong_check_case = 0;
				break;

			case 4:
				if( (buffer[i] & 0x30) == 0 ) {
					return KJE_UTF8_ERR_OVERLONG;
				}
				overlong_check_case = 0;
				break;
		}

		//Surrogates
		if( char_length == 3 && bytes_to_read == 1 && !KJEUtf8IsNotSurrogate(buffer[i])) {
			return KJE_UTF8_ERR_SURROGATE;
		}

	}/* }}} */

	//Null inside a character
	if(bytes_to_read != 0) {
		return KJE_UTF8_ERR_MALFORMED_UTF;
	}

	return KJE_UTF8_SUCCESS;
}


#endif //KJE_UTF8_VALIDATION_IMPL
