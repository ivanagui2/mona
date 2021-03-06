/*
 * CommandOptionParser.h - 
 *
 *   Copyright (c) 2011  Higepon(Taro Minowa)  <higepon@users.sourceforge.jp>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _COMMAND_OPTION_PARSER_
#define _COMMAND_OPTION_PARSER_

#include <string>
#include <vector>

namespace MonAPI {

class CommandOptionParser
{
public:
    CommandOptionParser() {}
    virtual ~CommandOptionParser() {}

    intptr_t parse(std::vector<std::string>& dest, const std::string& options)
    {
        std::string accum;
        for (size_t i = 0; i < options.size(); i++) {
            int c = options[i];
            if (c == ' ') {
                dest.push_back(accum);
                accum = "";
            } else if (accum.empty() &&  c == '"') {
                dest.push_back(readQuoated(++i, options));
            } else {
                accum += c;
            }
        }
        if (!accum.empty()) {
            dest.push_back(accum);
        }
        return M_OK;
    }

private:
    std::string readQuoated(size_t& index, const std::string& options)
    {
        std::string accum;
        for (; index < options.size(); index++) {
            int c = options[index];
            if (c == '"') {
                index++;
                break;
            } else {
                accum += c;
            }
        }
        return accum;
    }
};

}; // MonAPI
#endif // _COMMAND_OPTION_PARSER_
