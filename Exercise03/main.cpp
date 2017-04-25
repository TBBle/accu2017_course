
#include <iostream>
#include <stlab/channel.hpp>
#include <stlab/default_executor.hpp>
#include <string>

using namespace stlab;
using namespace std;

struct unpack {};

struct pack {};

int main() {
  sender<string> send;
  receiver<string> receiver;
  std::tie(send, receiver) = channel<string>(default_executor);
}