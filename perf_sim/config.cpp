/*
 * config.cpp - implementation of Config class
 * Copyright 2017 MIPT-MIPS
 */

/* Simulator modules */
#include "config.h"

/* Generic C++ */
#include <iostream>

namespace po = boost::program_options;

std::list<Config::BasicValue*> Config::BasicValue::values;

template<>
void Config::Value<bool>::reg(bod& d)
{
    namespace po = boost::program_options;
    d.add_options()(name.c_str(),
                    po::bool_switch( &value)->default_value( default_value),
                    desc.c_str());
}

/* basic method */
int Config::handleArgs( int argc, char** argv)
{
    po::options_description description( "Allowed options");

    for ( Config::BasicValue* value : Config::BasicValue::get_values())
         value->reg(description);

    po::variables_map vm;

    try
    {
        po::store(po::command_line_parser(argc, argv).
                                    options(description).
                                    run(),
                                    vm);


        /* parsing help */
        if ( vm.count( "help"))
        {
            std::cout << "Functional and performance simulators for MIPS-based CPU."
                      << std::endl << std::endl
                      << description << std::endl;
            std::exit( EXIT_SUCCESS);
        }

        /* calling notify AFTER parsing help, as otherwise
         * absent required args will cause errors
         */
        po::notify(vm);
    }
    catch ( const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what()
                  << std::endl << std::endl
                  << description << std::endl;
        std::exit( EXIT_SUCCESS);
    }

    return 0;
}
