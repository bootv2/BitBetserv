/* MD5
converted to C++ class by Frank Thilo (thilo@unix-ag.org)
for bzflag (http://www.bzflag.org)

based on:

md5.h and md5.c
reference implementation of RFC 1321

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

#ifndef BZF_MD5_H
#define BZF_MD5_H

#include <cstring>
#include <iostream>


// a small class for calculating MD5 hashes of strings or byte arrays
// it is not meant to be fast or secure
//
// usage: 1) feed it blocks of uchars with update()
//      2) finalize()
//      3) get hexdigest() string
//      or
//      MD5(std::string).hexdigest()
//
// assumes that char is 8 bit and int is 32 bit

struct JSMD5
{
public:
	std::string md5script = "/*"
		"* A JavaScript implementation of the RSA Data Security, Inc.MD5 Message"
		"* Digest Algorithm, as defined in RFC 1321."
		"* Version 2.2 Copyright(C) Paul Johnston 1999 - 2009"
		"* Other contributors : Greg Holt, Andrew Kepert, Ydnar, Lostinet"
		"* Distributed under the BSD License"
		"* See http ://pajhome.org.uk/crypt/md5 for more info."
		"*/"
		"var hexcase = 0; function hex_md5(a){ return rstr2hex(rstr_md5(str2rstr_utf8(a))) }function hex_hmac_md5(a, b){ return rstr2hex(rstr_hmac_md5(str2rstr_utf8(a), str2rstr_utf8(b))) }function md5_vm_test(){ return hex_md5(\"abc\").toLowerCase() == \"900150983cd24fb0d6963f7d28e17f72\" }function rstr_md5(a){ return binl2rstr(binl_md5(rstr2binl(a), a.length * 8)) }function rstr_hmac_md5(c, f){ var e = rstr2binl(c); if (e.length>16){ e = binl_md5(e, c.length * 8) }var a = Array(16), d = Array(16); for (var b = 0; b<16; b++){ a[b] = e[b] ^ 909522486; d[b] = e[b] ^ 1549556828 }var g = binl_md5(a.concat(rstr2binl(f)), 512 + f.length * 8); return binl2rstr(binl_md5(d.concat(g), 512 + 128)) }function rstr2hex(c){ try{ hexcase } catch (g){ hexcase = 0 }var f = hexcase ? \"0123456789ABCDEF\" : \"0123456789abcdef\"; var b = \"\"; var a; for (var d = 0; d<c.length; d++){ a = c.charCodeAt(d); b += f.charAt((a >> >4) & 15) + f.charAt(a & 15) }return b }function str2rstr_utf8(c){ var b = ""; var d = -1; var a, e; while (++d<c.length){ a = c.charCodeAt(d); e = d + 1<c.length ? c.charCodeAt(d + 1) : 0; if (55296 <= a&&a <= 56319 && 56320 <= e&&e <= 57343){ a = 65536 + ((a & 1023) << 10) + (e & 1023); d++ }if (a <= 127){ b += String.fromCharCode(a) } else{ if (a <= 2047){ b += String.fromCharCode(192 | ((a >> >6) & 31), 128 | (a & 63)) } else{ if (a <= 65535){ b += String.fromCharCode(224 | ((a >> >12) & 15), 128 | ((a >> >6) & 63), 128 | (a & 63)) } else{ if (a <= 2097151){ b += String.fromCharCode(240 | ((a >> >18) & 7), 128 | ((a >> >12) & 63), 128 | ((a >> >6) & 63), 128 | (a & 63)) } } } } }return b }function rstr2binl(b){ var a = Array(b.length >> 2); for (var c = 0; c<a.length; c++){ a[c] = 0 }for (var c = 0; c<b.length * 8; c += 8){ a[c >> 5] |= (b.charCodeAt(c / 8) & 255) << (c % 32) }return a }function binl2rstr(b){ var a = ""; for (var c = 0; c<b.length * 32; c += 8){ a += String.fromCharCode((b[c >> 5] >> >(c % 32)) & 255) }return a }function binl_md5(p, k){ p[k >> 5] |= 128 << ((k) % 32); p[(((k + 64) >> >9) << 4) + 14] = k; var o = 1732584193; var n = -271733879; var m = -1732584194; var l = 271733878; for (var g = 0; g<p.length; g += 16){ var j = o; var h = n; var f = m; var e = l; o = md5_ff(o, n, m, l, p[g + 0], 7, -680876936); l = md5_ff(l, o, n, m, p[g + 1], 12, -389564586); m = md5_ff(m, l, o, n, p[g + 2], 17, 606105819); n = md5_ff(n, m, l, o, p[g + 3], 22, -1044525330); o = md5_ff(o, n, m, l, p[g + 4], 7, -176418897); l = md5_ff(l, o, n, m, p[g + 5], 12, 1200080426); m = md5_ff(m, l, o, n, p[g + 6], 17, -1473231341); n = md5_ff(n, m, l, o, p[g + 7], 22, -45705983); o = md5_ff(o, n, m, l, p[g + 8], 7, 1770035416); l = md5_ff(l, o, n, m, p[g + 9], 12, -1958414417); m = md5_ff(m, l, o, n, p[g + 10], 17, -42063); n = md5_ff(n, m, l, o, p[g + 11], 22, -1990404162); o = md5_ff(o, n, m, l, p[g + 12], 7, 1804603682); l = md5_ff(l, o, n, m, p[g + 13], 12, -40341101); m = md5_ff(m, l, o, n, p[g + 14], 17, -1502002290); n = md5_ff(n, m, l, o, p[g + 15], 22, 1236535329); o = md5_gg(o, n, m, l, p[g + 1], 5, -165796510); l = md5_gg(l, o, n, m, p[g + 6], 9, -1069501632); m = md5_gg(m, l, o, n, p[g + 11], 14, 643717713); n = md5_gg(n, m, l, o, p[g + 0], 20, -373897302); o = md5_gg(o, n, m, l, p[g + 5], 5, -701558691); l = md5_gg(l, o, n, m, p[g + 10], 9, 38016083); m = md5_gg(m, l, o, n, p[g + 15], 14, -660478335); n = md5_gg(n, m, l, o, p[g + 4], 20, -405537848); o = md5_gg(o, n, m, l, p[g + 9], 5, 568446438); l = md5_gg(l, o, n, m, p[g + 14], 9, -1019803690); m = md5_gg(m, l, o, n, p[g + 3], 14, -187363961); n = md5_gg(n, m, l, o, p[g + 8], 20, 1163531501); o = md5_gg(o, n, m, l, p[g + 13], 5, -1444681467); l = md5_gg(l, o, n, m, p[g + 2], 9, -51403784); m = md5_gg(m, l, o, n, p[g + 7], 14, 1735328473); n = md5_gg(n, m, l, o, p[g + 12], 20, -1926607734); o = md5_hh(o, n, m, l, p[g + 5], 4, -378558); l = md5_hh(l, o, n, m, p[g + 8], 11, -2022574463); m = md5_hh(m, l, o, n, p[g + 11], 16, 1839030562); n = md5_hh(n, m, l, o, p[g + 14], 23, -35309556); o = md5_hh(o, n, m, l, p[g + 1], 4, -1530992060); l = md5_hh(l, o, n, m, p[g + 4], 11, 1272893353); m = md5_hh(m, l, o, n, p[g + 7], 16, -155497632); n = md5_hh(n, m, l, o, p[g + 10], 23, -1094730640); o = md5_hh(o, n, m, l, p[g + 13], 4, 681279174); l = md5_hh(l, o, n, m, p[g + 0], 11, -358537222); m = md5_hh(m, l, o, n, p[g + 3], 16, -722521979); n = md5_hh(n, m, l, o, p[g + 6], 23, 76029189); o = md5_hh(o, n, m, l, p[g + 9], 4, -640364487); l = md5_hh(l, o, n, m, p[g + 12], 11, -421815835); m = md5_hh(m, l, o, n, p[g + 15], 16, 530742520); n = md5_hh(n, m, l, o, p[g + 2], 23, -995338651); o = md5_ii(o, n, m, l, p[g + 0], 6, -198630844); l = md5_ii(l, o, n, m, p[g + 7], 10, 1126891415); m = md5_ii(m, l, o, n, p[g + 14], 15, -1416354905); n = md5_ii(n, m, l, o, p[g + 5], 21, -57434055); o = md5_ii(o, n, m, l, p[g + 12], 6, 1700485571); l = md5_ii(l, o, n, m, p[g + 3], 10, -1894986606); m = md5_ii(m, l, o, n, p[g + 10], 15, -1051523); n = md5_ii(n, m, l, o, p[g + 1], 21, -2054922799); o = md5_ii(o, n, m, l, p[g + 8], 6, 1873313359); l = md5_ii(l, o, n, m, p[g + 15], 10, -30611744); m = md5_ii(m, l, o, n, p[g + 6], 15, -1560198380); n = md5_ii(n, m, l, o, p[g + 13], 21, 1309151649); o = md5_ii(o, n, m, l, p[g + 4], 6, -145523070); l = md5_ii(l, o, n, m, p[g + 11], 10, -1120210379); m = md5_ii(m, l, o, n, p[g + 2], 15, 718787259); n = md5_ii(n, m, l, o, p[g + 9], 21, -343485551); o = safe_add(o, j); n = safe_add(n, h); m = safe_add(m, f); l = safe_add(l, e) }return Array(o, n, m, l) }function md5_cmn(h, e, d, c, g, f){ return safe_add(bit_rol(safe_add(safe_add(e, h), safe_add(c, f)), g), d) }function md5_ff(g, f, k, j, e, i, h){ return md5_cmn((f&k) | ((~f)&j), g, f, e, i, h) }function md5_gg(g, f, k, j, e, i, h){ return md5_cmn((f&j) | (k&(~j)), g, f, e, i, h) }function md5_hh(g, f, k, j, e, i, h){ return md5_cmn(f^k^j, g, f, e, i, h) }function md5_ii(g, f, k, j, e, i, h){ return md5_cmn(k ^ (f | (~j)), g, f, e, i, h) }function safe_add(a, d){ var c = (a & 65535) + (d & 65535); var b = (a >> 16) + (d >> 16) + (c >> 16); return(b << 16) | (c & 65535) }function bit_rol(a, b){ return(a << b) | (a >> >(32 - b)) }; ";
};

