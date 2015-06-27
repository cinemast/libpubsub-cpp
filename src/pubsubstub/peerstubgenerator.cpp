/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    peerstubgenerator.cpp
 * @date    6/27/2015
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "peerstubgenerator.h"
#include <jsonrpccpp/stubgen/helper/cpphelper.h>
#include <algorithm>

using namespace jsonrpc;
using namespace std;

#define  TEMPLATE_CPPSERVER_NOTIFICATIONBINDING "this->bindAndAddNotification(jsonrpc::Procedure(\"<rawprocedurename>\", <paramtype>, <parameterlist> NULL), &<stubname>::<procedurename>I);"

#define TEMPLATE_CPPSERVER_SIGCLASS "class <stubname> : public PubSubPeer"
#define TEMPLATE_CPPSERVER_SIGCONSTRUCTOR "<stubname>(const std::string &ip, int port) : PubSubPeer(ip, port)"

#define TEMPLATE_CPPSERVER_SIGNOTIFICATION "inline virtual void <procedurename>I(const Json::Value &request)"
#define TEMPLATE_SERVER_ABSTRACTDEFINITION "virtual <returntype> <procedurename>(<parameterlist>) = 0;"

#define TEMPLATE_NAMED_ASSIGNMENT "p[\"<paramname>\"] = <paramname>;"


PeerStubGenerator::PeerStubGenerator(const std::string &stubname, std::vector<Procedure> &subscribeProcs, std::vector<Procedure> &publishProcs, const std::string &filename) :
    CPPServerStubGenerator(stubname, subscribeProcs, filename),
    publishProcs(publishProcs),
    subscribeProcs(subscribeProcs)
{
    for (auto &proc: subscribeProcs)
    {
        proc.SetProcedureName("on"+ proc.GetProcedureName());
    }
}

void PeerStubGenerator::generateStub()
{
    CPPHelper::prolog(*this, this->stubname);

    this->writeLine("#include <pubsubcpp/pubsubpeer.h>");
    this->writeLine("#include <string>");

    this->writeNewLine();

    int depth = CPPHelper::namespaceOpen(*this, stubname);

    this->writeLine(replaceAll(TEMPLATE_CPPSERVER_SIGCLASS, "<stubname>", this->stubname));
    this->writeLine("{");
    this->increaseIndentation();
    this->writeLine("public:");
    this->increaseIndentation();

    this->writeLine(replaceAll(TEMPLATE_CPPSERVER_SIGCONSTRUCTOR, "<stubname>", this->stubname));
    this->writeLine("{");
    this->generateBindings();
    this->writeLine("}");

    this->writeNewLine();

    this->generateProcedureDefinitions();

    this->generateAbstractDefinitions();

    this->generatePublishMethods();

    this->generateTopicConstants();

    this->decreaseIndentation();
    this->decreaseIndentation();
    this->writeLine("};");
    this->writeNewLine();

    CPPHelper::namespaceClose(*this, depth);
    CPPHelper::epilog(*this,this->stubname);
}

void PeerStubGenerator::generatePublishMethods()
{
    for (auto &proc : this->publishProcs)
    {
        string methodName = CPPHelper::normalizeString(proc.GetProcedureName());
        methodName[0] = toupper(methodName[0]);
        this->write("void publish" + methodName + "(");
        this->write(CPPHelper::generateParameterDeclarationList(proc));
        this->writeLine(")");
        this->writeLine("{");
        this->increaseIndentation();

        this->generateParamMappings(proc);

        this->writeLine("this->publishTopic(\""+proc.GetProcedureName()+"\", p);");

        this->decreaseIndentation();
        this->writeLine("}");
    }
}

void PeerStubGenerator::generateParamMappings(Procedure &proc)
{
    string assignment;
    this->writeLine("Json::Value p;");
    parameterNameList_t list = proc.GetParameters();
    if(list.size() > 0)
    {
        for (parameterNameList_t::iterator it = list.begin(); it != list.end(); ++it)
        {
            assignment = TEMPLATE_NAMED_ASSIGNMENT;
            replaceAll2(assignment, "<paramname>", it->first);
            this->writeLine(assignment);
        }
    }
    else
    {
        this->writeLine("p = Json::nullValue;");
    }
}

void PeerStubGenerator::generateTopicConstants()
{
    string constantname;
    string constantvalue;

    this->writeNewLine();

    this->writeLine("//Topic Constants, use for registering publish/subscribe topics");

    for(auto& proc: this->publishProcs)
    {
        constantname = "TOPIC_PUBLISH_" + CPPHelper::normalizeString(proc.GetProcedureName());
        constantvalue = proc.GetProcedureName();
        std::transform(constantname.begin(), constantname.end(), constantname.begin(),::toupper);
        this->writeLine("static constexpr const char* " + constantname + " = \"" + constantvalue + "\";");
    }

    for(auto& proc: this->subscribeProcs)
    {
        constantname = "TOPIC_SUBSCRIBE_" + CPPHelper::normalizeString(proc.GetProcedureName());
        constantvalue = proc.GetProcedureName();
        std::transform(constantname.begin(), constantname.end(), constantname.begin(),::toupper);
        this->writeLine("static constexpr const char* " + constantname + " = \"" + constantvalue + "\";");
    }

}
