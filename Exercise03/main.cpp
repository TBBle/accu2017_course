
#include <array>
#include <iostream>
#include <stlab/channel.hpp>
#include <stlab/default_executor.hpp>
#include <string>

using namespace stlab;
using namespace std;

struct unpack {
  process_state_scheduled state_ = await_forever;
  std::string curLine_;
  std::string::const_iterator curChar_;

  void await(std::string inputLine) {
    curLine_ = std::move(inputLine);
    curChar_ = curLine_.begin();
    state_ = yield_immediate;
  }

  char yield() {
    char output = *curChar_++;
    if (curChar_ == curLine_.end()) {
      state_ = await_forever;
    }
    return output;
  }

  process_state_scheduled state() const { return state_; }
};

template <size_t packLen = 125> struct pack {
  process_state_scheduled state_ = await_forever;
  std::string buffer_;

  pack() { buffer_.reserve(packLen); }

  void await(char input) {
    buffer_ += (input);
    if (buffer_.size() < packLen)
      return;
    state_ = yield_immediate;
  }

  std::string yield() {
    std::string result{std::move(buffer_)};
    buffer_.reserve(packLen);
    state_ = await_forever;
    return std::move(result);
  }

  process_state_scheduled state() const { return state_; }
};

static const std::array<char[80], 10> cards{
    "tmfJ03HZFEdg8spSETT7DEMxUci9hXm6KsBPeYXpoIzYye7id43APvCwTrcfMfbfEKNPVvjY1H"
    "YGcPm",
    "ih0UGFpU22ldN5cKRrvYRzqoTAL9Xa20WDxzNpGXglrADzLrCDbVXJeZGT62to4TWFWRYm2ZQX"
    "Gv2Vl",
    "OrpgA4OyxZRt82CmH6mw64iHYXFop75q3t3Tqzf1yAQGTlsGQAlYpMry72OkA4vSZbCDIpIbW2"
    "YdtBH",
    "IZw4uoh0A1O8wqDIMV4sORFgB3IroBoEbmZw4gp6ABAVcIkSurHxako5euM9zexoediIgyKLM7"
    "s16dA",
    "THDsS7rLWg1AvK8yod9J2T0vCn0U1h61eqYpu4IuoUyNy6eTmPQbLYGZwiUp16xSzJH6CUMnP6"
    "DBfK6",
    "iM5a0Be0rIZYeLwaugU0GWhRRzL20N471KkSFm5z35NNJLKpF5Omsq9Vr7yIPTPZFYZzYBIXyT"
    "138bL",
    "ij2PXT2b5fNVDY1LsH4cfObQHX2Nsp8NYSGWko4YtVctR2zPlq5vUdzTIYfjaYtY5RumTfBFsy"
    "2Ven5",
    "UOFEwFXjEJvmpul4zqcYiWQRvVugTtmEIHkM2fDRrrjFHxrnOw9fgiij1FlZivNuRXxFD5Sqiw"
    "5AD8I",
    "lsKOSRxn5e68WgEu0N0YNyW42W60ST4Jw7J5VKboZTpxiKbzYPLvcJN5SfOhZRQVRluYvge0q7"
    "3MTu6",
    "TeQCFTNKF9xvf5rGEI0D6fZGSJ01uQ5fxFnAGEDAgyOJokQnTfz8FjHRDXmqAYkuFZ9BvYqukn"
    "GfLYc"};

int main() {
  sender<string> send;
  receiver<string> receiver;
  std::tie(send, receiver) = channel<string>(default_executor);

  auto punchcard_rewrapper = receiver | unpack{} | pack<125>{} |
                             [](auto x) { std::cout << x << "\n"; };

  receiver.set_ready();

  for (const auto &line : cards) {
    send(line);
  }

  int n;
  std::cin >> n;
}