//
//  argparser.cpp
//  miteFinder
//
//  Created by Jialu Hu on 2017/5/7.
//  Copyright © 2017年 Jialu Hu. All rights reserved.
//

#include "argparser.h"
#include <stdlib.h>

ArgParser::ArgParser(int argc, const char** argv){
	optnum=argc;
	const char* pArg;
	//commandName=argv[0];
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			pArg=&argv[i][1];
			args.push(std::string(pArg));
		}else{
			parValues.push(argv[i]);
		}
	}
    boolOption("help","Show help information.");
    boolOption("version","Show the current version.");
}
ArgParser::~ArgParser(){
}
void ArgParser::setName(const char* comName,const char* comDesc)
{
    commandName=comName;
    commandDesc=comDesc;
}
void ArgParser::setVerion(const char* version)
{
    commandVersion=version;
}
void ArgParser::boolOption(const char* optName, const char* help,bool mandatory){
    std::string opt=optName;
    ParData obj;
    obj.type=BOOL;
    obj.help=help;
    obj.mandatory=mandatory;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::refOption(const std::string &optName,const std::string &help,
                                int &refVariable,bool mandatory){
    std::string opt=optName;
    ParData obj;
    obj.type=INTEGER;
    obj.mandatory=mandatory;
    obj.help=help;
    obj.pInt=&refVariable;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::refOption(const std::string &optName,const std::string &help,
                                std::string &refVariable,bool mandatory){
    std::string opt=optName;
    ParData obj;
    obj.type=STRING;
    obj.mandatory=mandatory;
    obj.help=help;
    obj.pString=&refVariable;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::refOption(const std::string &optName,const std::string &help,
                                double &refVariable,bool mandatory){
    std::string opt=optName;
    ParData obj;
    obj.type=DOUBLE;
    obj.mandatory=mandatory;
    obj.help=help;
    obj.pDouble=&refVariable;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::showUsages()
{
	std::cerr <<"USAGE: "<< commandName<<" ";
	for(std::unordered_map<std::string, ParData>::iterator it=para_map.begin();it!=para_map.end();it++)
	{
		if(!it->second.mandatory)
			std::cerr <<" [";
		if(it->second.type==STRING)
			std::cerr <<"-"<<it->first<<" STRING";
		if(it->second.type==INTEGER)
			std::cerr <<"-"<<it->first<<" INTEGER";
		if(it->second.type==DOUBLE)
		std::cerr <<"-"<<it->first<<" DOUBLE";
		if(it->second.type==BOOL)
			std::cerr <<"-"<<it->first;
		if(!it->second.mandatory)
			std::cerr <<"]";
	}
	std::cerr <<std::endl;
    std::cerr <<"DESCRIPTION: "<<commandDesc<<std::endl;
}
void ArgParser::showOptions()
{
    showUsages();
    std::cerr <<"OPTIONAL ARGUMENTS: "<<std::endl;
    for(std::unordered_map<std::string, ParData>::iterator it=para_map.begin();it!=para_map.end();it++)
    {
        std::cerr << "-" << it->first<<"\t" << it->second.help<< std::endl;
        
        /*if(!it->second.mandatory)
            std::cerr <<" [";
        if(it->second.type==STRING)
            std::cerr <<"-"<<it->first<<" STRING";
        if(it->second.type==INTEGER)
            std::cerr <<"-"<<it->first<<" INTEGER";
        if(it->second.type==DOUBLE)
            std::cerr <<"-"<<it->first<<" DOUBLE";
        if(it->second.type==BOOL)
            std::cerr <<"-"<<it->first;
        if(!it->second.mandatory)
            std::cerr <<"]";*/
    }
    
}
bool ArgParser::checkMandatories()
{
    for (std::unordered_map<std::string, ParData>::iterator it=para_map.begin();it!=para_map.end();it++)
    {
        if (it->second.mandatory==1&&it->second.sign==false)
        {
            std::cerr <<"WARNING: "<<"The parameter "<<it->first<<" should be specified."<< std::endl;
			showUsages();
            showOptions();
            return false;
        }
    }
    return true;
}
bool ArgParser::run()
{
    // check mandatory
    // check help version
    //
    while(!args.empty()){
        std::string optname=args.front();
        std::unordered_map<std::string, ParData>::iterator it=para_map.find(optname);
        if (it!=para_map.end()){
            it->second.sign=true;
            switch (it->second.type) {
                case UNKNOWN:
                    break;
                case BOOL:
                    //it->second.pBool=it
                    break;
                case STRING:
                    *it->second.pString=parValues.front();
                    break;
                case DOUBLE:
                    {
                        if (atof(parValues.front().c_str())==0)
                        std::cerr <<"WARNING:"<<"The "<<it->first<<" should be "<<it->second.help <<std::endl;
                        else
                        *it->second.pDouble=atof(parValues.front().c_str());
                    }
                    break;
                case INTEGER:
                    {
                        if (atoi(parValues.front().c_str())==0)
                        std::cerr <<"WARNING:"<<"The "<<it->first<<" should be "<<it->second.help <<std::endl;
                        else
                        *it->second.pInt=atoi(parValues.front().c_str());
                    }
                    break;
                case FUNC:
                    break;
            }
        }
        args.pop();
        parValues.pop();
    }
    return ArgParser::checkMandatories();
}
