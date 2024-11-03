#include "iostream"
#include "sparkium/app/app.h"
#include "sparkium/assets/assets.h"
#include "sparkium/utils/utils.h"

int main() {
  sparkium::AppSettings app_settings;
  sparkium::Application app(app_settings);
  app.Run();
}
