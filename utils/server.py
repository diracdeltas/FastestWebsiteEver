#!/usr/bin/env python

import SimpleHTTPServer
import BaseHTTPServer

class CustomHTTPRequestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Content-Encoding', 'deflate')
        SimpleHTTPServer.SimpleHTTPRequestHandler.end_headers(self)

if __name__ == '__main__':
    BaseHTTPServer.test(HandlerClass=CustomHTTPRequestHandler, ServerClass = BaseHTTPServer.HTTPServer, protocol="HTTP/1.1")
