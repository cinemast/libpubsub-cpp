/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    main.cpp
 * @date    6/25/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <jsonrpccpp/common/specificationparser.h>
#include "peerstubgenerator.h"

using namespace std;
using namespace jsonrpc;





int handleParameters(Json::Value& result, int argc, char** argv)
{
    if (argc < 2)
    {
        cerr << "too few arguments" << endl;
        return 1;
    }

    ifstream stream;
    stream.open(argv[1]);

    if(!stream.is_open())
    {
        cerr << "file not found: " << argv[1] << endl;
        return 1;
    }

    Json::Reader reader;

    if (!reader.parse(stream, result))
    {
        cerr << argv[1] << " is not a valid JSON file" << endl;
        return 1;
    }
    return 0;
}

bool checkTopLevelStructs(Json::Value &value)
{
    if (value.isMember("topics") && value["topics"].isArray())
    {
        if (value.isMember("peers") && value["peers"].isArray())
        {
            return true;
        }
    }
    return false;
}

Procedure findProcedure(vector<Procedure>& procs, const string& name)
{
    for (auto& proc: procs)
    {
        if (proc.GetProcedureName() == name)
        {
            return proc;
        }
    }
    return Procedure();
}

int main(int argc, char** argv)
{
    Json::Value root;
    int result = handleParameters(root, argc, argv);
    if (result != 0)
        return result;

    if (!checkTopLevelStructs(root))
    {
        cerr << "Invalid JSON structure" << endl;
        return 1;
    }


    std::vector<Procedure> procs = jsonrpc::SpecificationParser::GetProceduresFromString(root["topics"].toStyledString());

    for (auto i=0; i < root["peers"].size(); i++)
    {
        Json::Value peer = root["peers"][i];
        string peerName = peer["name"].asString();

        cout << peer.toStyledString() << endl;

        vector<Procedure> subscribeProcs;
        vector<Procedure> publishProcs;

        if (peer.isMember("subscribe"))
        {
            peerName = string("Abstract") + peerName;
            for (auto i=0; i < peer["subscribe"].size(); i++)
            {
                Procedure proc = findProcedure(procs, peer["subscribe"][i].asString());
                if (proc.GetProcedureName() != "")
                    subscribeProcs.push_back(findProcedure(procs, peer["subscribe"][i].asString()));
                else
                {
                    cerr << "Could not find referenced notification: " << peer["subscribe"][i].asString() << endl;
                    return 1;
                }
            }
        }

        if (peer.isMember("publish"))
        {
            for (auto i=0; i < peer["publish"].size(); i++)
            {
                Procedure proc = findProcedure(procs, peer["publish"][i].asString());
                if (proc.GetProcedureName() != "")
                    publishProcs.push_back(findProcedure(procs, peer["publish"][i].asString()));
                else
                {
                    cerr << "Could not find referenced notification: " << peer["publish"][i].asString() << endl;
                    return 1;
                }
            }
        }

        PeerStubGenerator peerStub(peerName, subscribeProcs, publishProcs, peerName + ".h");

        peerStub.generateStub();
    }

    return 0;
}

