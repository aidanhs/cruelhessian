/*   Regex.cpp
 *
 *   Cruel Hessian
 *   Copyright (C) 2008, 2009, 2010, 2011 by Paweł Konieczny <konp84 at gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#include "Regex.h"
#include <pcre.h>
#include <cstring>


#define OVECCOUNT 1024


bool regex_match(const std::string& name, const std::string& pattern)
{

    int ovector[OVECCOUNT];
    const char *errStr;
    int errOff;
    char *input = const_cast<char*>(name.c_str());

    pcre *re = pcre_compile(const_cast<char*>(pattern.c_str()), PCRE_CASELESS, &errStr, &errOff, NULL);
    int rc = pcre_exec(re, NULL, input, strlen(input), 0, PCRE_ANCHORED, ovector, OVECCOUNT);

    return (rc < 0 ? false : true);

}
