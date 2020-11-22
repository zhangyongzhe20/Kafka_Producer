# cppkafka: high level C++ producer for dumping json data

# Configuration Before Compilation
* Put "cppkafka_EXPORTS" in VS "Project's Properties -> C/C++ -> Preprocessor definitions"

# Documents
* main functions are defined in `kafkajson.h`
* included libs are in folder `lib`
* included headers are in folder `headers`

## Example:
_kafkajson_'s API is simple to use. For example, this code creates a producer that writes a message to kafka server.

```c++
#include "stdafx.h"
#include "kafkajson.h"
#include <nlohmann/json.hpp>;
using json = nlohmann::json;
using kafka_json = kafkajson::dumpData;

int main() {
	string broker = "127.0.0.1:9092", topic = "test";
	json jobject;
     kafka_json kafakaDumper(broker);
     kafakaDumper.dumpToKafka(topic, jobject);
}
```



## Local Development

### Configuration Before Compilation
* put "cppkafka_EXPORTS" in VS "Project's Properties -> C/C++ -> Preprocessor definitions"
* used the modified buffered_producer.h
* Check for details [C2491](https://github.com/mfontanini/cppkafka/issues/206)


### Compiling

In order to compile you need:

* vcpkg [link](https://github.com/microsoft/vcpkg)
* boost
* librdkafka
* cppkafka

```Shell
vcpkg install boost
vcpkg install cppkafka
vcpkg install librdkafka
```


### Pass Arguments
The following options can be specified:
* `help\h` : produce this help message.
* `brokers\b` : the kafka broker list.
* `topic\t` : the topic in which to write to, required.
* `directory\d` : the directory to search.
* `file\f` : this option will be disabled if `path` is defined.
* `partition\p` : the partition to write into (unassigned if not provided).


### Documentation
Make sure to check the [wiki](https://github.com/mfontanini/cppkafka/wiki) which includes
some documentation about the project and some of its features.