/*
 * Copyright (C) 2014-2019 fdns Authors
 *
 * This file is part of fdns project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "fdns.h"

static int server_id = -1;
static char *server_default = "cloudflare";

DnsServer server[] = {
	{
		"42l",		// name
		"https://42l.fr",	// website
		"non-profit, France",		// description

		"185.216.27.142:443",		// IP address

		// POST request
		"POST /dns-query HTTP/1.1\r\n" \
		"Host: doh.42l.fr\r\n" \
		"accept: application/dns-message\r\n" \
		"content-type: application/dns-message\r\n" \
		"content-length: %d\r\n" \
		"\r\n",

		30			// keepalive in seconds
	},

	{
		"appliedprivacy",		// name
		"https://appliedprivacy.net ",	// website
		"non-profit, Austria",		// description

		"37.252.185.229:443",		// IP address

		// POST request
		"POST /query HTTP/1.1\r\n" \
		"Host: doh.appliedprivacy.net\r\n" \
		"accept: application/dns-message\r\n" \
		"content-type: application/dns-message\r\n" \
		"content-length: %d\r\n" \
		"\r\n",

		30			// keepalive in seconds
	},

	{
		"cleanbrowsing",
		"https://cleanbrowsing.org",
		"anycast network, security filter",

		"185.228.168.168:443",

		"POST /doh/security-filter HTTP/1.1\r\n" \
		"Host: doh.cleanbrowsing.org\r\n" \
		"accept: application/dns-message\r\n" \
		"content-type: application/dns-message\r\n" \
		"content-length: %d\r\n" \
		"\r\n",

		30
	},

	{
		"cleanbrowsing-family",
		"https://cleanbrowsing.org",
		"anycast network, security filter + family filter",

		"185.228.168.168:443",

		"POST /doh/family-filter HTTP/1.1\r\n" \
		"Host: doh.cleanbrowsing.org\r\n" \
		"accept: application/dns-message\r\n" \
		"content-type: application/dns-message\r\n" \
		"content-length: %d\r\n" \
		"\r\n",

		30
	},

	{
		"cloudflare",
		"https://www.cloudflare.com",
		"massive anycast network",

		"1.1.1.1:443",
		"POST /dns-query HTTP/1.1\r\n" \
		"Host: cloudflare-dns.com\r\n" \
		"accept: application/dns-message\r\n" \
		"content-type: application/dns-message\r\n" \
		"content-length: %d\r\n" \
		"\r\n",

		250			// keepalive in seconds
	},


#if 0
	{
		"google",
		"https://dns.google",
		"massive anycast network, heavy logging!!!",

		"8.8.8.8:443",

		"POST /dns-query HTTP/1.1\r\n" \
		"Host: dns.google\r\n" \
		"accept: application/dns-message\r\n" \
		"content-type: application/dns-message\r\n" \
		"content-length: %d\r\n" \
		"\r\n",

		250
	},
#endif

	{
		"powerdns",
		"https://powerdns.org",
		"Netherlands",

		"136.144.215.158:443",

		"POST /dns-query HTTP/1.1\r\n" \
		"Host: doh.powerdns.org\r\n" \
		"accept: application/dns-message\r\n" \
		"content-type: application/dns-message\r\n" \
		"content-length: %d\r\n" \
		"\r\n",

		7
	},

	{
		"quad9",
		"https://quad9.net",
		"massive anycast network, security filter",

		"9.9.9.9:443",

		"POST /dns-query HTTP/1.1\r\n" \
		"Host: dns.quad9.net\r\n" \
		"accept: application/dns-message\r\n" \
		"content-type: application/dns-message\r\n" \
		"content-length: %d\r\n" \
		"\r\n",

		7
	},

	{
		"seby.io",		// name
		"https://dns.seby.io ",	// website
		"Australia",		// description

		"45.76.113.31:8443",		// IP address

		// POST request
		"POST /dns-query HTTP/1.1\r\n" \
		"Host: dns.seby.io\r\n" \
		"accept: application/dns-message\r\n" \
		"content-type: application/dns-message\r\n" \
		"content-length: %d\r\n" \
		"\r\n",

		7			// keepalive in seconds
	},

	{ NULL, NULL, NULL, NULL, NULL, 0}
};

void dns_list(void) {
	int i = 0;
	while(server[i].name) {
		// name - website
		printf("%s - %s\n", server[i].name, server[i].website);

		// description.
		printf("\t%s; SSL keepalive %ds\n", server[i].description, server[i].ssl_keepalive);
		i++;
	}
}

void dns_set_server(const char *srv) {
	assert(srv);

	int i = 0;
	while (server[i].name) {
		if (strcmp(server[i].name, srv) == 0) {
			server_id = i;
			return;
		}
		i++;
	}

	if (arg_id == -1)
		logprintf("Warning: server %s not found, using %s\n", srv, server[server_id].name);
}

DnsServer *dns_get_server(void) {
	if (server_id == -1) {
		// set the default
		int i = 0;
		while (server[i].name) {
			if (strcmp(server[i].name, server_default) == 0) {
				server_id = i;
				break;
			}
			i++;
		}
	}

	assert(server_id != -1);
	return server + server_id;
}
