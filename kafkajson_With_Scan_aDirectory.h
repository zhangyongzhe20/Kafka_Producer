/*
* Produce Json data to kafka server
*/

#ifndef KAFKAJSON_H
#define KAFKAJSON_H

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <cppkafka/cppkafka.h>
#include <fstream>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>;
using json = nlohmann::json;
using namespace cppkafka;
using namespace std;
namespace fs = ::boost::filesystem;
namespace po = ::boost::program_options;

namespace kafkajson
{
     class dumpData
     {
     public:
          // pass arguments, topic & directory
          string topic_name;
          string brokers = "127.0.0.1:9092";
          fs::path dir_path;
         // string file_name;
          int partition_value = 0;

          //create parameterized constructors
          dumpData(string topic, string dir)
          {
               topic_name = topic;
               dir_path= fs::path(dir);
          }

          void searchDirs(const fs::path &root, const string &ext, vector<string> &ret)
          {
               if (!fs::exists(root) || !fs::is_directory(root))
                    return;
               fs::recursive_directory_iterator it(root);
               fs::recursive_directory_iterator endit;
               while (it != endit)
               {
                    if (fs::is_regular_file(*it) && it->path().extension() == ext)
                         ret.push_back(it->path().string());
                    ++it;
               }
          }

          void dumpDataToKafka(Producer &producer, MessageBuilder &builder, const string &file)
          {
               // open file
               ifstream infile(file);
               //serialize to json object
               json jobject;
               infile >> jobject;
               // deserialized to string
               string jobject_tostring = jobject.dump();
               producer.produce(builder.payload(jobject_tostring));
               infile.close();
          }

          void main_dump()
          {
               // po::options_description options("Options");
               // options.add_options()("help,h", "produce this help message")("brokers,b", po::value<string>(&brokers),
               //                                                              "the kafka broker list")("directory,d", po::value<fs::path>(&dir_path)->required(),
               //                                                                                       "the directory to search")
               //     //("file,f", po::value<string>(&file_name), "if sending single file is enabled, path is disabled")
               //     ("partition,p", po::value<int>(&partition_value),
               //      "the partition to write into (deault is 0 if unassigned)")("topic,t", po::value<string>(&topic_name)->required(),
               //                                                                 "the topic in which to write to");

               // po::variables_map vm;
               // try
               // {
               //      po::store(po::command_line_parser(argc, argv).options(options).run(), vm);
               //      po::notify(vm);
               //      // make sure one of p/f is defined
               //      if (dir_path.empty() && file_name.empty())
               //      {
               //           throw "indicate directory or single file";
               //      }
               // }
               // catch (exception &ex)
               // {
               //      cout << "Error parsing options: " << ex.what() << endl;
               //      cout << endl;
               //      cout << options << endl;
               //      return 1;
               // }

               // Create a message builder for this topic
               MessageBuilder builder(topic_name);

               // default partiton is 0
               builder.partition(partition_value);

               // Create the config
               Configuration config = {
                   {"metadata.broker.list", brokers}};
               // Create the producer
               Producer producer(config);
               // define the extension of searched files
               string ext(".json");

               //if (!dir_path.empty()) {
               // Get recursive list of files in given directory and its sub directories
               vector<string> listOfFiles;
               searchDirs(dir_path, ext, listOfFiles);
               for (auto file_name_ : listOfFiles)
               {
                    dumpDataToKafka(producer, builder, file_name_);
               }
               //}
               /** Searching dir include this case
	//check if single file name is defined
	else if(!file_name.empty()){
		dumpDataToKafka(producer, builder, file_name);
	}
	**/
               producer.flush();
          }

	 };
} // namespace kafkajson
#endif