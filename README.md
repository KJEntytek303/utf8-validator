Single header UTF-8 string validation library.

define `KJE_UTF8_VALIDATION_IMPL` where you want the function definition to be, then include the header file.

# Macros
`KJEUtf8GetUTFByteLength(x)` - Return the byte length of the provided character. `x` must be first byte of the character, otherwise the behavior is undefined.
`KJEUtf8IsValidTrail(x)` - Return if the byte is in the format of `0b10xx xxxx`.
`KJEUtf8IsNotSurrogate(x)` - Return if the byte is not a UTF-16 surrogate

# Functions

`signed int kjeUtf8Validate( const unsigned char* restrict const string )`

### Description:
Function takes in a C-string as input and validates if it's valid UTF-8 string.

### Argument list:
`string` - A null terminated C-string to validate.

### return values:
Zero represents a success. A non-zero value represents failure to validate the string.
`0` - Success
`-1` (`KJE_UTF8_ERR_MALFORMED_UTF`) - string is malformed in some way.
`-2` (`KJE_UTF8_ERR_INVALID_FIRST_BYTE`) - First byte is invalid.
`-3` (`KJE_UTF8_ERR_UTF_RANGE_OVERFLOW`) - Overflow of Unicode range (codepoint > `0x1F FF FF`)
`-4` (`KJE_UTF8_ERR_OVERLONG`) - Overlong encoding detected in the string.
`-5` (`KJE_UTF8_ERR_SURROGATE`) - UTF-16 surrogate code point detected.
