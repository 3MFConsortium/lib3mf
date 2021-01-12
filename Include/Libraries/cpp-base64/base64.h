//
//  base64 encoding and decoding with C++.
//  Version: 1.01.00
//

#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>
#include <vector>

std::string base64_encode(std::vector<unsigned char> const & buffer);
std::vector<unsigned char> base64_decode(std::string const& s);

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
