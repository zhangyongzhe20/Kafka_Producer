/*
* Produce Json data to kafka server
*/

#ifndef KAFKAJSON_H
#define KAFKAJSON_H

#include "stdafx.h"
#include <iostream>
#include <cppkafka/cppkafka.h>
using namespace cppkafka;
using namespace std;

namespace kafkajson
{
     class dumpData
     {
     public:
          // pass arguments, topic & directory
          string topic = "test";
          string brokers = "127.0.0.1:9092";
         // string file_name;
          int partition_value = 0;
          //create parameterized constructors

          dumpData(string brokers_)
          {
             brokers = brokers_;
          }

          void dumpToKafka(string topic, json jobject)
          {
               // Create a message builder for this topic
               MessageBuilder builder(topic);

               // default partiton is 0
               builder.partition(partition_value);

               // Create the config
               Configuration config = {
                   {"metadata.broker.list", brokers}};
               // Create the producer
               Producer producer(config);

                // deserialized to string
               string jobject_tostring = jobject.dump();
               try{
               producer.produce(builder.payload(jobject_tostring));
               producer.flush();
               }
               catch(const HandleException& kafkaErr){
                    std::cerr << "Kafka Server not start correctly " << kafkaErr.what() << '\n';
                }
               catch(const std::exception& e){
                    std::cerr << "Error..." << e.what() << '\n';
               }
          }

	 };
} // namespace kafkajson
#endif