/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    peerstubgenerator.h
 * @date    6/27/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef PEERSTUBGENERATOR_H
#define PEERSTUBGENERATOR_H

#include <string>
#include <jsonrpccpp/stubgen/server/cppserverstubgenerator.h>

class PeerStubGenerator : public jsonrpc::CPPServerStubGenerator
{
    public:
        PeerStubGenerator(const std::string& stubname, std::vector<jsonrpc::Procedure> &subscribeProcs, std::vector<jsonrpc::Procedure> &publishProcs, const std::string &filename);

        virtual void generateStub();

        virtual void generatePublishMethods();

        void generateParamMappings(jsonrpc::Procedure& proc);

        void generateTopicConstants();

    private:
        std::vector<jsonrpc::Procedure> &publishProcs;
        std::vector<jsonrpc::Procedure> &subscribeProcs;

};

#endif // PEERSTUBGENERATOR_H
