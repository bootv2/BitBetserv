/* 
   UrlHelper.cpp

   Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/


#include "UrlHelper.h"
#include "Tracer.h"
#include "stdHelpers.h"

#include <windows.h>
#include <sstream>
#include <iostream>
#include <vector>

bool RemoveProtocolFromUrl(std::string const& url, std::string& protocol, std::string& rest) {
  TraceFunc("RemoveProtocolFromUrl");
  std::string::size_type pos_colon = url.find(":");

  if (pos_colon == std::string::npos) {
    rest = url;  
    return false;
  }

  if (url.size() < pos_colon + 2) {
    rest = url;
    return false;
  }

  if (url[pos_colon+1] != '/' ||
      url[pos_colon+2] != '/')  {
    rest = url;
    return false;
  }

  protocol = url.substr(0,pos_colon);
  rest     = url.substr(3+pos_colon);  // Skipping three characters ( '://' )

  return true;
}

void SplitPostReq(std::string _post_req, std::string& path, std::map<std::string, std::string>& params, size_t posStartPath)
{
	TraceFunc("SplitPostReq");
	std::cout << "Entered SplitPostReq()\n";
	std::cout << _post_req.size() << std::endl << _post_req << std::endl;
	path = _post_req.substr(5, _post_req.find_first_of(' ') - 1);
	std::cout << path << std::endl;
	std::vector<std::string> post_req;
	int lastIterator = 0;
	for (int i = 0; i < _post_req.size(); i++)
	{
		if (_post_req.at(i) == '\x0d' || _post_req.at(i) == '\x0a')
		{
			for (int x = 0; x < _post_req.size(); x++)
			{
				if (_post_req.at(x) == '\x0d' || _post_req.at(x) == '\x0a')
				{
					std::cout << _post_req.substr(i + 1, x - 1) << " i = " << i << std::endl;//TODO fix empty post_req vector problem
					post_req.emplace_back(_post_req.substr(i + 1, x - 1));
					i = x;
				}
			}
		}
	}
	for (int i = 0; i < post_req.size(); i++) std::cout << post_req.at(i) << ":" << post_req.at(i).size() << std::endl;
	post_req.at(0) = post_req.at(0).substr(4, post_req.at(0).size());//delete "POST " from the first line.
	bool trailerDeleted = false;
	for (int i = 1; i < post_req.at(0).size(); i++)
	{
		for (int x = i; x < post_req.at(0).size(); x++)
		{
			if (post_req.at(0).substr(i, x) == "http/1.")
			{
				post_req.at(0) = post_req.at(0).substr(0, i);
				trailerDeleted = true;
				break;
			}
		}
		if (trailerDeleted) break;
	}

	path = post_req.at(0);

	trailerDeleted = false;

	int POSTargsline = -1;

	for (int i = 0; i < post_req.size(); i++)
	{
		if (post_req.at(i).at(0) = '-')
			POSTargsline = i;
	}

	std::string name, val, args;
	bool loopbreak = false;

	if (POSTargsline == -1) return;
	else
	{
		args = post_req.at(POSTargsline);
		for (int i = 0; i < args.size(); i++)
		{
			for (int x = i; x < args.size(); x++)
			{
				if (args.at(x) == '=')
				{
					name = args.substr(i, x);
					i = x;
					loopbreak = true;
					break;
				}
				if (args.at(x) == '&')
				{
					val = args.substr(i, x);
					params[name] = val;
					name = "";
					val = "";
					loopbreak = true;
					break;
				}
			}
			if (!loopbreak)
			{
				params[name] = val;
				break;
			}
			loopbreak = false;
		}

	}



}

std::vector<std::string> ssplit(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> ssplit(const std::string &s, char delim) {
	std::vector<std::string> elems;
	ssplit(s, delim, elems);
	return elems;
}

void SplitGetReq(std::string get_req, std::string& path, std::map<std::string, std::string>& params) {
  TraceFunc("SplitGetReq");

  // Remove trailing newlines
  if (get_req[get_req.size()-1] == '\x0d' ||
      get_req[get_req.size()-1] == '\x0a')
      get_req=get_req.substr(0, get_req.size()-1);

  if (get_req[get_req.size()-1] == '\x0d' ||
      get_req[get_req.size()-1] == '\x0a')
      get_req=get_req.substr(0, get_req.size()-1);

  // Remove potential Trailing HTTP/1.x
  if (get_req.size() > 7) {
    if (get_req.substr(get_req.size()-8, 7) == "HTTP/1.") {
      get_req=get_req.substr(0, get_req.size()-9);
    }
  }

  std::string::size_type qm = get_req.find("?");
  if (qm != std::string::npos) {
    std::string url_params = get_req.substr(qm+1);

    path = get_req.substr(0, qm);

    // Appending a '&' so that there are as many '&' as name-value pairs.
    // It makes it easier to split the url for name value pairs, he he he
    url_params += "&";

    std::string::size_type next_amp = url_params.find("&");

    while (next_amp != std::string::npos) {
      std::string name_value = url_params.substr(0,next_amp);
      url_params             = url_params.substr(next_amp+1);
      next_amp               = url_params.find("&");

      std::string::size_type pos_equal = name_value.find("=");

      std::string nam = name_value.substr(0,pos_equal);
      std::string val = name_value.substr(pos_equal+1);

      std::string::size_type pos_plus;
      while ( (pos_plus = val.find("+")) != std::string::npos ) {
        val.replace(pos_plus, 1, " ");
      }

      // Replacing %xy notation
      std::string::size_type pos_hex = 0;
      while ( (pos_hex = val.find("%", pos_hex)) != std::string::npos ) {
        std::stringstream h;
        h << val.substr(pos_hex+1, 2);
        h << std::hex;

        int i;
        h>>i;

        std::stringstream f;
        f << static_cast<char>(i);
        std::string s;
        f >> s;

        val.replace(pos_hex, 3, s);
        pos_hex ++;
      }

	  std::cout << nam << " " << val << std::endl;
      params.insert(std::map<std::string,std::string>::value_type(nam, val));
    }
  }
  else {
    path = get_req;
  }
}

void SplitUrl(std::string const& url, std::string& protocol, std::string& server, std::string& path) {
  TraceFunc("SplitUrl");
  RemoveProtocolFromUrl(url, protocol, server);

  Trace2("protocol: ", protocol);
  Trace2("server:   ", server  );

  if (protocol == "http") {
    std::string::size_type pos_slash = server.find("/");
  
    if (pos_slash != std::string::npos) {
      Trace("slash found");
      path   = server.substr(pos_slash);
      server = server.substr(0, pos_slash);
    }
    else {
      Trace("slash not found");
      path = "/";
    }
  }
  else if (protocol == "file") {
    path = ReplaceInStr(server, "\\", "/");
    server = "";
  }
  else {
    std::cerr << "unknown protocol in SplitUrl: '" << protocol << "'" << std::endl;
  }
}
