##import socket
##
##UDP_IP = "192.168.1.42"
##UDP_PORT = 8888
##MESSAGE = "Hello, World!"
##
##print "UDP target IP:", UDP_IP
##print "UDP target port:", UDP_PORT
##print "message:", MESSAGE
## 
##sock = socket.socket(socket.AF_INET, # Internet
##                     socket.SOCK_DGRAM) # UDP
##sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

##import socket
##from datetime import datetime
##
##UDP_IP = "192.168.1.40"
##UDP_PORT = 15896
##
##sock = socket.socket(socket.AF_INET, # Internet
##                     socket.SOCK_DGRAM) # UDP
##sock.bind((UDP_IP, UDP_PORT))
##
##while True:
##    try:
##        data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
##        print "received message:", data
##        print datetime. now()
##    except :
##        pass

from bottle import route, run, template

@route('/hello')
def index():
    return "Hello World!"

run(host='0.0.0.0', port=8080)