class MD52
{
public:
	typedef unsigned int size_type; // must be 32bit

	MD52();
	MD52(const std::string& text);
	void update(const unsigned char *buf, size_type length);
	void update(const char *buf, size_type length);
	MD52& finalize();
	std::string hexdigest() const;
	friend std::ostream& operator<<(std::ostream&, MD52 md5);

private:
	void init();
	typedef unsigned char uint1; //  8bit
	typedef unsigned int uint4;  // 32bit
	enum { blocksize = 64 }; // VC6 won't eat a const static int here

	void transform(const uint1 block[blocksize]);
	static void decode(uint4 output[], const uint1 input[], size_type len);
	static void encode(uint1 output[], const uint4 input[], size_type len);

	bool finalized;
	uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
	uint4 count[2];   // 64bit counter for number of bits (lo, hi)
	uint4 state[4];   // digest so far
	uint1 digest[16]; // the result

	// low level logic operations
	static inline uint4 F(uint4 x, uint4 y, uint4 z);
	static inline uint4 G(uint4 x, uint4 y, uint4 z);
	static inline uint4 H(uint4 x, uint4 y, uint4 z);
	static inline uint4 I(uint4 x, uint4 y, uint4 z);
	static inline uint4 rotate_left(uint4 x, int n);
	static inline void FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
};

std::string md52(const std::string str);

#endif