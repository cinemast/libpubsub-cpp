/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file
 * @date    7/5/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <string>

//if interface == "", return first valid IP-Address that is non local link only.
std::string getIPAddress(const std::string &interface="");

#endif // NETWORKHELPER_H
