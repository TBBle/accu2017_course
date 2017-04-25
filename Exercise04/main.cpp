#include <array>
#include <chrono>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/wvl.hpp>
#include <stlab/default_executor.hpp>
#include <stlab/future.hpp>
#include <thread>

class cad_report {
  int _id;
  std::string _val;

public:
  cad_report() : _id(-1) {}

  cad_report(int id) : _id(id) {
    std::this_thread::sleep_for(std::chrono::seconds(3 * (_id + 1)));
    _val = 'A' + id;
  }

  std::string val() const { return _val; }
};

class SmallWindow : public nana::form {
  static const int reportCount = 2;

public:
  SmallWindow()
      : _resetBtn(*this, "Reset", true), _oneBtn(*this, "1", true),
        _twoBtn(*this, "2", true), _label(*this, "-"),
        _status(*this, "Status: ") {
    nana::place layout(*this);
    layout.div("vert <> <<><reset_btn><>> <> <<><button_one><>> "
               "<<><button_two><>> <<><label><>> <status>");
    layout["reset_btn"] << _resetBtn;
    layout["button_one"] << _oneBtn;
    layout["button_two"] << _twoBtn;
    layout["label"] << _label;
    layout["status"] << _status;
    layout.collocate();

    _resetBtn.events().click([this]() { reset(); });
    _oneBtn.events().click([this]() { displayReport(0); });
    _twoBtn.events().click([this]() { displayReport(1); });

    _oneBtn.enabled(false);
    _twoBtn.enabled(false);
  }

private:
  void reset() {
    _status.caption("Status: Performing reset ...");
    _reports.clear();
    _reports.resize(_reportLoads.size());
    _currentReport = -1;
    _oneBtn.enabled(false);
    _twoBtn.enabled(false);

    for (size_t i = 0; i < _reportLoads.size(); ++i) {
      _reportLoads[i].reset();
      _reportLoads[i] = stlab::async(stlab::default_executor, [this, i]() {
        _reports[i] = cad_report((int)i);
        switch (i) {
        case 0:
          _oneBtn.enabled(true);
          break;
        case 1:
          _twoBtn.enabled(true);
          break;
        }
      });
    }

    _label.caption("-");
    _status.caption("Status: Reset done.");
  }

  void displayReport(int id) { showReport(_reports[id]); }

  void showReport(const cad_report &report) {
    _label.caption("Report " + report.val());
  }

  nana::button _resetBtn;
  nana::button _oneBtn;
  nana::button _twoBtn;
  nana::label _label;
  nana::label _status;

  std::vector<cad_report> _reports;

  std::atomic<int> _currentReport{-1};
  std::array<stlab::future<void>, reportCount> _reportLoads;
};

int main() {
  SmallWindow ex;
  ex.show();
  nana::exec();
  return 0;
}