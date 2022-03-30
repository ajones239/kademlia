/*
 *
 * utility functions
 *
 */

/*
 * Get a port number (in network byte order) from a service string and a transport protocol
 * * @param char *svcStr - service to use
 * @param char *transportProto - either "tcp" or "udp"
 * @return port number in network byte order
 */
int getPortFromSvc(const char *svcStr, const char *transportProto);


/*
 *
 * client socket functions
 *
 */

/*
 * Connects to a remote system using the given service over TCP
 * @param char *host - host to connect to
 * @param char *svc - service to use
 * @return int - socket descriptor
 */
int connectTCP(char *host, char *svc);

/*
 * Connects to a remote system using the given service over UDP
 * @param char *host - host to connect to
 * @param char *svc - service to use
 * @return int - socket descriptor
 */
int connectUDP(char *host, char *svc);

/*
 * Connects to a remote system at the given port over TCP
 * @param char *host - host to connect to
 * @param int port - port to connect to
 * @return int - socket descriptor
 */
int connectTCPWithPort(char *host, int port);

/*
 * Connects to a remote system at the given port over UDP
 * @param char *host - host to connect to
 * @param int port - port to connect to
 * @return int - socket descriptor
 */
int connectUDPWithPort(char *host, int port);

/*
 * Connects to a remote system using the given service over the given transport protocol
 * @param char *hostStr - host to connect to
 * @param int port - port to connect to
 * @param char *transportProto - either "tcp" or "udp"
 * @return int - socket descriptor
 */
int connectSock(char *hostStr, int port, char *transportProto);

/*
 *
 * server socket functions
 * 
 */

/*
 * Create a passive UDP socket for the given service
 * @param char *service - service to create passive socket for
 * @return int - socket descriptor
 */
int passiveUDP(const char *service);

/*
 * Create a passive TCP socket for the given service
 * @param char *service - service to create passive socket for
 * @param int qlen - the client queue length
 * @return int - socket descriptor
 */
int passiveTCP(const char *service, int qlen);

/*
 * Create a passive UDP socket listening on the given port
 * @param int port - port to listen on (in host byte order)
 * @return int- socket descriptor
 */
int passiveUDPWithPort(const int port);

/*
 * Create a passive TCP socket listening on the given port
 * @param int port - port to listen on (in host byte order)
 * @param int qlen - the client queue length
 * @return int- socket descriptor
 */
int passiveTCPWithPort(const int port, int qlen);

/*
 * Create a passive socket for the given service and transport protocol
 * @param int port - port to listen on (in network byte order)
 * @param char *transportProto - either "tcp" or "udp"
 * @param int qlen - the client queue length (0 if transportProto is "udp")
 * @return int - socket descriptor
 */
int passiveSock(const int port, const char *transportProto, int qlen);

