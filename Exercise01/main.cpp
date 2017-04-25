#include <atomic>
#include <chrono>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/wvl.hpp>
#include <stlab/default_executor.hpp>
#include <stlab/future.hpp>
#include <thread>

class SmallWindow : public nana::form {
public:
  SmallWindow()
      : _oneBtn(*this, "Start", true), _cancelBtn(*this, "Cancel", true),
        _progressBar(*this, nana::rectangle(10, 40, 280, 20)) {
    nana::place layout(*this);
    layout.div("vert <><<><button_start><button_cancel><>><><<><progress_bar "
               "weight=80><>><>");
    layout["button_start"] << _oneBtn;
    layout["button_cancel"] << _cancelBtn;
    layout["progress_bar"] << _progressBar;
    layout.collocate();

    _oneBtn.events().click([this]() { start(); });
    _cancelBtn.events().click([this]() { cancel(); });
    _cancelBtn.enabled(false);
  }

private:
  void start() {
    _stop = false;
    _oneBtn.enabled(false);
    _cancelBtn.enabled(true);
    _progressBar.amount(50);
    auto doStuff = [this]() {
      for (int i = 0; i < 50 && !_stop; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        _progressBar.value(i + 1);
      }
      _oneBtn.enabled(true);
      _cancelBtn.enabled(false);
    };

    auto afterStuff = [this]() {};
    auto stuffDone =
        stlab::async(stlab::default_executor, doStuff); // .then(afterStuff);
    // Needed because stlab::future doens't have shared-state
    stuffDone.detach();
  }

  void cancel() { _stop = true; }

  std::atomic<bool> _stop = false;
  nana::button _oneBtn;
  nana::button _cancelBtn;
  nana::progress _progressBar;
};

int main() {
  SmallWindow ex;
  ex.show();
  nana::exec();
  return 0;
}